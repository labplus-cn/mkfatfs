# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.11

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "E:\3 micropython\2 micropython-esp32\mkfatfs"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "E:\3 micropython\2 micropython-esp32\mkfatfs\build"

# Utility rule file for ContinuousMemCheck.

# Include the progress variables for this target.
include CMakeFiles/ContinuousMemCheck.dir/progress.make

CMakeFiles/ContinuousMemCheck:
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build" && "C:\Program Files\CMake\bin\ctest.exe" -D ContinuousMemCheck

ContinuousMemCheck: CMakeFiles/ContinuousMemCheck
ContinuousMemCheck: CMakeFiles/ContinuousMemCheck.dir/build.make

.PHONY : ContinuousMemCheck

# Rule to build all files generated by this target.
CMakeFiles/ContinuousMemCheck.dir/build: ContinuousMemCheck

.PHONY : CMakeFiles/ContinuousMemCheck.dir/build

CMakeFiles/ContinuousMemCheck.dir/clean:
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build" && $(CMAKE_COMMAND) -P CMakeFiles\ContinuousMemCheck.dir\cmake_clean.cmake
.PHONY : CMakeFiles/ContinuousMemCheck.dir/clean

CMakeFiles/ContinuousMemCheck.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "E:\3 micropython\2 micropython-esp32\mkfatfs" "E:\3 micropython\2 micropython-esp32\mkfatfs" "E:\3 micropython\2 micropython-esp32\mkfatfs\build" "E:\3 micropython\2 micropython-esp32\mkfatfs\build" "E:\3 micropython\2 micropython-esp32\mkfatfs\build\CMakeFiles\ContinuousMemCheck.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/ContinuousMemCheck.dir/depend

