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
   includedirs { "../PolarBear/include" }

   vpaths { ["*"] = "../PolarBear/" }
   files { "../PolarBear/include/**.h", "../PolarBear/include/**.hpp", "../PolarBear/include/**.inl", "../PolarBear/src/**.c", "../PolarBear/src/**.cpp", "../PolarBear/src/**.inl" }

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
   kind "ConsoleApp"

   language "C++"
   targetdir "../.bin/%{cfg.buildcfg}"
   cppdialect "C++latest"
   includedirs { "../PolarBear/include" }
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