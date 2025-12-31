-- premake5.lua
workspace "dive"
	configurations { "Debug", "Release"}
	location "build"	

project "dive"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	objdir "%{cfg.location}/obj/%{cfg.platform}_%{cfg.buildcfg}"	

	targetdir "build/%{cfg.buildcfg}"
	includedirs { "../utils/src" }	
	includedirs { "../logger/src" }	
	includedirs { "../eilig/src" }	
	includedirs { "../basis/src" }	
	includedirs { "../quadrature/src" }	
	includedirs { "../node/src" }	
	includedirs { "../kdtree/src" }	
	includedirs { "../interpolation/src" }	
	includedirs { "../values/src" }	
	includedirs { "../material/src" }
	includedirs { "../lua/include" }
	includedirs { "../sol2/include" }	
	includedirs { "../thread-pool/include" }	
	includedirs { "../gmsh/include" }	

	files { "src/**.hpp", "src/**.cpp" }

	filter "configurations:Debug"
		architecture "x86_64"     
		defines { "DEBUG" }
		symbols "On"
			
	filter "configurations:Release"
		architecture "x86_64"     
		defines { "NDEBUG" }
		optimize "Speed"