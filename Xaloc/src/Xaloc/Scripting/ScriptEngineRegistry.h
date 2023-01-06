#pragma once

namespace Xaloc {

	/// <summary>
	/// Used to register the implementation of the C# Xaloc methods
	/// that the client scripting application is going to call.
	/// </summary>
	class ScriptEngineRegistry
	{
	public:
		static void RegisterAll();
	};

}
