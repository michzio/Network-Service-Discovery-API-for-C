# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Network_Service_Discovery_Lib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Network_Service_Discovery_Lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Network_Service_Discovery_Lib.dir/flags.make

CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o: CMakeFiles/Network_Service_Discovery_Lib.dir/flags.make
CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o: ../network_service_discovery.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o   -c /Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/network_service_discovery.c

CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/network_service_discovery.c > CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.i

CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/network_service_discovery.c -o CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.s

CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o.requires:

.PHONY : CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o.requires

CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o.provides: CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o.requires
	$(MAKE) -f CMakeFiles/Network_Service_Discovery_Lib.dir/build.make CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o.provides.build
.PHONY : CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o.provides

CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o.provides.build: CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o


# Object files for target Network_Service_Discovery_Lib
Network_Service_Discovery_Lib_OBJECTS = \
"CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o"

# External object files for target Network_Service_Discovery_Lib
Network_Service_Discovery_Lib_EXTERNAL_OBJECTS =

libnsd_shared.dylib: CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o
libnsd_shared.dylib: CMakeFiles/Network_Service_Discovery_Lib.dir/build.make
libnsd_shared.dylib: CMakeFiles/Network_Service_Discovery_Lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libnsd_shared.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Network_Service_Discovery_Lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Network_Service_Discovery_Lib.dir/build: libnsd_shared.dylib

.PHONY : CMakeFiles/Network_Service_Discovery_Lib.dir/build

CMakeFiles/Network_Service_Discovery_Lib.dir/requires: CMakeFiles/Network_Service_Discovery_Lib.dir/network_service_discovery.c.o.requires

.PHONY : CMakeFiles/Network_Service_Discovery_Lib.dir/requires

CMakeFiles/Network_Service_Discovery_Lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Network_Service_Discovery_Lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Network_Service_Discovery_Lib.dir/clean

CMakeFiles/Network_Service_Discovery_Lib.dir/depend:
	cd /Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd /Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd /Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug /Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug /Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug/CMakeFiles/Network_Service_Discovery_Lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Network_Service_Discovery_Lib.dir/depend

