# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = "/Applications/CMake 2.8-12.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-12.app/Contents/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-12.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82

# Include any dependencies generated for this target.
include Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/depend.make

# Include the progress variables for this target.
include Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/progress.make

# Include the compile flags for this target's objects.
include Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/flags.make

Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o: Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/flags.make
Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o: Demos/UserCollisionAlgorithm/UserCollisionAlgorithm.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/UserCollisionAlgorithm && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o -c /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/UserCollisionAlgorithm/UserCollisionAlgorithm.cpp

Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.i"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/UserCollisionAlgorithm && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/UserCollisionAlgorithm/UserCollisionAlgorithm.cpp > CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.i

Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.s"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/UserCollisionAlgorithm && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/UserCollisionAlgorithm/UserCollisionAlgorithm.cpp -o CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.s

Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o.requires:
.PHONY : Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o.requires

Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o.provides: Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o.requires
	$(MAKE) -f Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/build.make Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o.provides.build
.PHONY : Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o.provides

Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o.provides.build: Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o

# Object files for target AppUserCollisionAlgorithm
AppUserCollisionAlgorithm_OBJECTS = \
"CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o"

# External object files for target AppUserCollisionAlgorithm
AppUserCollisionAlgorithm_EXTERNAL_OBJECTS =

Demos/UserCollisionAlgorithm/AppUserCollisionAlgorithm: Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o
Demos/UserCollisionAlgorithm/AppUserCollisionAlgorithm: Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/build.make
Demos/UserCollisionAlgorithm/AppUserCollisionAlgorithm: Demos/OpenGL/libOpenGLSupport.a
Demos/UserCollisionAlgorithm/AppUserCollisionAlgorithm: src/BulletDynamics/libBulletDynamics.a
Demos/UserCollisionAlgorithm/AppUserCollisionAlgorithm: src/BulletCollision/libBulletCollision.a
Demos/UserCollisionAlgorithm/AppUserCollisionAlgorithm: src/LinearMath/libLinearMath.a
Demos/UserCollisionAlgorithm/AppUserCollisionAlgorithm: Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable AppUserCollisionAlgorithm"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/UserCollisionAlgorithm && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AppUserCollisionAlgorithm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/build: Demos/UserCollisionAlgorithm/AppUserCollisionAlgorithm
.PHONY : Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/build

Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/requires: Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/UserCollisionAlgorithm.o.requires
.PHONY : Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/requires

Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/clean:
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/UserCollisionAlgorithm && $(CMAKE_COMMAND) -P CMakeFiles/AppUserCollisionAlgorithm.dir/cmake_clean.cmake
.PHONY : Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/clean

Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/depend:
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82 /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/UserCollisionAlgorithm /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82 /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/UserCollisionAlgorithm /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Demos/UserCollisionAlgorithm/CMakeFiles/AppUserCollisionAlgorithm.dir/depend

