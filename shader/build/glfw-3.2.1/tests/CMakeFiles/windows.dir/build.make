# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.6.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.6.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/david/Documents/Senior/184/cs184/assignment1/example_01

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build

# Include any dependencies generated for this target.
include glfw-3.2.1/tests/CMakeFiles/windows.dir/depend.make

# Include the progress variables for this target.
include glfw-3.2.1/tests/CMakeFiles/windows.dir/progress.make

# Include the compile flags for this target's objects.
include glfw-3.2.1/tests/CMakeFiles/windows.dir/flags.make

glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o: glfw-3.2.1/tests/CMakeFiles/windows.dir/flags.make
glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o: ../glfw-3.2.1/tests/windows.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o"
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/windows.dir/windows.c.o   -c /Users/david/Documents/Senior/184/cs184/assignment1/example_01/glfw-3.2.1/tests/windows.c

glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/windows.dir/windows.c.i"
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/david/Documents/Senior/184/cs184/assignment1/example_01/glfw-3.2.1/tests/windows.c > CMakeFiles/windows.dir/windows.c.i

glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/windows.dir/windows.c.s"
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/david/Documents/Senior/184/cs184/assignment1/example_01/glfw-3.2.1/tests/windows.c -o CMakeFiles/windows.dir/windows.c.s

glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o.requires:

.PHONY : glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o.requires

glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o.provides: glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o.requires
	$(MAKE) -f glfw-3.2.1/tests/CMakeFiles/windows.dir/build.make glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o.provides.build
.PHONY : glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o.provides

glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o.provides.build: glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o


glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o: glfw-3.2.1/tests/CMakeFiles/windows.dir/flags.make
glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o: ../glfw-3.2.1/deps/getopt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o"
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/windows.dir/__/deps/getopt.c.o   -c /Users/david/Documents/Senior/184/cs184/assignment1/example_01/glfw-3.2.1/deps/getopt.c

glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/windows.dir/__/deps/getopt.c.i"
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/david/Documents/Senior/184/cs184/assignment1/example_01/glfw-3.2.1/deps/getopt.c > CMakeFiles/windows.dir/__/deps/getopt.c.i

glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/windows.dir/__/deps/getopt.c.s"
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/david/Documents/Senior/184/cs184/assignment1/example_01/glfw-3.2.1/deps/getopt.c -o CMakeFiles/windows.dir/__/deps/getopt.c.s

glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o.requires:

.PHONY : glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o.requires

glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o.provides: glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o.requires
	$(MAKE) -f glfw-3.2.1/tests/CMakeFiles/windows.dir/build.make glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o.provides.build
.PHONY : glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o.provides

glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o.provides.build: glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o


glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o: glfw-3.2.1/tests/CMakeFiles/windows.dir/flags.make
glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o: ../glfw-3.2.1/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o"
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/windows.dir/__/deps/glad.c.o   -c /Users/david/Documents/Senior/184/cs184/assignment1/example_01/glfw-3.2.1/deps/glad.c

glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/windows.dir/__/deps/glad.c.i"
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/david/Documents/Senior/184/cs184/assignment1/example_01/glfw-3.2.1/deps/glad.c > CMakeFiles/windows.dir/__/deps/glad.c.i

glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/windows.dir/__/deps/glad.c.s"
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/david/Documents/Senior/184/cs184/assignment1/example_01/glfw-3.2.1/deps/glad.c -o CMakeFiles/windows.dir/__/deps/glad.c.s

glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o.requires:

.PHONY : glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o.requires

glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o.provides: glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o.requires
	$(MAKE) -f glfw-3.2.1/tests/CMakeFiles/windows.dir/build.make glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o.provides.build
.PHONY : glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o.provides

glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o.provides.build: glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o


# Object files for target windows
windows_OBJECTS = \
"CMakeFiles/windows.dir/windows.c.o" \
"CMakeFiles/windows.dir/__/deps/getopt.c.o" \
"CMakeFiles/windows.dir/__/deps/glad.c.o"

# External object files for target windows
windows_EXTERNAL_OBJECTS =

glfw-3.2.1/tests/windows.app/Contents/MacOS/windows: glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o
glfw-3.2.1/tests/windows.app/Contents/MacOS/windows: glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o
glfw-3.2.1/tests/windows.app/Contents/MacOS/windows: glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o
glfw-3.2.1/tests/windows.app/Contents/MacOS/windows: glfw-3.2.1/tests/CMakeFiles/windows.dir/build.make
glfw-3.2.1/tests/windows.app/Contents/MacOS/windows: glfw-3.2.1/src/libglfw3.a
glfw-3.2.1/tests/windows.app/Contents/MacOS/windows: glfw-3.2.1/tests/CMakeFiles/windows.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable windows.app/Contents/MacOS/windows"
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/windows.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glfw-3.2.1/tests/CMakeFiles/windows.dir/build: glfw-3.2.1/tests/windows.app/Contents/MacOS/windows

.PHONY : glfw-3.2.1/tests/CMakeFiles/windows.dir/build

glfw-3.2.1/tests/CMakeFiles/windows.dir/requires: glfw-3.2.1/tests/CMakeFiles/windows.dir/windows.c.o.requires
glfw-3.2.1/tests/CMakeFiles/windows.dir/requires: glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o.requires
glfw-3.2.1/tests/CMakeFiles/windows.dir/requires: glfw-3.2.1/tests/CMakeFiles/windows.dir/__/deps/glad.c.o.requires

.PHONY : glfw-3.2.1/tests/CMakeFiles/windows.dir/requires

glfw-3.2.1/tests/CMakeFiles/windows.dir/clean:
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests && $(CMAKE_COMMAND) -P CMakeFiles/windows.dir/cmake_clean.cmake
.PHONY : glfw-3.2.1/tests/CMakeFiles/windows.dir/clean

glfw-3.2.1/tests/CMakeFiles/windows.dir/depend:
	cd /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/david/Documents/Senior/184/cs184/assignment1/example_01 /Users/david/Documents/Senior/184/cs184/assignment1/example_01/glfw-3.2.1/tests /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests /Users/david/Documents/Senior/184/cs184/assignment1/example_01/build/glfw-3.2.1/tests/CMakeFiles/windows.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glfw-3.2.1/tests/CMakeFiles/windows.dir/depend

