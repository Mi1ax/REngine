project "RCore"
	kind "StaticLib"
	language "C++"
	
	targetdir("%{wks.location}/bin/" .. outputdir)
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rcpch.h"
	pchsource "src/rcpch.cpp"

	files 
	{
		"src/**.cpp",
		"src/**.h",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/entt/include/**.hpp",
		"vendor/stb_image/**.cpp",
        "vendor/stb_image/**.h",
	}

	links 
	{ 
		"glfw",
		"imgui",
		"glad",

		"%{Library.mono}",
	}
	
	includedirs 
	{
		"src",
		"%{IncludeDir.glfw}",
        "%{IncludeDir.rlgl}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.mono}",
	}
	
	filter "system:windows"
		systemversion "latest"

		links
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.BCrypt}",
		}

	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}

	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}
		
	filter {}