include "Dependencies.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "REngine"
	configurations { "Debug", "Release" }
	platforms { "x64" }
	architecture "x86_64"
	cppdialect "C++20"

	startproject "REngine"

	targetdir("%{wks.location}/bin/" .. outputdir)
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "RELEASE" }
		optimize "On"	

	filter {}

group "Dependencies"
	include "RCore/vendor/glfw"
	include "RCore/vendor/glad"
	include "RCore/vendor/imgui"
group ""

group "Core"
	include "RCore"
	include "RScript"
group ""


include "REngine"