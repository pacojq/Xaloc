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
IncludeDir["GLFW"] = "Xaloc/vendor/GLFW/include"
IncludeDir["Glad"] = "Xaloc/vendor/Glad/include"
IncludeDir["ImGui"] = "Xaloc/vendor/imgui"
IncludeDir["ImPlot"] = "Xaloc/vendor/implot"
IncludeDir["stb_image"] = "Xaloc/vendor/stb_image"
IncludeDir["glm"] = "Xaloc/vendor/glm"
IncludeDir["entt"] = "Xaloc/vendor/entt/single_include"
IncludeDir["mono"] = "Xaloc/vendor/mono/include"
IncludeDir["pugixml"] = "Xaloc/vendor/pugixml"

LibraryDir = {}
LibraryDir["mono"] = "vendor/mono/lib/Debug/mono-2.0-sgen.lib"



group "Dependencies"
	include "Xaloc/vendor/GLFW"
	include "Xaloc/vendor/Glad"
	include "Xaloc/vendor/imgui"
group ""



group "Core"

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
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		
		"%{prj.name}/vendor/implot/implot.h",
		"%{prj.name}/vendor/implot/implot.cpp",
		"%{prj.name}/vendor/implot/implot_internal.h",
		"%{prj.name}/vendor/implot/implot_items.cpp",
		
		"%{prj.name}/vendor/pugixml/**.hpp",
		"%{prj.name}/vendor/pugixml/**.cpp"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImPlot}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.pugixml}"
	}
	
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"%{LibraryDir.mono}"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"XA_PLATFORM_WINDOWS"
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
		optimize "On"

		
		
project "XalocSharp"
	location "XalocSharp"
	kind "SharedLib"
	language "C#"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.cs", 
	}
	
group ""



group "Tools"

project "XalocEditor"
	location "XalocEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Xaloc/vendor/spdlog/include",
		"Xaloc/src",
		"Xaloc/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Xaloc"
	}

	postbuildcommands 
	{
		'{COPY} "../XalocEditor/assets" "%{cfg.targetdir}/assets"'
	}
	
	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"XA_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "XA_DEBUG"
		runtime "Debug"
		symbols "on"
		
		postbuildcommands 
		{
			'{COPY} "../Xaloc/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}
		

	filter "configurations:Release"
		defines "XA_RELEASE"
		runtime "Release"
		optimize "on"
		
		postbuildcommands 
		{
			'{COPY} "../Xaloc/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Dist"
		defines "XA_DIST"
		runtime "Release"
		optimize "on"
		
		postbuildcommands 
		{
			'{COPY} "../Xaloc/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}

group ""




	
group "Sandbox"	

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
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Xaloc/vendor/spdlog/include",
		"Xaloc/src",
		"Xaloc/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Xaloc"
	}

	postbuildcommands 
	{
		'{COPY} "../Sandbox/assets" "%{cfg.targetdir}/assets"'
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "XA_DEBUG"
		runtime "Debug"
		symbols "on"
		
		postbuildcommands 
		{
			'{COPY} "../Xaloc/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}
		

	filter "configurations:Release"
		defines "XA_RELEASE"
		runtime "Release"
		optimize "on"
		
		postbuildcommands 
		{
			'{COPY} "../Xaloc/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Dist"
		defines "XA_DIST"
		runtime "Release"
		optimize "on"
		
		postbuildcommands 
		{
			'{COPY} "../Xaloc/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}

project "SandboxCs"
	location "SandboxCs"
	kind "SharedLib"
	language "C#"

	targetdir ("Sandbox/assets/scripts")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.cs", 
	}

	links
	{
		"XalocSharp"
	}


