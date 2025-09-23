project "glm"
	kind "None"
	language "C"
	staticruntime "on"
	warnings "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"glm/**.hpp",
		"glm/**.inl"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		buildoptions "/utf-8"

		filter "configurations:Debug"
			defines "DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "RELEASE"
			runtime "Release"
			optimize "On"
