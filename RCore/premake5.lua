project "RCore"
	kind "StaticLib"
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
		"raylib"
	}
	
	includedirs 
	{
		"src",
		"%{IncludeDir.glfw}",
        "%{IncludeDir.raylib}",
        "%{IncludeDir.imgui}"
	}
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}

	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}
		
	filter {}