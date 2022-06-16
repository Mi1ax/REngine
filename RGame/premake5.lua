project "RGame"
	kind "ConsoleApp"
	language "C++"
	
	targetdir("%{wks.location}/bin/" .. outputdir)
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.cpp",
		"src/**.h"
	}

	links 
	{ 
		"glfw",
		"imgui",
		"raylib",
        "RCore"
	}
	
	includedirs 
	{
		"src",
		"%{wks.location}/RCore/src",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.spdlog}",
        "%{IncludeDir.raylib}",
        "%{IncludeDir.imgui}"
	}
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}

	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}
		
	filter {}