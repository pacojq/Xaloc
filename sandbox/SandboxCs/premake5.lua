project "SandboxCs"
	location "SandboxCs"
	kind "SharedLib"
	language "C#"

	targetdir ("../Sandbox/assets/scripts")
	objdir (_MAIN_SCRIPT_DIR .. "/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.cs", 
	}

	links
	{
		"XalocSharp"
	}
