workspace "Xaloc"
	architecture "x64"
	targetdir "build"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	startproject "XalocEditor"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = _MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor/GLFW/include"
IncludeDir["Glad"] = _MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor/Glad/include"
IncludeDir["ImGui"] = _MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor/imgui"
IncludeDir["ImPlot"] = _MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor/implot"
IncludeDir["stb_image"] = _MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor/stb_image"
IncludeDir["glm"] = _MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor/glm"
IncludeDir["entt"] = _MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor/entt/single_include"
IncludeDir["mono"] = _MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor/mono/include"
IncludeDir["pugixml"] = _MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor/pugixml"

LibraryDir = {}
LibraryDir["mono"] = _MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor/mono/lib/Debug/mono-2.0-sgen.lib"


group "Dependencies"
	include "code/Xaloc/vendor/GLFW"
	include "code/Xaloc/vendor/Glad"
	include "code/Xaloc/vendor/imgui"
group ""


group "Core"
	include "code/Xaloc"
	include "code/XalocSharp"
group ""


group "Tools"
	include "code/XalocEditor"
group ""


group "Sandbox"
	include "sandbox/Sandbox"
	include "sandbox/SandboxCS"
group ""
