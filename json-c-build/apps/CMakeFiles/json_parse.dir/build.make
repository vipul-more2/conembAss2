# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/Assignment2/json-c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Assignment2/json-c-build

# Include any dependencies generated for this target.
include apps/CMakeFiles/json_parse.dir/depend.make

# Include the progress variables for this target.
include apps/CMakeFiles/json_parse.dir/progress.make

# Include the compile flags for this target's objects.
include apps/CMakeFiles/json_parse.dir/flags.make

apps/CMakeFiles/json_parse.dir/json_parse.c.o: apps/CMakeFiles/json_parse.dir/flags.make
apps/CMakeFiles/json_parse.dir/json_parse.c.o: /home/pi/Assignment2/json-c/apps/json_parse.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Assignment2/json-c-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object apps/CMakeFiles/json_parse.dir/json_parse.c.o"
	cd /home/pi/Assignment2/json-c-build/apps && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/json_parse.dir/json_parse.c.o   -c /home/pi/Assignment2/json-c/apps/json_parse.c

apps/CMakeFiles/json_parse.dir/json_parse.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/json_parse.dir/json_parse.c.i"
	cd /home/pi/Assignment2/json-c-build/apps && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Assignment2/json-c/apps/json_parse.c > CMakeFiles/json_parse.dir/json_parse.c.i

apps/CMakeFiles/json_parse.dir/json_parse.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/json_parse.dir/json_parse.c.s"
	cd /home/pi/Assignment2/json-c-build/apps && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Assignment2/json-c/apps/json_parse.c -o CMakeFiles/json_parse.dir/json_parse.c.s

# Object files for target json_parse
json_parse_OBJECTS = \
"CMakeFiles/json_parse.dir/json_parse.c.o"

# External object files for target json_parse
json_parse_EXTERNAL_OBJECTS =

apps/json_parse: apps/CMakeFiles/json_parse.dir/json_parse.c.o
apps/json_parse: apps/CMakeFiles/json_parse.dir/build.make
apps/json_parse: libjson-c.so.5.1.0
apps/json_parse: apps/CMakeFiles/json_parse.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/Assignment2/json-c-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable json_parse"
	cd /home/pi/Assignment2/json-c-build/apps && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/json_parse.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/CMakeFiles/json_parse.dir/build: apps/json_parse

.PHONY : apps/CMakeFiles/json_parse.dir/build

apps/CMakeFiles/json_parse.dir/clean:
	cd /home/pi/Assignment2/json-c-build/apps && $(CMAKE_COMMAND) -P CMakeFiles/json_parse.dir/cmake_clean.cmake
.PHONY : apps/CMakeFiles/json_parse.dir/clean

apps/CMakeFiles/json_parse.dir/depend:
	cd /home/pi/Assignment2/json-c-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Assignment2/json-c /home/pi/Assignment2/json-c/apps /home/pi/Assignment2/json-c-build /home/pi/Assignment2/json-c-build/apps /home/pi/Assignment2/json-c-build/apps/CMakeFiles/json_parse.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/CMakeFiles/json_parse.dir/depend

