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

# Include any dependencies generated for this target.
include filesystem/fatfs/CMakeFiles/fatfs.dir/depend.make

# Include the progress variables for this target.
include filesystem/fatfs/CMakeFiles/fatfs.dir/progress.make

# Include the compile flags for this target's objects.
include filesystem/fatfs/CMakeFiles/fatfs.dir/flags.make

filesystem/fatfs/CMakeFiles/fatfs.dir/ccsbcs.c.obj: filesystem/fatfs/CMakeFiles/fatfs.dir/flags.make
filesystem/fatfs/CMakeFiles/fatfs.dir/ccsbcs.c.obj: filesystem/fatfs/CMakeFiles/fatfs.dir/includes_C.rsp
filesystem/fatfs/CMakeFiles/fatfs.dir/ccsbcs.c.obj: ../filesystem/fatfs/ccsbcs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="E:\3 micropython\2 micropython-esp32\mkfatfs\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object filesystem/fatfs/CMakeFiles/fatfs.dir/ccsbcs.c.obj"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\fatfs.dir\ccsbcs.c.obj   -c "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\ccsbcs.c"

filesystem/fatfs/CMakeFiles/fatfs.dir/ccsbcs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fatfs.dir/ccsbcs.c.i"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\ccsbcs.c" > CMakeFiles\fatfs.dir\ccsbcs.c.i

filesystem/fatfs/CMakeFiles/fatfs.dir/ccsbcs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fatfs.dir/ccsbcs.c.s"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\ccsbcs.c" -o CMakeFiles\fatfs.dir\ccsbcs.c.s

filesystem/fatfs/CMakeFiles/fatfs.dir/esp_vfs_fat.c.obj: filesystem/fatfs/CMakeFiles/fatfs.dir/flags.make
filesystem/fatfs/CMakeFiles/fatfs.dir/esp_vfs_fat.c.obj: filesystem/fatfs/CMakeFiles/fatfs.dir/includes_C.rsp
filesystem/fatfs/CMakeFiles/fatfs.dir/esp_vfs_fat.c.obj: ../filesystem/fatfs/esp_vfs_fat.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="E:\3 micropython\2 micropython-esp32\mkfatfs\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object filesystem/fatfs/CMakeFiles/fatfs.dir/esp_vfs_fat.c.obj"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\fatfs.dir\esp_vfs_fat.c.obj   -c "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\esp_vfs_fat.c"

filesystem/fatfs/CMakeFiles/fatfs.dir/esp_vfs_fat.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fatfs.dir/esp_vfs_fat.c.i"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\esp_vfs_fat.c" > CMakeFiles\fatfs.dir\esp_vfs_fat.c.i

filesystem/fatfs/CMakeFiles/fatfs.dir/esp_vfs_fat.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fatfs.dir/esp_vfs_fat.c.s"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\esp_vfs_fat.c" -o CMakeFiles\fatfs.dir\esp_vfs_fat.c.s

filesystem/fatfs/CMakeFiles/fatfs.dir/ff.c.obj: filesystem/fatfs/CMakeFiles/fatfs.dir/flags.make
filesystem/fatfs/CMakeFiles/fatfs.dir/ff.c.obj: filesystem/fatfs/CMakeFiles/fatfs.dir/includes_C.rsp
filesystem/fatfs/CMakeFiles/fatfs.dir/ff.c.obj: ../filesystem/fatfs/ff.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="E:\3 micropython\2 micropython-esp32\mkfatfs\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object filesystem/fatfs/CMakeFiles/fatfs.dir/ff.c.obj"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\fatfs.dir\ff.c.obj   -c "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\ff.c"

filesystem/fatfs/CMakeFiles/fatfs.dir/ff.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fatfs.dir/ff.c.i"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\ff.c" > CMakeFiles\fatfs.dir\ff.c.i

filesystem/fatfs/CMakeFiles/fatfs.dir/ff.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fatfs.dir/ff.c.s"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\ff.c" -o CMakeFiles\fatfs.dir\ff.c.s

filesystem/fatfs/CMakeFiles/fatfs.dir/syscall.c.obj: filesystem/fatfs/CMakeFiles/fatfs.dir/flags.make
filesystem/fatfs/CMakeFiles/fatfs.dir/syscall.c.obj: filesystem/fatfs/CMakeFiles/fatfs.dir/includes_C.rsp
filesystem/fatfs/CMakeFiles/fatfs.dir/syscall.c.obj: ../filesystem/fatfs/syscall.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="E:\3 micropython\2 micropython-esp32\mkfatfs\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building C object filesystem/fatfs/CMakeFiles/fatfs.dir/syscall.c.obj"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\fatfs.dir\syscall.c.obj   -c "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\syscall.c"

filesystem/fatfs/CMakeFiles/fatfs.dir/syscall.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fatfs.dir/syscall.c.i"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\syscall.c" > CMakeFiles\fatfs.dir\syscall.c.i

filesystem/fatfs/CMakeFiles/fatfs.dir/syscall.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fatfs.dir/syscall.c.s"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && C:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs\syscall.c" -o CMakeFiles\fatfs.dir\syscall.c.s

# Object files for target fatfs
fatfs_OBJECTS = \
"CMakeFiles/fatfs.dir/ccsbcs.c.obj" \
"CMakeFiles/fatfs.dir/esp_vfs_fat.c.obj" \
"CMakeFiles/fatfs.dir/ff.c.obj" \
"CMakeFiles/fatfs.dir/syscall.c.obj"

# External object files for target fatfs
fatfs_EXTERNAL_OBJECTS =

filesystem/fatfs/libfatfs.a: filesystem/fatfs/CMakeFiles/fatfs.dir/ccsbcs.c.obj
filesystem/fatfs/libfatfs.a: filesystem/fatfs/CMakeFiles/fatfs.dir/esp_vfs_fat.c.obj
filesystem/fatfs/libfatfs.a: filesystem/fatfs/CMakeFiles/fatfs.dir/ff.c.obj
filesystem/fatfs/libfatfs.a: filesystem/fatfs/CMakeFiles/fatfs.dir/syscall.c.obj
filesystem/fatfs/libfatfs.a: filesystem/fatfs/CMakeFiles/fatfs.dir/build.make
filesystem/fatfs/libfatfs.a: filesystem/fatfs/CMakeFiles/fatfs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="E:\3 micropython\2 micropython-esp32\mkfatfs\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking C static library libfatfs.a"
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && $(CMAKE_COMMAND) -P CMakeFiles\fatfs.dir\cmake_clean_target.cmake
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\fatfs.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
filesystem/fatfs/CMakeFiles/fatfs.dir/build: filesystem/fatfs/libfatfs.a

.PHONY : filesystem/fatfs/CMakeFiles/fatfs.dir/build

filesystem/fatfs/CMakeFiles/fatfs.dir/clean:
	cd /d "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" && $(CMAKE_COMMAND) -P CMakeFiles\fatfs.dir\cmake_clean.cmake
.PHONY : filesystem/fatfs/CMakeFiles/fatfs.dir/clean

filesystem/fatfs/CMakeFiles/fatfs.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "E:\3 micropython\2 micropython-esp32\mkfatfs" "E:\3 micropython\2 micropython-esp32\mkfatfs\filesystem\fatfs" "E:\3 micropython\2 micropython-esp32\mkfatfs\build" "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs" "E:\3 micropython\2 micropython-esp32\mkfatfs\build\filesystem\fatfs\CMakeFiles\fatfs.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : filesystem/fatfs/CMakeFiles/fatfs.dir/depend

