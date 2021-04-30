local LOCATION = "../.build_genie/"
if _ACTION == nil then
    LOCATION = LOCATION .. "vs2019"
else
    LOCATION = LOCATION .. _ACTION
end

local ROOT_DIR = path.getabsolute("../")
local BINARY_DIR = LOCATION .. "/bin/"

function defaultTargetDir()
    configuration {"Debug"}
        targetdir(BINARY_DIR .. "Debug")

    configuration {"Development"}
        targetdir(BINARY_DIR .. "Development")

    configuration {"Profile"}
        targetdir(BINARY_DIR .. "Profile")

    configuration {"Release"}
        targetdir(BINARY_DIR .. "Release")

    configuration {}
end

function dependenciesIncDir()
    includedirs {
        "../dependencies/imgui",
        "../dependencies/fmt/include",
        "../dependencies/strconv",
        "../dependencies/stb",
    }
end

function setLibDir()
    configuration {"Debug"}
        libdirs(BINARY_DIR .. "Debug")

    configuration {"Development"}
        libdirs(BINARY_DIR .. "Development")

    configuration {"Profile"}
        libdirs(BINARY_DIR .. "Profile")

    configuration {"Release"}
        libdirs(BINARY_DIR .. "Release")

    configuration {}
end

solution "PolarBear"
    location(LOCATION)
    language "C++"
    configurations { "Debug", "Development", "Profile", "Release" }
    platforms { "x64" }
    startproject "app"
    flags {
        "CppLatest",
        "UseObjectResponseFile",
        "UseLDResponseFile",
        "LinkSupportCircularDependencies",
        "FatalWarnings",
        "NoExceptions",
        "NoRTTI",
        "NoEditAndContinue",
        "Unicode",
    }

    configuration { "vs*" }
        defines { "_HAS_EXCEPTIONS=0" }
        buildoptions { "/wd4503", "/wd4819"}

    configuration "not windows"
        removefiles { "../PolarBear/**/windows/*", "../PolarBear/**/Windows/*"}

    configuration {}

project "Dependencies"
    kind "StaticLib"

    defaultTargetDir()

    configuration {}

    configuration {"Debug"}
        defines { "DEBUG" }
        flags { "FullSymbols", "Symbols", "DebugRuntime" }

    configuration {"Development"}
        defines { "DEBUG" }
        flags { "Symbols", "Optimize", "DebugRuntime" }

    configuration {"Profile"}
        defines { "DEBUG", "PB_DEBUG", "PB_PROFILE" }
        flags { "Symbols", "Optimize" }

    configuration {"Release"}
        defines { "NDEBUG" }
        flags { "OptimizeSpeed", "No64BitChecks" }

    configuration {}

    dependenciesIncDir()

    files {
        "../dependencies/imgui/imconfig.h",
        "../dependencies/imgui/imgui_demo.cpp",
        "../dependencies/imgui/imgui_draw.cpp",
        "../dependencies/imgui/imgui_internal.h",
        "../dependencies/imgui/imgui_tables.cpp",
        "../dependencies/imgui/imgui_widgets.cpp",
        "../dependencies/imgui/imgui.cpp",
        "../dependencies/imgui/imgui.h",

        "../dependencies/imgui/backends/imgui_impl_win32.h",
        "../dependencies/imgui/backends/imgui_impl_win32.cpp",
        "../dependencies/imgui/backends/imgui_impl_dx12.h",
        "../dependencies/imgui/backends/imgui_impl_dx12.cpp",
     }
    files {
        "../dependencies/fmt/include/fmt/core.h" ,
        "../dependencies/fmt/include/fmt/format.h" ,
        "../dependencies/fmt/include/fmt/format-inl.h",
        "../dependencies/fmt/src/format.cc",
        "../dependencies/fmt/src/os.cc",
    }
    files {
        "../dependencies/strconv/strconv.h",
    }
    files {
        "../dependencies/stb/strconv.h",
    }

project "PolarBear"
    kind "StaticLib"

    files {
        "../PolarBear/**.h",
        "../PolarBear/**.hpp",
        "../PolarBear/**.inl",
        "../PolarBear/**.c",
        "../PolarBear/**.cpp",
        "../PolarBear/**.inl",
    }

    dependenciesIncDir()
    includedirs {
        "../PolarBear"
    }
    setLibDir()

    links { "Dependencies" }

    defaultTargetDir()

    configuration {"Debug"}
        defines { "DEBUG", "PB_DEBUG", "PB_PROFILE" }
        flags { "FullSymbols", "Symbols", "DebugRuntime" }

    configuration {"Development"}
        defines { "DEBUG", "PB_DEBUG", "PB_PROFILE" }
        flags { "Symbols", "Optimize", "DebugRuntime" }

    configuration {"Profile"}
        defines { "DEBUG", "PB_DEBUG", "PB_PROFILE" }
        flags { "Symbols", "Optimize" }

    configuration {"Release"}
        defines { "NDEBUG" }
        flags { "OptimizeSpeed", "No64BitChecks" }

    configuration{}

    pchheader( "Precompile.h" )
    pchsource( "../PolarBear/Precompile.cpp" )

project "app"
    kind "WindowedApp"

    defaultTargetDir()
    dependenciesIncDir()
    includedirs {
        "../PolarBear"
    }

    setLibDir()
    links {
        "PolarBear",
        "Dependencies"
    }

    configuration {"Debug or Development"}
        defines { "DEBUG", "PB_DEBUG", "PB_PROFILE" }
        flags { "FullSymbols", "Symbols", "WinMain" }

    configuration {"Profile"}
        defines { "DEBUG", "PB_DEBUG", "PB_PROFILE" }
        flags { "Symbols", "Optimize", "WinMain" }

    configuration {"Release"}
        defines { "NDEBUG" }
        flags { "OptimizeSpeed", "No64BitChecks", "WinMain" }

    configuration {}

    files {
        "../app/src/**.h",
        "../app/src/**.hpp",
        "../app/src/**.inl",
        "../app/src/**.c",
        "../app/src/**.cpp"
    }
