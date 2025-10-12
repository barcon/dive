-- premake5.lua
workspace "dive"
	configurations { "Release" }
	location "build"	

project "dive"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	targetname ("dive")
	targetextension (".lib")
	objdir "%{cfg.location}/obj/%{cfg.platform}_%{cfg.buildcfg}"	

	targetdir "build/%{cfg.buildcfg}"
	includedirs { "../utils/src" }
	includedirs { "../logger/src" }
	includedirs { "../eilig/src" }	
	includedirs { "../basis/src" }
	includedirs { "../quadrature/src" }
	includedirs { "../nodes/src" }
	includedirs { "../kdtree/src" }
	includedirs { "../interpolation/src" }
	includedirs { "../values/src" }
	includedirs { "../material/src" }
	includedirs { "../thread-pool/include" }
	includedirs { "../gmsh/include" }	

	files { "src/**.hpp", "src/**.cpp" }

	filter "configurations:Release"
		kind "SharedLib"
		architecture "x86_64"
		defines { "NDEBUG", "EILIG_ENABLE_OPENCL" }
		optimize "Speed"
		
		includedirs { "../club/src" }		
		includedirs { "../opencl/inc" }		
		
		links { "utils.lib", "logger.lib", "eilig.lib", "basis.lib", "quadrature.lib", "nodes.lib", "kdtree.lib",
				"interpolation.lib", "values.lib", "material.lib", "club.lib", "opencl.lib", "gmsh.dll.lib" }
		
		libdirs { "../utils/build/Release" }
		libdirs { "../logger/build/Release" }
		libdirs { "../eilig/build/ReleaseCL" }
		libdirs { "../basis/build/Release" }
		libdirs { "../quadrature/build/Release" }
		libdirs { "../nodes/build/Release" }
		libdirs { "../kdtree/build/Release" }
		libdirs { "../interpolation/build/Release" }
		libdirs { "../values/build/Release" }
		libdirs { "../material/build/Release" }
		libdirs { "../club/build/Release" }
		libdirs { "../opencl/lib/x86_64" }
		libdirs { "../gmsh/lib" }