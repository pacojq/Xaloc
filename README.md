<h1 align="center" style="border-bottom: none;">
  <a href="https://github.com/pacojq/Xaloc/">Xaloc Engine</a>
</h1>

<p align="center">
  <img height="200" src="res/xaloc-logo.png" alt="Xaloc Engine logo" />
</p>

<p align="center">
  <a href=""><img alt="platforms" src="https://img.shields.io/badge/platform-Windows-blue?style=flat-square"/></a>
  <img alt="GitHub" src="https://img.shields.io/github/license/pacojq/Xaloc?color=blue&style=flat-square">
  <a href=""><img alt="size" src="https://img.shields.io/github/repo-size/pacojq/Xaloc?style=flat-square"/></a>
  <br/>
  <!-- <a href=""><img alt="stars" src="https://img.shields.io/github/stars/pacojq/Xaloc?style=social"/></a> -->
  <img alt="Twitter Follow" src="https://img.shields.io/twitter/follow/_thisIsPJ?style=social">
</p>

> Sirocco (Catalan: _Xaloc_).
> 
> From arabic, _shaluq_ 'wind from the sea' .
> 
> A Mediterranean wind that comes from the Sahara and can reach hurricane speeds in North Africa and Southern Europe, especially during the summer season.


## About

Xaloc is an in-development 2D game engine written in C++, allowing an ECS approach and C# scripting.

I develop it in my spare time as a personal project, so expect frequent periods of time with no activity in this repository.

<p align="center">
    <img height="400" src="res/sandbox.png" alt="Screenshot of an example application built with Xaloc." />
</p>

## Set up

The recommended environment to develop the Xaloc Engine is Visual Studio 2019.

You can clone Xaloc using git. Make sure you do a ```--recursive``` clone!

```git clone --recursive https://github.com/pacojq/Xaloc```

If you are working on Windows, execute the script ```scripts/Win-GenProjects.bat``` 
to generate the solution and project files.

### Vulkan 

To work with the Vulkan API, you should first install the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home).


## Execution

The default startup project is the ```Sandbox``` App.

Remember to build the **entire solution** if you modify the C# files in ```SandboxCs```, since the dll file for the
scripting project is generated appart from the main Sandbox application.

<p align="center">
    <img height="400" src="res/csharp-demo.gif" alt="Example of C# scripting." />
</p>

## Current Features

Right now, Xaloc is very bare-bones. Its most basic features are:

 - Basic Scene system.
 - ECS support.
 - Scene XML serialization.
 - C# scripting.
 - In-game debug UI using ImGui.
 - Simple 2D batch renderer.
 - OpenGL support.
 
 > For in-development features, check the [dev branch](https://github.com/pacojq/Xaloc/tree/dev).
 

## Planned Features

Other features coming up in the future are:

 - Editor.
 - Audio system.
 - 2D physics.
 - Support for Vulkan.

The game editor is currently work-in-progress:

<p align="center">
    <img height="400" src="res/sandbox-editor.png" alt="Screenshot of a work-in-progress editor." />
</p>


## Dependencies

Xaloc has the following dependencies:

  - [ImGui](https://github.com/ocornut/imgui) for GUI rendering.
  - [mono](https://www.mono-project.com/docs/about-mono/) for C# [scripting](https://www.mono-project.com/docs/advanced/embedding/scripting/).
  - [GLFW](https://github.com/glfw/glfw) for OpenGL, OpenGL ES, window creation 
  and input handling. More info at its [website](https://www.glfw.org/).
  - [Glad](https://glad.dav1d.de) for OpenGL rendering.
  - [glm](https://github.com/g-truc//glm) as math(s) library.
  - [entt](https://github.com/skypjack/entt) for ECS management.
  - [spdlog](https://github.com/gabime/spdlog) for logging.
  - [stb_image.h](https://github.com/nothings/stb) for loading textures.
  - [pugixml](https://pugixml.org) for scene serialization in XML.

Projects are generated with [Premake 5](https://github.com/premake/premake-core/releases).

## Special Mentions

Huge shout out to TheCherno's [video series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT). Original code at [TheCherno/Hazel](https://github.com/TheCherno/Hazel).
