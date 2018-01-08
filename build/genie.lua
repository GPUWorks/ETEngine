solution "ETEngine"
    configurations {
        "Debug",
        "DebugEditor",
        "Development",
        "DevelopmentEditor",
		"Shipping"
    }

    platforms {
        "x64",
        "x32"
    }

    language "C++"

    location "../build/"
    objdir "../build/"
	
PROJECT_DIR = "../"
SOURCE_DIR = path.join(PROJECT_DIR, "source/")
DEP_DIR = path.join(PROJECT_DIR, "dependancies/")
DEP_INCLUDE = path.join(DEP_DIR, "include/")

--setting output directories
--intermediate files go in		build/Intermediate/config_platform/project
--binaries go in				bin/config/project/platform
function outputDirectories(_project)
	local cfgs = configurations()
	local p = platforms()
	for i = 1, #cfgs do
		for j = 1, #p do
			configuration { cfgs[i], p[j] }
				targetdir("../bin/" .. cfgs[i] .. "_" .. p[j] .. "/" .. _project)
				objdir("../build/Intermediate/" .. cfgs[i]  .. "/" .. _project)		--seems like the platformm will automatically be added
		end
	end
	configuration {}
end

--platform specific library paths
function platformLibraries()
	local p = platforms()
	for j = 1, #p do
		local depPf = path.join(DEP_DIR, p[j] .. "/") 

		configuration { "vs*", p[j] }
			libdirs { path.join(depPf, "sdl2"),path.join(depPf, "freeImage"), path.join(depPf, "assimp") }
	end
	configuration {}
end

--copy files that are specific for the platform being built for
function windowsPlatformPostBuild()
	local p = platforms()
	for j = 1, #p do
		local copyCmd = "$(SolutionDir)..\\build\\copyResources_windows.bat " .. path.getabsolute(SOURCE_DIR) .. " $(OutDir) " .. p[j] .. " true"

		configuration { "vs*", p[j] }
			postbuildcommands { copyCmd } --copy dlls and resources after build
	end
	configuration {}
end

configuration "Debug"
	defines { "_DEBUG" }
	flags { "Symbols", "ExtraWarnings" }
configuration "DebugEditor"
	defines { "_DEBUG", "EDITOR" }
	flags { "Symbols", "ExtraWarnings" }
configuration "Development"
	flags {"OptimizeSpeed", "Symbols", "ExtraWarnings" }
configuration "DevelopmentEditor"
	defines { "EDITOR" }
	flags {"OptimizeSpeed", "Symbols", "ExtraWarnings" }
configuration "Shipping"
	defines { "SHIPPING" }
	flags {"OptimizeSpeed", "No64BitChecks" }

configuration "vs*"
	flags { "NoIncrementalLink" }
	defines { "WIN32", "PLATFORM_Win" }
	includedirs { path.join(DEP_INCLUDE, "sdl2"),path.join(DEP_INCLUDE, "freeImage"), path.join(DEP_INCLUDE, "assimp") }
	debugdir "$(OutDir)"
configuration { "vs*", "x32" }
	flags { "EnableSSE2" }
configuration { "linux", "gmake"}
	defines { "PLATFORM_Linux", "__linux__" }
	includedirs { "/usr/include" }
	buildoptions_cpp { "-std=c++14" }
configuration {}

startproject "Demo"

--project that has the build files, easy way to make sure we can see the build files in the IDE, shouldn't actually be built
project "General"
	kind "StaticLib" --required
	location "."
	--specific files to avoid showing vs projects and solutions in build folder
	files { 
		path.join(PROJECT_DIR, "build/*.bat"), 
		path.join(PROJECT_DIR, "build/*.lua"), 
		path.join(PROJECT_DIR, "*.*"), 
		path.join(PROJECT_DIR, ".gitignore"), 
		path.join(PROJECT_DIR, ".gitattributes") 
	}

--in future this should be generated for any application running on the engine
project "Demo"
	kind "ConsoleApp"

	location "../source/Demo"

    defines { "_CONSOLE" }

	--Set output folders
	outputDirectories("Demo")

	configuration "vs*"
		flags { "Winmain"}

		links { "opengl32", "SDL2main" } 

	platformLibraries()
	windowsPlatformPostBuild()

	--Linked libraries
    links{ "ETEngine", "SDL2", "FreeImage", "assimp" }

	--additional includedirs
	local ProjBase = path.join(SOURCE_DIR, "Demo") 
	includedirs { path.join(ProjBase, "../Demo"), path.join(ProjBase, "Materials"), path.join(ProjBase, "Scenes") }

	--Source files
    files { 
		path.join(SOURCE_DIR, "Demo/**.cpp"), 
		path.join(SOURCE_DIR, "Demo/**.hpp"), 
		path.join(SOURCE_DIR, "Demo/**.h"), 
		path.join(SOURCE_DIR, "Demo/**.glsl"), 
	}

project "ETEngine"
    kind "StaticLib"

	location "../source/Engine"
	
	outputDirectories("Engine")

    files { 
		path.join(SOURCE_DIR, "Engine/**.cpp"), 
		path.join(SOURCE_DIR, "Engine/**.hpp"), 
		path.join(SOURCE_DIR, "Engine/**.h"), 
		path.join(SOURCE_DIR, "Engine/**.glsl"), 

		path.join(SOURCE_DIR, "Engine/**.c"),									--for glad
	}
	nopch { path.join(SOURCE_DIR, "Engine/StaticDependancies/glad/glad.c") }	--c code shouldn't use precompiled headers

	--additional includedirs
	local ProjBase = path.join(SOURCE_DIR, "Engine") 
	includedirs { path.join(ProjBase, "Base"), path.join(ProjBase, "Components"), path.join(ProjBase, "Content"), path.join(ProjBase, "Helper"), 
		path.join(ProjBase, "Graphics"), path.join(ProjBase, "Prefabs"), path.join(ProjBase, "SceneGraph"), path.join(ProjBase, "Framebuffers"), 
		path.join(ProjBase, "GraphicsHelper"), path.join(ProjBase, "Materials"), path.join(ProjBase, "Math"), path.join(ProjBase, "PlanetTech"), 
		path.join(ProjBase, "PlanetTech/Types"), path.join(ProjBase, "StaticDependancies/glad"), path.join(ProjBase, "../Engine")}

	pchheader "stdafx.hpp"
	pchsource "../source/Engine/stdafx.cpp"

project "Testing"
	kind "ConsoleApp"

	location "../source/Testing"

    defines { "_CONSOLE" }

	outputDirectories("Testing")

	configuration "vs*"
		flags { "Winmain"}
		debugdir "$(SolutionDir)"
		includedirs { path.join(DEP_INCLUDE, "catch") }
		links { "opengl32", "SDL2main" } 

	platformLibraries()

    files { path.join(SOURCE_DIR, "Testing/**.cpp") }

    links{ "ETEngine", "SDL2", "FreeImage", "assimp" }