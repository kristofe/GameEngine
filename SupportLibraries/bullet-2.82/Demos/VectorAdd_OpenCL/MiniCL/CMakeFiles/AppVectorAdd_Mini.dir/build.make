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
include Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/depend.make

# Include the progress variables for this target.
include Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/progress.make

# Include the compile flags for this target's objects.
include Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/flags.make

Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o: Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/flags.make
Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o: Demos/VectorAdd_OpenCL/MiniCL_VectorAdd.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o -c /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL_VectorAdd.cpp

Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.i"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL_VectorAdd.cpp > CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.i

Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.s"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL_VectorAdd.cpp -o CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.s

Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o.requires:
.PHONY : Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o.requires

Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o.provides: Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o.requires
	$(MAKE) -f Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/build.make Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o.provides.build
.PHONY : Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o.provides

Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o.provides.build: Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o

# Object files for target AppVectorAdd_Mini
AppVectorAdd_Mini_OBJECTS = \
"CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o"

# External object files for target AppVectorAdd_Mini
AppVectorAdd_Mini_EXTERNAL_OBJECTS =

Demos/VectorAdd_OpenCL/MiniCL/AppVectorAdd_Mini: Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o
Demos/VectorAdd_OpenCL/MiniCL/AppVectorAdd_Mini: Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/build.make
Demos/VectorAdd_OpenCL/MiniCL/AppVectorAdd_Mini: src/MiniCL/libMiniCL.a
Demos/VectorAdd_OpenCL/MiniCL/AppVectorAdd_Mini: src/BulletMultiThreaded/libBulletMultiThreaded.a
Demos/VectorAdd_OpenCL/MiniCL/AppVectorAdd_Mini: src/LinearMath/libLinearMath.a
Demos/VectorAdd_OpenCL/MiniCL/AppVectorAdd_Mini: Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable AppVectorAdd_Mini"
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AppVectorAdd_Mini.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL && "/Applications/CMake 2.8-12.app/Contents/bin/cmake" -E copy_if_different /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/VectorAddKernels.cl /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL

# Rule to build all files generated by this target.
Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/build: Demos/VectorAdd_OpenCL/MiniCL/AppVectorAdd_Mini
.PHONY : Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/build

Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/requires: Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/__/MiniCL_VectorAdd.o.requires
.PHONY : Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/requires

Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/clean:
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL && $(CMAKE_COMMAND) -P CMakeFiles/AppVectorAdd_Mini.dir/cmake_clean.cmake
.PHONY : Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/clean

Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/depend:
	cd /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82 /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82 /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL /Users/kristofe/Documents/Projects/GameEngine/SupportLibraries/bullet-2.82/Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Demos/VectorAdd_OpenCL/MiniCL/CMakeFiles/AppVectorAdd_Mini.dir/depend

