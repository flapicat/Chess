workspace "Chess"
	architecture "x64"
	startproject "Chess"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Chess/vendor/GLFW/include"
IncludeDir["glad"] = "Chess/vendor/glad/include"
IncludeDir["ImGui"] = "Chess/vendor/imgui"
IncludeDir["glm"] = "Chess/vendor/glm"
IncludeDir["stb"] = "Chess/vendor/stb/include"

group "Dependencies"
	include "Chess/vendor/GLFW"
	include "Chess/vendor/glad"
	include "Chess/vendor/imgui"
	include "Chess/vendor/stb"
group ""

project "Chess"
	location "Chess"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	buildoptions "/utf-8"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGui}/backends",
		"%{IncludeDir.stb}",
		"%{IncludeDir.glm}"
	}
	
	links 
	{ 
		"GLFW",
		"glad",
		"ImGui",
		"stb",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

