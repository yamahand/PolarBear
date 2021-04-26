-- premake5.lua
workspace "PolarBear"
   configurations { "Debug", "Development", "Profile", "Release" }
   platforms { "Win64" }
   location ("../.build/" .. _ACTION)

   filter { "platforms:Win64" }
   system "Windows"
   architecture "x86_64"

project "PolarBear"
   kind "StaticLib"
   language "C++"
   targetdir "../.bin/%{cfg.buildcfg}"
   cppdialect "C++latest"
   includedirs { 
      "../PolarBear/source",
      "../PolarBear/dependencies/imgui",
      "../PolarBear/dependencies/fmt/include",
      "../PolarBear/dependencies/strconv",
    }

   vpaths { 
      ["dependencies/imgui"] = "../PolarBear/dependencies/imgui/",
      ["dependencies/fmt"] = "../PolarBear/dependencies/fmt/**.*",
      ["dependencies/strconv"] = "../PolarBear/dependencies/strconv/",
      ["*"] = "../PolarBear/"
    }
   files {
      "../PolarBear/source/**.h",
      "../PolarBear/source/**.hpp",
      "../PolarBear/source/**.inl",
      "../PolarBear/source/**.c",
      "../PolarBear/source/**.cpp", 
      "../PolarBear/source/**.inl",
   }
   files {
      "../PolarBear/dependencies/imgui/imconfig.h",
      "../PolarBear/dependencies/imgui/imgui_demo.cpp",
      "../PolarBear/dependencies/imgui/imgui_draw.cpp",
      "../PolarBear/dependencies/imgui/imgui_internal.h",
      "../PolarBear/dependencies/imgui/imgui_tables.cpp",
      "../PolarBear/dependencies/imgui/imgui_widgets.cpp",
      "../PolarBear/dependencies/imgui/imgui.cpp",
      "../PolarBear/dependencies/imgui/imgui.h",

      "../PolarBear/dependencies/imgui/backends/imgui_impl_win32.h",
      "../PolarBear/dependencies/imgui/backends/imgui_impl_win32.cpp",
      "../PolarBear/dependencies/imgui/backends/imgui_impl_dx12.h",
      "../PolarBear/dependencies/imgui/backends/imgui_impl_dx12.cpp",
   }
   files {
      "../PolarBear/dependencies/fmt/include/fmt/core.h" ,
      "../PolarBear/dependencies/fmt/include/fmt/format.h" ,
      "../PolarBear/dependencies/fmt/include/fmt/format-inl.h", 
      "../PolarBear/dependencies/fmt/src/format.cc",
      "../PolarBear/dependencies/fmt/src/os.cc",
   }
   files {
      "../PolarBear/dependencies/strconv/strconv.h",
   }

   filter "configurations:Debug"
      defines { "DEBUG", "PB_DEBUG", "PB_PROFILE" }
      symbols "On"

   filter "configurations:Development or Profile"
      defines { "DEBUG", "PB_DEBUG", "PB_PROFILE" }
      symbols "On"
      optimize "On"
   
   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

   filter {}

project "App"
   kind "WindowedApp"

   language "C++"
   targetdir "../.bin/%{cfg.buildcfg}"
   cppdialect "C++latest"
   includedirs { 
      "../PolarBear/source",
      "../PolarBear/dependencies/imgui",
      "../PolarBear/dependencies/fmt/include",
      "../PolarBear/dependencies/strconv",
    }
   libdirs { "../.bin/%{cfg.buildcfg}" }
   links { "PolarBear" }

   vpaths { ["*"] = "../app/src" }
   files { "../app/src/**.h", "../app/src/**.hpp", "../app/src/**.inl", "../app/src/**.c", "../app/src/**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG", "PB_DEBUG", "PB_PROFILE" }
      symbols "On"

   filter "configurations:Development"
      defines { "DEBUG", "PB_DEBUG", "PB_PROFILE" }
      symbols "On"
   
   filter "configurations:Profile"
      defines { "DEBUG", "PB_DEBUG", "PB_PROFILE" }
      symbols "On"
      optimize "On"
   
   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

   filter {}
