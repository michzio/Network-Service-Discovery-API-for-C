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
CMAKE_COMMAND = /cygdrive/c/Users/michzio/.CLion2017.1/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/michzio/.CLion2017.1/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Network_Service_Discovery.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Network_Service_Discovery.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Network_Service_Discovery.dir/flags.make

CMakeFiles/Network_Service_Discovery.dir/main.c.o: CMakeFiles/Network_Service_Discovery.dir/flags.make
CMakeFiles/Network_Service_Discovery.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Network_Service_Discovery.dir/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Network_Service_Discovery.dir/main.c.o   -c /cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/main.c

CMakeFiles/Network_Service_Discovery.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Network_Service_Discovery.dir/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/main.c > CMakeFiles/Network_Service_Discovery.dir/main.c.i

CMakeFiles/Network_Service_Discovery.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Network_Service_Discovery.dir/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/main.c -o CMakeFiles/Network_Service_Discovery.dir/main.c.s

CMakeFiles/Network_Service_Discovery.dir/main.c.o.requires:

.PHONY : CMakeFiles/Network_Service_Discovery.dir/main.c.o.requires

CMakeFiles/Network_Service_Discovery.dir/main.c.o.provides: CMakeFiles/Network_Service_Discovery.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/Network_Service_Discovery.dir/build.make CMakeFiles/Network_Service_Discovery.dir/main.c.o.provides.build
.PHONY : CMakeFiles/Network_Service_Discovery.dir/main.c.o.provides

CMakeFiles/Network_Service_Discovery.dir/main.c.o.provides.build: CMakeFiles/Network_Service_Discovery.dir/main.c.o


# Object files for target Network_Service_Discovery
Network_Service_Discovery_OBJECTS = \
"CMakeFiles/Network_Service_Discovery.dir/main.c.o"

# External object files for target Network_Service_Discovery
Network_Service_Discovery_EXTERNAL_OBJECTS =

nsd.exe: CMakeFiles/Network_Service_Discovery.dir/main.c.o
nsd.exe: CMakeFiles/Network_Service_Discovery.dir/build.make
nsd.exe: libnsd.a
nsd.exe: /usr/lib/libdns_sd.dll.a
nsd.exe: CMakeFiles/Network_Service_Discovery.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable nsd.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Network_Service_Discovery.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Network_Service_Discovery.dir/build: nsd.exe

.PHONY : CMakeFiles/Network_Service_Discovery.dir/build

CMakeFiles/Network_Service_Discovery.dir/requires: CMakeFiles/Network_Service_Discovery.dir/main.c.o.requires

.PHONY : CMakeFiles/Network_Service_Discovery.dir/requires

CMakeFiles/Network_Service_Discovery.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Network_Service_Discovery.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Network_Service_Discovery.dir/clean

CMakeFiles/Network_Service_Discovery.dir/depend:
	cd /cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd /cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd /cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug /cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug /cygdrive/c/Users/michzio/Developer/MyProjects/BachelorDegree/c/nsd/cmake-build-debug/CMakeFiles/Network_Service_Discovery.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Network_Service_Discovery.dir/depend

