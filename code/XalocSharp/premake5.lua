project "XalocSharp"
	location "XalocSharp"
	kind "SharedLib"
	language "C#"

	targetdir (_MAIN_SCRIPT_DIR .. "/bin/" .. outputdir .. "/%{prj.name}")
	objdir (_MAIN_SCRIPT_DIR .. "/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.cs", 
	}