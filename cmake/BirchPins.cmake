# Pinned toolchain and dependency versions (SC-41, SC-42), and the per-build
# record of what was actually used.
#
# Changing a pin is a one-line change here, and MUST be reviewed like a code
# change: a new SDL release or compiler update changes the shipped product.
#
# Enforcement:
#   - A mismatch is always reported as a configure WARNING.
#   - Release configs (Release, RelWithDebInfo, MinSizeRel) MUST NOT build
#     with a mismatch: single-config generators stop at configure, and every
#     generator (including Visual Studio, where the config is only known at
#     build time) fails the birch_pin_check target.
#   - BIRCH_ALLOW_UNPINNED=ON lets a release config build anyway. It is a
#     deviation: it is written into build-info.txt so the record shows it.
#
# DEVIATION (SC-42): the versions match on every platform, but the binaries
# do not come from one pinned source:
#   - Windows: official libsdl-org VC development packages, installed by hand
#     and found through CMAKE_PREFIX_PATH.
#   - Linux: SDL2 and SDL2_ttf from Ubuntu packages (Debian-patched builds);
#     SDL2_image built locally from the signed 2.8.12 release tarball with
#     SDL2IMAGE_AVIF=OFF, installed to /usr/local. Ubuntu's
#     libsdl2-image-dev (2.8.8) MUST NOT be installed alongside it: its
#     header shadows the /usr/local one (caught by the static_assert in
#     Game.cpp).
# Risk: same versions, different patches and build options, so behaviour can
# still differ between platforms; a distro update can change SDL2 or
# SDL2_ttf (caught by the pin check). Accepted for development builds;
# resolved by one package source for all platforms (stage 3, e.g. vcpkg).

# Dependencies. SDL2_image 2.8.12 is the version checked in the 2026-11-01
# supply-chain review.
set(BIRCH_PIN_SDL2       "2.32.10")
set(BIRCH_PIN_SDL2_image "2.8.12")
set(BIRCH_PIN_SDL2_ttf   "2.24.0")

# Compilers, by CMAKE_CXX_COMPILER_ID. An ID not listed here is unpinned and
# counts as a mismatch.
set(BIRCH_PIN_COMPILER_GNU  "15.2.0")          # Ubuntu GCC
set(BIRCH_PIN_COMPILER_MSVC "19.51.36260.0")   # Visual Studio 18 2026

option(BIRCH_ALLOW_UNPINNED
    "Allow release configs to build with unpinned dependency or compiler versions (deviation, recorded in build-info.txt)"
    OFF)

# Compare the found versions with the pins. Call after find_package().
# Sets BIRCH_PIN_MISMATCHES (list of messages) in the caller's scope.
function(birch_check_pins)
    set(mismatches "")

    # Where each package came from. On Windows SDL is a manual install found
    # through CMAKE_PREFIX_PATH (environment or -D), so the location can
    # change without any change to the project; the version check below is
    # what catches a different copy. Logged at configure time only: absolute
    # paths are kept out of build-info.txt because it ships with the build.
    foreach(pkg IN ITEMS SDL2 SDL2_image SDL2_ttf)
        message(STATUS "${pkg} ${${pkg}_VERSION} from ${${pkg}_DIR}")
    endforeach()

    foreach(pkg IN ITEMS SDL2 SDL2_image SDL2_ttf)
        if(NOT "${${pkg}_VERSION}" VERSION_EQUAL "${BIRCH_PIN_${pkg}}")
            list(APPEND mismatches
                "${pkg}: found '${${pkg}_VERSION}', pinned ${BIRCH_PIN_${pkg}}")
        endif()
    endforeach()

    set(id "${CMAKE_CXX_COMPILER_ID}")
    if(NOT DEFINED BIRCH_PIN_COMPILER_${id})
        list(APPEND mismatches
            "compiler: ${id} ${CMAKE_CXX_COMPILER_VERSION} has no pin")
    elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL BIRCH_PIN_COMPILER_${id})
        list(APPEND mismatches
            "compiler: ${id} ${CMAKE_CXX_COMPILER_VERSION}, pinned ${BIRCH_PIN_COMPILER_${id}}")
    endif()

    set(BIRCH_PIN_MISMATCHES "${mismatches}" PARENT_SCOPE)
endfunction()

# Report and enforce the result of birch_check_pins() for `target`.
function(birch_enforce_pins target)
    if(NOT BIRCH_PIN_MISMATCHES)
        message(STATUS "Pinned versions: all match")
        return()
    endif()

    list(JOIN BIRCH_PIN_MISMATCHES "\n  " text)
    message(WARNING "Unpinned versions (SC-41/SC-42):\n  ${text}")

    if(BIRCH_ALLOW_UNPINNED)
        message(WARNING "BIRCH_ALLOW_UNPINNED=ON: release configs will build anyway (deviation, recorded in build-info.txt)")
        return()
    endif()

    get_property(multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(NOT multi_config AND CMAKE_BUILD_TYPE MATCHES "^(Release|RelWithDebInfo|MinSizeRel)$")
        message(FATAL_ERROR "Release build with unpinned versions. Fix the versions, or set BIRCH_ALLOW_UNPINNED=ON and accept the deviation.")
    endif()

    # Build-time check, for generators that choose the config at build time.
    # `cmake -E false` fails the build; `true` passes.
    set(is_release "$<CONFIG:Release,RelWithDebInfo,MinSizeRel>")
    add_custom_target(birch_pin_check
        COMMAND ${CMAKE_COMMAND} -E echo "Unpinned versions: see the configure warning (SC-41/SC-42)"
        COMMAND ${CMAKE_COMMAND} -E $<IF:${is_release},false,true>
        COMMENT "Checking pinned versions"
        VERBATIM
    )
    add_dependencies(${target} birch_pin_check)
endfunction()

# Pass the versions CMake found to the compiler, so the code can check them
# against the headers it actually includes (static_assert in Game.cpp).
# Needed because the pin check reads each package's version file, not the
# header the compiler picks up: with two installs, an earlier include
# directory can shadow the found package's header (seen on Linux with Ubuntu
# SDL2_image 2.8.8 headers shadowing a /usr/local 2.8.13 install).
function(birch_add_version_definitions target)
    foreach(pkg IN ITEMS SDL2 SDL2_image SDL2_ttf)
        if(NOT "${${pkg}_VERSION}" MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)")
            message(FATAL_ERROR "${pkg}: cannot parse found version '${${pkg}_VERSION}'")
        endif()
        string(TOUPPER "${pkg}" up)
        target_compile_definitions(${target} PRIVATE
            BIRCH_FOUND_${up}_MAJOR=${CMAKE_MATCH_1}
            BIRCH_FOUND_${up}_MINOR=${CMAKE_MATCH_2}
            BIRCH_FOUND_${up}_PATCH=${CMAKE_MATCH_3}
        )
    endforeach()
endfunction()

# Write build-info.txt next to the executable, one per config, recording the
# toolchain and dependency versions this build used (SC-41).
function(birch_write_build_info target)
    # Git state is captured at configure time. Re-run configure when HEAD or
    # the index changes, so the commit stays current; unstaged edits made
    # after configuring are not seen, hence the "at configure" label.
    set(git_state "unknown (git not found or not a repository)")
    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --always --dirty --tags
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE git_out
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE git_rc
            ERROR_QUIET
        )
        if(git_rc EQUAL 0)
            set(git_state "${git_out}")
        endif()
        foreach(f IN ITEMS HEAD index)
            if(EXISTS "${PROJECT_SOURCE_DIR}/.git/${f}")
                set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
                    "${PROJECT_SOURCE_DIR}/.git/${f}")
            endif()
        endforeach()
    endif()

    if(BIRCH_PIN_MISMATCHES)
        list(JOIN BIRCH_PIN_MISMATCHES "\n  " mismatch_text)
        if(BIRCH_ALLOW_UNPINNED)
            set(pin_state "UNPINNED, release override ON (deviation)\n  ${mismatch_text}")
        else()
            set(pin_state "UNPINNED (release configs blocked)\n  ${mismatch_text}")
        endif()
    else()
        set(pin_state "all match")
    endif()

    set(content
"${PROJECT_NAME} ${PROJECT_VERSION}
Config:      $<CONFIG>
Git:         ${git_state} (at configure)
Compiler:    ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}
Generator:   ${CMAKE_GENERATOR}
CMake:       ${CMAKE_VERSION}
Host system: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION}
SDL2:        ${SDL2_VERSION}
SDL2_image:  ${SDL2_image_VERSION}
SDL2_ttf:    ${SDL2_ttf_VERSION}
Pins:        ${pin_state}
")
    file(GENERATE
        OUTPUT "$<TARGET_FILE_DIR:${target}>/build-info.txt"
        CONTENT "${content}"
        TARGET ${target}
    )
endfunction()
