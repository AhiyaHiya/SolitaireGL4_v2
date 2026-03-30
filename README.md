## Solitaire

A classic fun card game, written to demonstrate using OpenGL as your renderer, as well as the other mechanisms needed to handle assets and such.

Tested on Linux with X11 and with NVIDIA driver version 580.82.09.

### Note
GLAD is downloaded via glad.cmake file because vcpkg port only has older 1.x release.

### Setup

GLAD uses jinja2 in Python, so:

```
python3 -m pip install --user jinja2
# or
python -m pip install --user jinja2
```

Linux needed packages (Debian based)

```
sudo apt update
sudo apt-get install -y \
        freeglut3-dev \
        libglew-dev \
        libglm-dev \
        libglfw3-dev \
```

CMake command for Linux:
```
cmake -S . -B build -G "Ninja"
```
CMake command for Windows

```
cmake -S . -B build -G "Visual Studio 18 2026"
```

### Refs

C++ Bindings for OpenGL: https://github.com/cginternals/glbinding/

GLFW: https://www.glfw.org/

Mesa: https://mesa3d.org/

### Assets attribution

http://code.google.com/p/vector-playing-cards/

