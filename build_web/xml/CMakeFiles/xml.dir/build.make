# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/manh/Documents/manh/macao/web

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/manh/Documents/manh/macao/build_web

# Include any dependencies generated for this target.
include xml/CMakeFiles/xml.dir/depend.make

# Include the progress variables for this target.
include xml/CMakeFiles/xml.dir/progress.make

# Include the compile flags for this target's objects.
include xml/CMakeFiles/xml.dir/flags.make

xml/CMakeFiles/xml.dir/xml.c.o: xml/CMakeFiles/xml.dir/flags.make
xml/CMakeFiles/xml.dir/xml.c.o: xml/CMakeFiles/xml.dir/includes_C.rsp
xml/CMakeFiles/xml.dir/xml.c.o: /home/manh/Documents/manh/cherry/xml/xml.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manh/Documents/manh/macao/build_web/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object xml/CMakeFiles/xml.dir/xml.c.o"
	cd /home/manh/Documents/manh/macao/build_web/xml && /home/manh/Downloads/emsdk_portable/emscripten/master/emcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/xml.dir/xml.c.o   -c /home/manh/Documents/manh/cherry/xml/xml.c

xml/CMakeFiles/xml.dir/xml.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/xml.dir/xml.c.i"
	cd /home/manh/Documents/manh/macao/build_web/xml && /home/manh/Downloads/emsdk_portable/emscripten/master/emcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/manh/Documents/manh/cherry/xml/xml.c > CMakeFiles/xml.dir/xml.c.i

xml/CMakeFiles/xml.dir/xml.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/xml.dir/xml.c.s"
	cd /home/manh/Documents/manh/macao/build_web/xml && /home/manh/Downloads/emsdk_portable/emscripten/master/emcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/manh/Documents/manh/cherry/xml/xml.c -o CMakeFiles/xml.dir/xml.c.s

xml/CMakeFiles/xml.dir/xml.c.o.requires:

.PHONY : xml/CMakeFiles/xml.dir/xml.c.o.requires

xml/CMakeFiles/xml.dir/xml.c.o.provides: xml/CMakeFiles/xml.dir/xml.c.o.requires
	$(MAKE) -f xml/CMakeFiles/xml.dir/build.make xml/CMakeFiles/xml.dir/xml.c.o.provides.build
.PHONY : xml/CMakeFiles/xml.dir/xml.c.o.provides

xml/CMakeFiles/xml.dir/xml.c.o.provides.build: xml/CMakeFiles/xml.dir/xml.c.o


# Object files for target xml
xml_OBJECTS = \
"CMakeFiles/xml.dir/xml.c.o"

# External object files for target xml
xml_EXTERNAL_OBJECTS =

xml/libxml.a: xml/CMakeFiles/xml.dir/xml.c.o
xml/libxml.a: xml/CMakeFiles/xml.dir/build.make
xml/libxml.a: xml/CMakeFiles/xml.dir/objects1.rsp
xml/libxml.a: xml/CMakeFiles/xml.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/manh/Documents/manh/macao/build_web/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libxml.a"
	cd /home/manh/Documents/manh/macao/build_web/xml && $(CMAKE_COMMAND) -P CMakeFiles/xml.dir/cmake_clean_target.cmake
	cd /home/manh/Documents/manh/macao/build_web/xml && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/xml.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
xml/CMakeFiles/xml.dir/build: xml/libxml.a

.PHONY : xml/CMakeFiles/xml.dir/build

xml/CMakeFiles/xml.dir/requires: xml/CMakeFiles/xml.dir/xml.c.o.requires

.PHONY : xml/CMakeFiles/xml.dir/requires

xml/CMakeFiles/xml.dir/clean:
	cd /home/manh/Documents/manh/macao/build_web/xml && $(CMAKE_COMMAND) -P CMakeFiles/xml.dir/cmake_clean.cmake
.PHONY : xml/CMakeFiles/xml.dir/clean

xml/CMakeFiles/xml.dir/depend:
	cd /home/manh/Documents/manh/macao/build_web && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/manh/Documents/manh/macao/web /home/manh/Documents/manh/cherry/xml /home/manh/Documents/manh/macao/build_web /home/manh/Documents/manh/macao/build_web/xml /home/manh/Documents/manh/macao/build_web/xml/CMakeFiles/xml.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : xml/CMakeFiles/xml.dir/depend

