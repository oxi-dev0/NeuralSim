workspace "BioSim"
	architecture "x64"

	configurations 
	{
		"Debug", 
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "BioSim"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir)
	objdir ("bin-int/" .. outputdir)

	files 
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"vendor/SFML/include/"
	}

	libdirs { "vendor/SFML/extlibs/libs-msvc-universal/x64", "vendor/SFML/lib" }
	links { "opengl32.lib", "freetype.lib", "winmm.lib", "gdi32.lib", "openal32.lib", "flac.lib", "vorbisenc.lib", "vorbisfile.lib", "vorbis.lib", "ogg.lib", "ws2_32.lib" }


	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

		defines "SFML_STATIC"

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
		staticruntime "on"
		runtime "Debug"

		links { "sfml-graphics-s-d.lib", "sfml-system-s-d.lib", "sfml-window-s-d.lib" }

	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"
		staticruntime "on"
		runtime "Release"

		links { "sfml-graphics-s.lib", "sfml-system-s.lib", "sfml-window-s.lib" }

	filter "configurations:Dist"
		defines "DIST"
		optimize "On"
		staticruntime "on"
		runtime "Release"

		links { "sfml-graphics-s.lib", "sfml-system-s.lib", "sfml-window-s.lib" }