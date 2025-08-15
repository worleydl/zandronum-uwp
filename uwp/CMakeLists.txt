cmake_minimum_required(VERSION 3.14)

# Setup UWP platform
set(CMAKE_SYSTEM_NAME WindowsStore)
set(CMAKE_SYSTEM_VERSION 10.0.19041.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

# Define project
project(zandronum-uwp LANGUAGES CXX)
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${PROJECT_NAME})

# Config for main project path
set(PROJECT_DIR "" CACHE PATH "")
if(NOT PROJECT_DIR)
  message(FATAL_ERROR "PROJECT_DIR must be set to a valid path pointing to the main Zandronum cmake dir")
endif()

add_executable(${PROJECT_NAME} WIN32
	main.cpp
)

set(PkgAssets
	Assets/LockScreenLogo.scale-200.png
	Assets/Square44x44Logo.targetsize-24_altform-unplated.png
	Assets/Square44x44Logo.scale-200.png
	Assets/Square150x150Logo.scale-200.png
	Assets/StoreLogo.png
)

set(BinDeps
	${PROJECT_DIR}/${CMAKE_BUILD_TYPE}/zandronum.dll

	${PROJECT_DIR}/_deps/uwpdep-src/x64/bin/dxil.dll
	${PROJECT_DIR}/_deps/uwpdep-src/x64/bin/fmodex64.dll
	${PROJECT_DIR}/_deps/uwpdep-src/x64/bin/libcrypto-3-x64.dll
	${PROJECT_DIR}/_deps/uwpdep-src/x64/bin/libuwp.dll
	${PROJECT_DIR}/_deps/uwpdep-src/x64/bin/libgallium_wgl.dll
	${PROJECT_DIR}/_deps/uwpdep-src/x64/bin/opengl32.dll
	${PROJECT_DIR}/_deps/uwpdep-src/x64/bin/SDL2.dll
	${PROJECT_DIR}/_deps/uwpdep-src/x64/bin/z-1.dll

	${PROJECT_DIR}/${CMAKE_BUILD_TYPE}/skulltag_actors.pk3
	${PROJECT_DIR}/${CMAKE_BUILD_TYPE}/zandronum.pk3

	# Temporary
	"C:/Windows/System32/glu32.dll"
	"G:/dev/uwp/libs/winmm-wrapper/uwpmm.dll"
	"G:/dev/uwp/libs/fluidlite/fluidsynth64.dll"
	"E:/skulltag/doom.wad"


	Package.appxmanifest
)

set(BinLibs
	${PROJECT_DIR}/src/${CMAKE_BUILD_TYPE}/zandronum.lib

	${PROJECT_DIR}/_deps/uwpdep-src/x64/lib/libuwp.lib
	${PROJECT_DIR}/_deps/uwpdep-src/x64/lib/SDL2.lib

	OneCore
	WindowsApp
)
source_group("BinDeps" FILES ${BinDeps})

set_source_files_properties(${BinDeps} PROPERTIES
	VS_COPY_TO_OUT_DIR Always
	VS_DEPLOYMENT_CONTENT TRUE
	VS_DEPLOYMENT_LOCATION "."
)

# Override deployment location for agility core
set_source_files_properties(${AgilityBins} PROPERTIES 
	VS_DEPLOYMENT_LOCATION "D3D12"
)

source_group("PkgAssets" FILES ${PkgAssets})

set_source_files_properties(${PkgAssets} PROPERTIES
	#VS_COPY_TO_OUT_DIR Always
	VS_DEPLOYMENT_CONTENT TRUE
	VS_DEPLOYMENT_LOCATION "Assets"
)

target_sources(${PROJECT_NAME} PRIVATE
	${BinDeps}
	${PkgAssets}
)

target_include_directories(${PROJECT_NAME} PRIVATE
 	${PROJECT_DIR}/_deps/uwpdep-src/x64/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${BinLibs})
set_target_properties(${PROJECT_NAME} PROPERTIES VS_WINDOWS_TARGET_PLATFORM_MIN_VERSION 10.0.19041.0)
