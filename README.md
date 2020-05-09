# Xaloc Engine

<p align="center">
  <img height="350" src="res/xaloc-logo.png" alt="Xaloc Engine logo" />
</p>

> Sirocco (Catalan: _Xaloc_).
> 
> From arabic, _shaluq_ 'wind from the sea' .
> 
> A Mediterranean wind that comes from the Sahara and can reach hurricane speeds in North Africa and Southern Europe, especially during the summer season.

## About

Xaloc is an in-development 2D game engine written in C++. 

I develop it in my spare time as a personal project, so expect frequent periods of time with no activity in this repository.


## Set up

The recommended environment to develop the Xaloc Engine is Visual Studio 2019.

You can clone Xaloc using git. Make sure you do a ```--recursive``` clone!

```git clone --recursive https://github.com/pacojq/Xaloc```

If you are working on Windows, execute the script ```scripts/Win-GenProjects.bat``` 
to generate the solution and project files.


## Dependencies

Xaloc has the following dependencies:

  - [ImGui](https://github.com/pacojq/imgui) for GUI rendering.
  - [GLFW](https://github.com/pacojq/glfw) for OpenGL, OpenGL ES, window creation 
  and input handling. More info at its [website](https://www.glfw.org/).
  - [Glad](https://glad.dav1d.de) for OpenGL rendering.
  - [glm](https://github.com/pacojq/glm) as math(s) library.
  - [spdlog](https://github.com/gabime/spdlog) for logging.
  - [stb_image.h](https://github.com/nothings/stb) for loading textures.

Projects are generated with [Premake 5](https://github.com/premake/premake-core/releases).

## Special Mentions

Huge shout out to TheCherno's [video series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT). Original code at [TheCherno/Hazel](https://github.com/TheCherno/Hazel).
