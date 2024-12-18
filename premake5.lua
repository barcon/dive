-- premake5.lua
workspace "dive"
	configurations { "Python", "PythonCL" }
	location "build"

project "dive"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

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
	includedirs { "../python/inc" }	

	files { "src/**.hpp", "src/**.cpp" }

	filter "configurations:Python"
		kind "SharedLib"
		architecture "x86_64"
		defines { "NDEBUG"}
		optimize "Speed"
		
		links { "utils", "logger", "eilig", "basis", "quadrature", "nodes", "kdtree",
				"interpolation", "values", "material", "python312" }
		
		libdirs { "../utils/build/Release" }
		libdirs { "../logger/build/Release" }
		libdirs { "../eilig/build/Release" }
		libdirs { "../basis/build/Release" }
		libdirs { "../quadrature/build/Release" }
		libdirs { "../nodes/build/Release" }
		libdirs { "../kdtree/build/Release" }
		libdirs { "../interpolation/build/Release" }
		libdirs { "../values/build/Release" }
		libdirs { "../material/build/Release" }
		libdirs { "../python/lib" }
	
	filter "configurations:PythonCL"
		kind "SharedLib"
		architecture "x86_64"
		defines { "NDEBUG", "EILIG_ENABLE_OPENCL" }
		optimize "Speed"
		
		includedirs { "../club/src" }		
		includedirs { "../opencl/inc" }		
		
		links { "utils", "logger", "eilig", "basis", "quadrature", "nodes", "kdtree",
				"interpolation", "values", "material", "club", "opencl", "python312" }		
		
		libdirs { "../utils/build/Release" }
		libdirs { "../logger/build/Release" }
		libdirs { "../eilig/build/Release" }
		libdirs { "../basis/build/Release" }
		libdirs { "../quadrature/build/Release" }
		libdirs { "../nodes/build/Release" }
		libdirs { "../kdtree/build/Release" }
		libdirs { "../interpolation/build/Release" }
		libdirs { "../values/build/Release" }
		libdirs { "../material/build/Release" }
		libdirs { "../club/build/Release" }
		libdirs { "../opencl/lib/x86_64" }
		libdirs { "../python/lib" }