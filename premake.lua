workspace "hfsm2"
	configurations {
		"debug",
		"release"
	}
	conformancemode "On"
	debugdir "."
	defines {
		"UNICODE",
		"_UNICODE",
	}
	filename "hfsm2-all"
	flags {
		"FatalCompileWarnings",
		"NoPCH",
	}
	includedirs {
		"development",
		"include",
		"external",
	}
	language "C++"
	location "projects/premake"
	objdir "$(BUILD_ROOT)/$(SolutionName)-$(PlatformArchitecture)/$(ProjectName)-$(Configuration)/"
	platforms {
		"32",
		"64",
	}

	system "windows"
	systemversion "latest"
	--systemversion "$([Microsoft.Build.Utilities.ToolLocationHelper]::GetLatestSDKTargetPlatformVersion('Windows', '10.0'))"

	targetdir "binaries-$(PlatformArchitecture)/"
	targetname "$(ProjectName)-$(Configuration)-$(PlatformArchitecture)"
	warnings "High"

	filter "toolset:msc-v140 or msc-v141"
		if os.getversion().majorversion == 10 then
			local sWin10SDK = os.getWindowsRegistry( "HKLM:SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
			if sWin10SDK ~= nil then systemversion( sWin10SDK .. ".0" ) end
		end

	filter "configurations:debug"
		defines "_DEBUG"
		symbols "On"

	filter "configurations:release"
		defines "NDEBUG"
		intrinsics "On"
		optimize "Speed"
		flags {
			"LinkTimeOptimization",
		}

	filter { "platforms:32" }
		architecture "x86"

	filter { "platforms:64" }
		architecture "x86_64"

	filter "toolset:msc-ClangCL"
		buildoptions {
			"-Wpedantic",
		}
		linkoptions {
			"/INCREMENTAL:NO"
		}

	filter "toolset:msc-v143"
		buildoptions {
			"/permissive-",
		}

-- advanced_event_handling

project "advanced_event_handling-14"
	cppdialect "C++11"
	files {
		"examples/advanced_event_handling/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v140"

project "advanced_event_handling-15"
	cppdialect "C++14"
	files {
		"examples/advanced_event_handling/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v141"

project "advanced_event_handling-16"
	cppdialect "C++17"
	files {
		"examples/advanced_event_handling/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v142"

project "advanced_event_handling-17"
	cppdialect "C++20"
	files {
		"examples/advanced_event_handling/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v143"

project "advanced_event_handling-clang"
	cppdialect "C++20"
	files {
		"examples/advanced_event_handling/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-ClangCL"

-- basic_audio_player

project "basic_audio_player-14"
	cppdialect "C++11"
	files {
		"examples/basic_audio_player/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v140"

project "basic_audio_player-15"
	cppdialect "C++14"
	files {
		"examples/basic_audio_player/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v141"

project "basic_audio_player-16"
	cppdialect "C++17"
	files {
		"examples/basic_audio_player/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v142"

project "basic_audio_player-17"
	cppdialect "C++20"
	files {
		"examples/basic_audio_player/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v143"

project "basic_audio_player-clang"
	cppdialect "C++20"
	files {
		"examples/basic_audio_player/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-ClangCL"

-- basic_traffic_light

project "basic_traffic_light-14"
	cppdialect "C++11"
	files {
		"examples/basic_traffic_light/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v140"

project "basic_traffic_light-15"
	cppdialect "C++14"
	files {
		"examples/basic_traffic_light/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v141"

project "basic_traffic_light-16"
	cppdialect "C++17"
	files {
		"examples/basic_traffic_light/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v142"

project "basic_traffic_light-17"
	cppdialect "C++20"
	files {
		"examples/basic_traffic_light/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v143"

project "basic_traffic_light-clang"
	cppdialect "C++20"
	files {
		"examples/basic_traffic_light/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-ClangCL"

-- calculator

project "calculator-14"
	cppdialect "C++11"
	files {
		"examples/calculator/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v140"

project "calculator-15"
	cppdialect "C++14"
	files {
		"examples/calculator/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v141"

project "calculator-16"
	cppdialect "C++17"
	files {
		"examples/calculator/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v142"

project "calculator-17"
	cppdialect "C++20"
	files {
		"examples/calculator/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v143"

project "calculator-clang"
	cppdialect "C++20"
	files {
		"examples/calculator/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-ClangCL"

-- debug_logger_interface

project "debug_logger_interface-14"
	cppdialect "C++11"
	files {
		"examples/debug_logger_interface/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v140"

project "debug_logger_interface-15"
	cppdialect "C++14"
	files {
		"examples/debug_logger_interface/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v141"

project "debug_logger_interface-16"
	cppdialect "C++17"
	files {
		"examples/debug_logger_interface/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v142"

project "debug_logger_interface-17"
	cppdialect "C++20"
	files {
		"examples/debug_logger_interface/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v143"

project "debug_logger_interface-clang"
	cppdialect "C++20"
	files {
		"examples/debug_logger_interface/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-ClangCL"

-- snippets

project "snippets-14"
	cppdialect "C++11"
	files {
		"development/**.*",
		"snippets/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v140"

project "snippets-15"
	cppdialect "C++14"
	files {
		"development/**.*",
		"snippets/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v141"

project "snippets-16"
	cppdialect "C++17"
	files {
		"development/**.*",
		"snippets/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v142"

project "snippets-17"
	cppdialect "C++20"
	files {
		"development/**.*",
		"snippets/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v143"

project "snippets-clang"
	cppdialect "C++20"
	files {
		"development/**.*",
		"snippets/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-ClangCL"

-- temp

project "temp-14"
	cppdialect "C++11"
	files {
		"development/**.*",
		"temp/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v140"

project "temp-15"
	cppdialect "C++14"
	files {
		"development/**.*",
		"temp/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v141"

project "temp-16"
	cppdialect "C++17"
	files {
		"development/**.*",
		"temp/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v142"

project "temp-17"
	cppdialect "C++20"
	files {
		"development/**.*",
		"temp/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v143"

project "temp-clang"
	cppdialect "C++20"
	files {
		"development/**.*",
		"temp/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-ClangCL"

-- test

project "test-14"
	cppdialect "C++11"
	files {
		"development/**.*",
		"test/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v140"

project "test-15"
	cppdialect "C++14"
	files {
		"development/**.*",
		"test/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v141"

project "test-16"
	cppdialect "C++17"
	files {
		"development/**.*",
		"test/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v142"

project "test-17"
	cppdialect "C++20"
	files {
		"development/**.*",
		"test/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-v143"

project "test-clang"
	cppdialect "C++20"
	files {
		"development/**.*",
		"test/**.*",
	}
	kind "ConsoleApp"
	toolset "msc-ClangCL"
