cmake_minimum_required(VERSION 3.15)

#! Check every comment after the "#!"

#! CHANGE YOUR PROJECT NAME
#  It is used as your project's main executable name. 
set(PROJECT_NAME mycat)
project(${PROJECT_NAME} C CXX) # project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 20)

##########################################################
# User configurable options of the template
##########################################################
# Note: symbols like WARNINGS_AS_ERRORS in configuration are intentionally variables
# and not CMake options --using options creates too many problems for students.

#! It is a good practice to set "WARNINGS_AS_ERRORS" ON,
#  but sometimes it creates too much trouble, so default is OFF.
set(WARNINGS_AS_ERRORS OFF)

#! Always use PVS Studio while developing.
set(ENABLE_PVS_STUDIO OFF)

#! Select appropriate sanitizers.
#  Definitely enable sanitizers while developing.
#  Disable it for the production builds and before submitting for grading.
#  Only one of the Memory (MSAN), Address (ASAN), or Thread (TSan)
#  sanitizers is applicable at the time -- the first defined.

#! UndefinedBehaviorSanitizer (UBSan).
#  Info: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
set(ENABLE_UBSan OFF)
#! AddressSanitizer -- detects use after free or after scope exit,
#  memory overflows and leaks.
#  Info: https://github.com/google/sanitizers/wiki/AddressSanitizer
set(ENABLE_ASAN OFF)
#! ThreadSanitizer -- detects data races.
#  Info: https://clang.llvm.org/docs/ThreadSanitizer.html
set(ENABLE_TSan OFF)
#! MemorySanitizer -- detects uninitialized memory reads
#  Info: https://github.com/google/sanitizers/wiki/MemorySanitizer
set(ENABLE_MSAN OFF)

#! Be default -- build release version if not specified otherwise.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()

# Warnings as errors should be imported here -- do not move this line
include(cmake/CompilerWarnings.cmake)

##########################################################
# Project files, packages, libraries and so on
##########################################################
#set(CMAKE_SYSTEM )
set(PROJECT_DEPENDENCIES
		options_parser/options_parser.cpp options_parser/options_parser.h
		file/Ifile.h
		file/stdfile_factory.h)

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
	set(SYSTEM_SPECIFIC_PROJECT_DEPENDENCIES
			file/file_win.cpp file/file_win.h
			file/stdfile_factory_win.h)
	set(CMAKE_VERBOSE_MAKEFILE ON)
#	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} /DWIN32) # if built with ninja
#	set(CMAKE_TOOLCHAIN_FILE "C:/SDKs/vcpkg/scripts/buildsystems/vcpkg.cmake")
else()
	set(SYSTEM_SPECIFIC_PROJECT_DEPENDENCIES
			file/stdfile_factory_posix.h
			file/file_posix.cpp file/file_posix.h)
#	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -DPOSIX)
	# since there are so many posix systems most likely, it is POSIX
#	target_compile_definitions()
	#does not work for nonposix
	#it should give an error for nonposix
endif ()

add_executable(${PROJECT_NAME} main.cpp ${PROJECT_DEPENDENCIES} ${SYSTEM_SPECIFIC_PROJECT_DEPENDENCIES})

#! Project main executable source compilation


#! Put path to your project headers
target_include_directories(${PROJECT_NAME} PRIVATE options_parser)
target_include_directories(${PROJECT_NAME} PRIVATE file)


#! Add external packages
# options_parser requires boost::program_options library
#message("${CMAKE_VERSION}")

#message("${BOOST_ROOT}")
#upda
#set(Boost_INCLUDE_DIR ${_VCPKG_INSTALLED_DIR}/x64-windows/include)

find_package(Boost 1.71.0 COMPONENTS program_options system REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::system)

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
	target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32) #seem better than adding flag -DFLAG
else()
	target_compile_definitions(${PROJECT_NAME} PRIVATE POSIX)
endif ()

#-DCMAKE_TOOLCHAIN_FILE=C:\SDKs\vcpkg\scripts\buildsystems\vcpkg.cmake



#-DBOOST_INCLUDEDIR="C:\SDKs\boost_1_82_0"
#-DBOOST_LIBRARYDIR="C:\SDKs\boost_1_82_0\stage\lib"
#-DBoost_DEBUG=ON
##########################################################
# Fixed CMakeLists.txt part
##########################################################

INSTALL(PROGRAMS
		$<TARGET_FILE:${PROJECT_NAME}> # ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
		DESTINATION bin)

# Define ALL_TARGETS variable to use in PVS and Sanitizers
set(ALL_TARGETS ${PROJECT_NAME})

# Include CMake setup
include(cmake/main-config.cmake)