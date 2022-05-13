workspace "NeuralSim"
	architecture "x64"
	flags {"MultiProcessorCompile"}

	configurations 
	{
		"Debug", 
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "NeuralSim"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir)
	objdir ("bin-int/" .. outputdir)

	buildoptions { "/bigobj" }

	postbuildcommands {
	  "{MKDIR} %{cfg.targetdir}/resources",
	  "{COPY} vendor/ffmpeg/ffmpeg.exe %{cfg.targetdir}/resources",
	  "{COPY} resources %{cfg.targetdir}/resources",
	  "{COPY} config %{cfg.targetdir}/config"
	}

	files 
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"vendor/SFML/include/",
		"vendor/spdlog/include",
		"vendor/mINI/include",
		"vendor/nodesoup/include",
		"vendor/SelbaWard/src"
	}

	libdirs { "vendor/SFML/extlibs/libs-msvc-universal/x64", "vendor/SFML/lib", "vendor/nodesoup/bin/src/Release", "vendor/nodesoup/bin/src/Debug" }
	links { "opengl32.lib", "freetype.lib", "winmm.lib", "gdi32.lib", "openal32.lib", "flac.lib", "vorbisenc.lib", "vorbisfile.lib", "vorbis.lib", "ogg.lib", "ws2_32.lib", "legacy_stdio_definitions.lib" }


	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

		defines "SFML_STATIC"

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
		staticruntime "on"
		runtime "Debug"

		links { "sfml-graphics-s-d.lib", "sfml-system-s-d.lib", "sfml-window-s-d.lib", "nodesoup-d.lib" }

	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"
		staticruntime "on"
		runtime "Release"

		links { "sfml-graphics-s.lib", "sfml-system-s.lib", "sfml-window-s.lib", "nodesoup.lib" }

	filter "configurations:Dist"
		defines "DIST"
		optimize "On"
		staticruntime "on"
		runtime "Release"

		links { "sfml-graphics-s.lib", "sfml-system-s.lib", "sfml-window-s.lib", "nodesoup.lib" }