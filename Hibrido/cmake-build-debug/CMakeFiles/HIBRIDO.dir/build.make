# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Valentina\Desktop\Cyc\HIBRIDO

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Valentina\Desktop\Cyc\HIBRIDO\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/HIBRIDO.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/HIBRIDO.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HIBRIDO.dir/flags.make

CMakeFiles/HIBRIDO.dir/main.c.obj: CMakeFiles/HIBRIDO.dir/flags.make
CMakeFiles/HIBRIDO.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Valentina\Desktop\Cyc\HIBRIDO\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/HIBRIDO.dir/main.c.obj"
	C:\PROGRA~1\HASKEL~1\828BF2~1.2\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\HIBRIDO.dir\main.c.obj -c C:\Users\Valentina\Desktop\Cyc\HIBRIDO\main.c

CMakeFiles/HIBRIDO.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/HIBRIDO.dir/main.c.i"
	C:\PROGRA~1\HASKEL~1\828BF2~1.2\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Valentina\Desktop\Cyc\HIBRIDO\main.c > CMakeFiles\HIBRIDO.dir\main.c.i

CMakeFiles/HIBRIDO.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/HIBRIDO.dir/main.c.s"
	C:\PROGRA~1\HASKEL~1\828BF2~1.2\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Valentina\Desktop\Cyc\HIBRIDO\main.c -o CMakeFiles\HIBRIDO.dir\main.c.s

# Object files for target HIBRIDO
HIBRIDO_OBJECTS = \
"CMakeFiles/HIBRIDO.dir/main.c.obj"

# External object files for target HIBRIDO
HIBRIDO_EXTERNAL_OBJECTS =

HIBRIDO.exe: CMakeFiles/HIBRIDO.dir/main.c.obj
HIBRIDO.exe: CMakeFiles/HIBRIDO.dir/build.make
HIBRIDO.exe: CMakeFiles/HIBRIDO.dir/linklibs.rsp
HIBRIDO.exe: CMakeFiles/HIBRIDO.dir/objects1.rsp
HIBRIDO.exe: CMakeFiles/HIBRIDO.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Valentina\Desktop\Cyc\HIBRIDO\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable HIBRIDO.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\HIBRIDO.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HIBRIDO.dir/build: HIBRIDO.exe
.PHONY : CMakeFiles/HIBRIDO.dir/build

CMakeFiles/HIBRIDO.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\HIBRIDO.dir\cmake_clean.cmake
.PHONY : CMakeFiles/HIBRIDO.dir/clean

CMakeFiles/HIBRIDO.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Valentina\Desktop\Cyc\HIBRIDO C:\Users\Valentina\Desktop\Cyc\HIBRIDO C:\Users\Valentina\Desktop\Cyc\HIBRIDO\cmake-build-debug C:\Users\Valentina\Desktop\Cyc\HIBRIDO\cmake-build-debug C:\Users\Valentina\Desktop\Cyc\HIBRIDO\cmake-build-debug\CMakeFiles\HIBRIDO.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HIBRIDO.dir/depend

