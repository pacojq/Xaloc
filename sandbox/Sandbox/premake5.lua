
project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir (_MAIN_SCRIPT_DIR .. "/bin/" .. outputdir .. "/%{prj.name}")
	objdir (_MAIN_SCRIPT_DIR .. "/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.c",
		"src/**.hpp",
		"src/**.cpp",
	}

	includedirs
	{
		_MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor/spdlog/include",
		_MAIN_SCRIPT_DIR .. "/code/Xaloc/src",
		_MAIN_SCRIPT_DIR .. "/code/Xaloc/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.mono}"
	}

	links
	{
		"Xaloc",
		"%{LibraryDir.mono}"
	}

	postbuildcommands 
	{
		'{COPY} "assets" "%{cfg.targetdir}/assets"'
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "XA_DEBUG"
		runtime "Debug"
		symbols "on"

		postbuildcommands 
		{
			'{COPY} '.. _MAIN_SCRIPT_DIR .. '"/code/Xaloc/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Release"
		defines "XA_RELEASE"
		runtime "Release"
		optimize "on"

		postbuildcommands 
		{
			'{COPY} '.. _MAIN_SCRIPT_DIR .. '"/code/Xaloc/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Dist"
		defines "XA_DIST"
		runtime "Release"
		optimize "on"

		postbuildcommands 
		{
			'{COPY} '.. _MAIN_SCRIPT_DIR .. '"/code/Xaloc/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}