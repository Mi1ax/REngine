outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "RGame"
	configurations { "Debug", "Release" }
	platforms { "x64" }
	architecture "x86_64"
	cppdialect "C++20"

	startproject "RGame"

	targetdir("%{wks.location}/bin/" .. outputdir)
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "RELEASE" }
		optimize "On"	

	filter {}

IncludeDir = {}
IncludeDir["glfw"] = "%{wks.location}/RCore/vendor/glfw/include"
IncludeDir["raylib"] = "%{wks.location}/RCore/vendor/raylib/src"
IncludeDir["imgui"] = "%{wks.location}/RCore/vendor/imGui"
IncludeDir["spdlog"] = "%{wks.location}/RCore/vendor/spdlog/include"
IncludeDir["entt"] = "%{wks.location}/RCore/vendor/entt/include"
IncludeDir["glm"] = "%{wks.location}/RCore/vendor/glm"

group "Dependencies"
	include "RCore/vendor/raylib"
	include "RCore/vendor/glfw"
	include "RCore/vendor/imgui"
group ""

include "RCore"
include "RGame"
	