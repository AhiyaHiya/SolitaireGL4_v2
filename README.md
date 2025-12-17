## Solitaire

A classic fun card game, written to demonstrate using OpenGL as your renderer, as well as the other mechanisms needed to handle assets and such.

Tested on Linux with X11 and with NVIDIA driver version 580.82.09.


### Setup

For Linux (Debian based)

```
sudo apt update
sudo apt-get install -y \
        freeglut3-dev \
        libglew-dev \
        libglm-dev \
        libglfw3-dev \
        
```

Python dependency for GLAD:

```
python3 -m pip install --user jinja2
```

### Refs

C++ Bindings for OpenGL: https://github.com/cginternals/glbinding/

GLFW: https://www.glfw.org/

Mesa: https://mesa3d.org/

### Assets attribution

http://code.google.com/p/vector-playing-cards/

