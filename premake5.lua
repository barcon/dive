-- premake5.lua
workspace "dive"
	configurations { "Python"}
	location "build"	

project "dive"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	targetname ("_dive")
	targetextension (".pyd")
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
	includedirs { "../python/include" }	

	files { "src/**.hpp", "src/**.cpp" }

	filter "configurations:Python"
		kind "SharedLib"
		architecture "x86_64"
		defines { "NDEBUG"}
		optimize "Speed"
		
		removefiles{"src/dive_export_python_OpenCL.cpp"}
		
		links { "utils", "logger", "eilig", "basis", "quadrature", "nodes", "kdtree",
				"interpolation", "values", "material", "python313" }
		
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
		
workspace "diveCL"
	configurations { "PythonCL" }
	location "build"	

project "diveCL"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	targetname ("_diveCL")
	targetextension (".pyd")
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
	includedirs { "../python/include" }	

	files { "src/**.hpp", "src/**.cpp" }

	filter "configurations:PythonCL"
		kind "SharedLib"
		architecture "x86_64"
		defines { "NDEBUG", "EILIG_ENABLE_OPENCL" }
		optimize "Speed"
		
		removefiles{"src/dive_export_python.cpp"}
		
		includedirs { "../club/src" }		
		includedirs { "../opencl/inc" }		
		
		links { "utils", "logger", "eilig", "basis", "quadrature", "nodes", "kdtree",
				"interpolation", "values", "material", "club", "opencl", "python313" }		
		
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
		libdirs { "../python/lib" }