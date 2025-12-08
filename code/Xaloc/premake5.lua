project "Xaloc"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir (_MAIN_SCRIPT_DIR .. "/bin/" .. outputdir .. "/%{prj.name}")
	objdir (_MAIN_SCRIPT_DIR .. "/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "xapch.h"
	pchsource "src/xapch.cpp"

	files
	{
		"src/**.h",
		"src/**.c",
		"src/**.hpp",
		"src/**.cpp",
		
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		
		"vendor/implot/implot.h",
		"vendor/implot/implot.cpp",
		"vendor/implot/implot_internal.h",
		"vendor/implot/implot_items.cpp",
		
		"vendor/pugixml/**.hpp",
		"vendor/pugixml/**.cpp"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"vendor",
		"vendor/spdlog/include",
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
