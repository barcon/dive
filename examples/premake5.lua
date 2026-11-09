-- premake5.lua
defines { "GLOBAL" }
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	architecture "x86_64"  
	
	location "build"	
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

	files { "src/routines.hpp", "src/routines.cpp" }

workspace "examples"
	configurations { "Release", "ReleaseCL"}
	
	filter {"configurations:Release" }   
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
		
		filter {"configurations:Release", "action:gmake*", "toolset:gcc" }	
			links { "dive", "gmsh-4.15", "material", "value", "interpolation", "kdtree", "node", "quadrature", "basis", "eilig", "logger", "utils"}

		filter {"configurations:Release", "action:vs*" }	
			links { "dive", "gmsh.dll.lib", "material", "value", "interpolation", "kdtree", "node", "quadrature", "basis", "eilig", "logger", "utils"}
	
	filter {"configurations:ReleaseCL"}   
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
		
	filter {"configurations:ReleaseCL", "action:gmake*", "toolset:gcc" }	
		links { "dive", "gmsh-4.15", "opencl", "material", "value", "interpolation", "kdtree", "node", "quadrature", "basis", "eilig", "club", "logger", "utils"}

	filter {"configurations:ReleaseCL", "action:vs*" }	
		links { "dive", "gmsh.dll.lib", "opencl", "material", "value", "interpolation", "kdtree", "node", "quadrature", "basis", "eilig", "club", "logger", "utils"}

project "mesh_deformation_rbf"
	targetname ("mesh_deformation_rbf")		
	files { "src/mesh_deformation_rbf.cpp" }	
	
project "mesh_deformation_strain"
	targetname ("mesh_deformation_strain")	
	files { "src/mesh_deformation_strain.cpp" }	

project "cavity_stokes_flow"
	targetname ("cavity_stokes_flow")	
	files { "src/cavity_stokes_flow.cpp" }	
	
project "bearing_shell_stokes"
	targetname ("bearing_shell_stokes")	
	files { "src/bearing_shell_stokes.cpp" }	
	
project "bearing_flat_hydrostatic"
	targetname ("bearing_flat_hydrostatic")	
	files { "src/bearing_flat_hydrostatic.cpp" }	