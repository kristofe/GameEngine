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
include Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/depend.make

# Include the progress variables for this target.
include Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/progress.make

# Include the compile flags for this target's objects.
include Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/flags.make

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/flags.make
Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o: Demos/SerializeDemo/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/AppSerializeDemo.dir/main.o -c /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo/main.cpp

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppSerializeDemo.dir/main.i"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo/main.cpp > CMakeFiles/AppSerializeDemo.dir/main.i

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppSerializeDemo.dir/main.s"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo/main.cpp -o CMakeFiles/AppSerializeDemo.dir/main.s

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o.requires:
.PHONY : Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o.requires

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o.provides: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o.requires
	$(MAKE) -f Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/build.make Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o.provides.build
.PHONY : Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o.provides

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o.provides.build: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/flags.make
Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o: Demos/SerializeDemo/SerializeDemo.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o -c /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo/SerializeDemo.cpp

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppSerializeDemo.dir/SerializeDemo.i"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo/SerializeDemo.cpp > CMakeFiles/AppSerializeDemo.dir/SerializeDemo.i

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppSerializeDemo.dir/SerializeDemo.s"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo/SerializeDemo.cpp -o CMakeFiles/AppSerializeDemo.dir/SerializeDemo.s

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o.requires:
.PHONY : Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o.requires

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o.provides: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o.requires
	$(MAKE) -f Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/build.make Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o.provides.build
.PHONY : Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o.provides

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o.provides.build: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o

# Object files for target AppSerializeDemo
AppSerializeDemo_OBJECTS = \
"CMakeFiles/AppSerializeDemo.dir/main.o" \
"CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o"

# External object files for target AppSerializeDemo
AppSerializeDemo_EXTERNAL_OBJECTS =

Demos/SerializeDemo/AppSerializeDemo: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o
Demos/SerializeDemo/AppSerializeDemo: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o
Demos/SerializeDemo/AppSerializeDemo: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/build.make
Demos/SerializeDemo/AppSerializeDemo: Demos/OpenGL/libOpenGLSupport.a
Demos/SerializeDemo/AppSerializeDemo: Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.a
Demos/SerializeDemo/AppSerializeDemo: src/BulletSoftBody/libBulletSoftBody.a
Demos/SerializeDemo/AppSerializeDemo: src/BulletDynamics/libBulletDynamics.a
Demos/SerializeDemo/AppSerializeDemo: src/BulletCollision/libBulletCollision.a
Demos/SerializeDemo/AppSerializeDemo: Extras/Serialize/BulletFileLoader/libBulletFileLoader.a
Demos/SerializeDemo/AppSerializeDemo: src/LinearMath/libLinearMath.a
Demos/SerializeDemo/AppSerializeDemo: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable AppSerializeDemo"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AppSerializeDemo.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo && "/Applications/CMake 2.8-12.app/Contents/bin/cmake" -E copy_if_different /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo/testFile.bullet /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo/testFile.bullet

# Rule to build all files generated by this target.
Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/build: Demos/SerializeDemo/AppSerializeDemo
.PHONY : Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/build

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/requires: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/main.o.requires
Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/requires: Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/SerializeDemo.o.requires
.PHONY : Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/requires

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/clean:
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo && $(CMAKE_COMMAND) -P CMakeFiles/AppSerializeDemo.dir/cmake_clean.cmake
.PHONY : Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/clean

Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/depend:
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82 /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82 /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Demos/SerializeDemo/CMakeFiles/AppSerializeDemo.dir/depend

