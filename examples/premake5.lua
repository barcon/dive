-- premake5.lua
workspace "dive"
	configurations { "Release", "ReleaseCL"}
	location "build"	

project "mesh_deformation_rbf"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	targetname ("mesh_deformation_rbf")	
	architecture "x86_64"  
	objdir "%{cfg.location}/obj/%{cfg.platform}_%{cfg.buildcfg}"	

	targetdir "build/%{cfg.buildcfg}"
	includedirs { "../../utils/src" }	
	includedirs { "../../logger/src" }	
	includedirs { "../../eilig/src" }	
	includedirs { "../../basis/src" }	
	includedirs { "../../quadrature/src" }	
	includedirs { "../../node/src" }	
	includedirs { "../../kdtree/src" }	
	includedirs { "../../interpolation/src" }	
	includedirs { "../../value/src" }	
	includedirs { "../../material/src" }
	includedirs { "../../dive/src" }
	includedirs { "../../thread-pool/include" }	
	includedirs { "../../gmsh/include" }
	includedirs { "../../json/include" }								

	files { "src/**.hpp", "src/**.cpp" }
		
	filter {"configurations:Release", "action:gmake*", "toolset:gcc" }   
		defines { "NDEBUG" }
		optimize "Speed"
		
		libdirs { "../../utils/build/Release" }
		libdirs { "../../logger/build/Release" }
		libdirs { "../../eilig/build/Release" }
		libdirs { "../../basis/build/Release" }
		libdirs { "../../quadrature/build/Release" }
		libdirs { "../../node/build/Release" }
		libdirs { "../../kdtree/build/Release" }
		libdirs { "../../interpolation/build/Release" }
		libdirs { "../../value/build/Release" }
		libdirs { "../../material/build/Release" }
		libdirs { "../../dive/build/Release" }
		libdirs { "../../gmsh/lib" }		
		
		links { "dive", "gmsh", "material", "value", "interpolation", "kdtree", "node", "quadrature", "basis", "eilig", "logger", "utils"}

	filter {"configurations:Release", "action:vs*" }   
		defines { "NDEBUG" }
		optimize "Speed"
		
		libdirs { "../../utils/build/Release" }
		libdirs { "../../logger/build/Release" }
		libdirs { "../../eilig/build/Release" }
		libdirs { "../../basis/build/Release" }
		libdirs { "../../quadrature/build/Release" }
		libdirs { "../../node/build/Release" }
		libdirs { "../../kdtree/build/Release" }
		libdirs { "../../interpolation/build/Release" }
		libdirs { "../../value/build/Release" }
		libdirs { "../../material/build/Release" }
		libdirs { "../../dive/build/Release" }
		libdirs { "../../gmsh/lib" }		
		
		links { "dive", "gmsh.dll.lib", "material", "value", "interpolation", "kdtree", "node", "quadrature", "basis", "eilig", "logger", "utils"}
		
	filter {"configurations:ReleaseCL", "action:gmake*", "toolset:gcc" }   
		defines { "NDEBUG", "EILIG_ENABLE_OPENCL" }
		optimize "Speed"
		
		includedirs { "../../club/src" }	
		includedirs { "../../opencl/inc" }	
		
		libdirs { "../../utils/build/Release" }
		libdirs { "../../logger/build/Release" }
		libdirs { "../../club/build/ReleaseCL" }
		libdirs { "../../eilig/build/ReleaseCL" }
		libdirs { "../../basis/build/ReleaseCL" }
		libdirs { "../../quadrature/build/ReleaseCL" }
		libdirs { "../../node/build/ReleaseCL" }
		libdirs { "../../kdtree/build/ReleaseCL" }
		libdirs { "../../interpolation/build/ReleaseCL" }
		libdirs { "../../value/build/ReleaseCL" }
		libdirs { "../../material/build/ReleaseCL" }
		libdirs { "../../dive/build/ReleaseCL" }
		libdirs { "../../opencl/lib/x86_64" }			
		libdirs { "../../gmsh/lib" }		
		
		links { "dive", "gmsh", "opencl", "material", "value", "interpolation", "kdtree", "node", "quadrature", "basis", "eilig", "club", "logger", "utils"}

	filter {"configurations:ReleaseCL", "action:vs*" }   
		defines { "NDEBUG", "EILIG_ENABLE_OPENCL" }
		optimize "Speed"
		
		includedirs { "../../club/src" }	
		includedirs { "../../opencl/inc" }	
		
		libdirs { "../../utils/build/Release" }
		libdirs { "../../logger/build/Release" }
		libdirs { "../../club/build/ReleaseCL" }
		libdirs { "../../eilig/build/ReleaseCL" }
		libdirs { "../../basis/build/ReleaseCL" }
		libdirs { "../../quadrature/build/ReleaseCL" }
		libdirs { "../../node/build/ReleaseCL" }
		libdirs { "../../kdtree/build/ReleaseCL" }
		libdirs { "../../interpolation/build/ReleaseCL" }
		libdirs { "../../value/build/ReleaseCL" }
		libdirs { "../../material/build/ReleaseCL" }
		libdirs { "../../dive/build/ReleaseCL" }
		libdirs { "../../opencl/lib/x86_64" }	
		libdirs { "../../gmsh/lib" }		
		
		links { "dive", "gmsh.dll.lib", "opencl", "material", "value", "interpolation", "kdtree", "node", "quadrature", "basis", "eilig", "club", "logger", "utils"}
