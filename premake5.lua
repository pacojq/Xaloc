workspace "Xaloc"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

startproject "Sandbox"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Xaloc/vendor/GLFW/include"
IncludeDir["Glad"] = "Xaloc/vendor/Glad/include"
IncludeDir["ImGui"] = "Xaloc/vendor/imgui"
IncludeDir["stb_image"] = "Xaloc/vendor/stb_image"
IncludeDir["glm"] = "Xaloc/vendor/glm"

group "Dependencies"
	include "Xaloc/vendor/GLFW"
	include "Xaloc/vendor/Glad"
	include "Xaloc/vendor/imgui"

group ""


project "Xaloc"
	location "Xaloc"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "xapch.h"
	pchsource "Xaloc/src/xapch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}"
	}
	
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "XA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "XA_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "XA_DIST"
		runtime "Release"
		optimize "on"



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Xaloc/vendor/spdlog/include",
		"Xaloc/src",
		"Xaloc/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Xaloc"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "XA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "XA_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "XA_DIST"
		runtime "Release"
		optimize "on"