# birch_engine

Implementing the "BirchEngine" SDL2 project from [^1] [Let's Make Games - Youtube](https://www.youtube.com/playlist?list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx).  The original code and design is solely the property of [Carl Birch](www.youtube.com/@CarlBirch) with my changes as I follow along.

> **Introduction from the Original Author**<br/>
> This 2D engine will be produced via an online series from my youtube channel: [Let's make games](www.youtube.com/carlbirch).
You can watch and learn the skills required to do this here: https://goo.gl/VERhC5

## Libraries

This project is written for [**SDL v2.32.10**](https://github.com/libsdl-org/SDL/releases?page=2#release-release-2.32.10)
and [**CMake**](https://cmake.org/download/#latest) v3.21 or later

1. You will need SDL2, [SDL_image](https://github.com/libsdl-org/SDL_image/releases#release-release-2.8.12), [SDL_ttf](https://github.com/libsdl-org/SDL_ttf/releases#release-release-2.24.0) and the [Open Source Liberation font](https://github.com/liberationfonts/liberation-fonts/releases/tag/2.1.5)
1. API references can be found here: https://wiki.libsdl.org/

## Getting started

These instructions assume that you already have an approriate version of CMAKE
and SDL installed.
```shell
cmake -B build ..
cmake --build build
```
To install (e.g. for deployment)
```shell
cmake --install build --install-prefix <absolute path to install dir>
```

>    _Note_: Install and library paths can be manky on Windows.  If needed the
    SDL2 library and header paths can be hinted for use with the `find_package`
    cmake command using either an environment variable `CMAKE_PREFIX_PATH` for
    any cmake projects or specifying the `CMAKE_PREFIX_PATH` definition on the
    command line using the `-DCMAKE_PREFIX_PATH=<path>` argument when initially
    parsing the ==CMakeLists.txt== file.  Similarly the default installation
    paths can be overridden using either the `CMAKE_INSTALL_PREFIX` environment
    variable or specifying the argument on the command line when parsing the
    CMakeLists.txt file using the `-D` argument.

## Contribute
_NOTE: Copied from the original author's README_
If you would like to contribute any ideas for the original codebase then you can contact me (Carl Birch) at:
[www.youtube.com/carlbirch] or
[www.facebook.com/gamesfromscratch]

[^1]: I have almost certainly used different formatting and coding style in a
few places and I have also added the reset `R` and quit `ESC` key handling.

