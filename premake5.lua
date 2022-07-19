outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "REngine"
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
IncludeDir["stb_image"] = "%{wks.location}/RCore/vendor/stb_image"
IncludeDir["rlgl"] = "%{wks.location}/RCore/vendor/rlgl"
IncludeDir["imgui"] = "%{wks.location}/RCore/vendor/imGui"
IncludeDir["glad"] = "%{wks.location}/RCore/vendor/glad/include"
IncludeDir["spdlog"] = "%{wks.location}/RCore/vendor/spdlog/include"
IncludeDir["entt"] = "%{wks.location}/RCore/vendor/entt/include"
IncludeDir["glm"] = "%{wks.location}/RCore/vendor/glm"
IncludeDir["mono"] = "%{wks.location}/RCore/vendor/mono/include"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/RCore/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"

group "Dependencies"
	include "RCore/vendor/glfw"
	include "RCore/vendor/glad"
	include "RCore/vendor/imgui"
group ""

group "Core"
	include "RCore"
	include "RScript"
group ""
include "RGame"
	