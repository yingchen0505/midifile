# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /mnt/c/Users/Yingchen/Documents/GitHub/midifile

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Yingchen/Documents/GitHub/midifile

# Include any dependencies generated for this target.
include CMakeFiles/midi2melody.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/midi2melody.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/midi2melody.dir/flags.make

CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o: CMakeFiles/midi2melody.dir/flags.make
CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o: src-programs/midi2melody.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o -c /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/midi2melody.cpp

CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/midi2melody.cpp > CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.i

CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/midi2melody.cpp -o CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.s

CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o.requires:

.PHONY : CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o.requires

CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o.provides: CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o.requires
	$(MAKE) -f CMakeFiles/midi2melody.dir/build.make CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o.provides.build
.PHONY : CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o.provides

CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o.provides.build: CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o


# Object files for target midi2melody
midi2melody_OBJECTS = \
"CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o"

# External object files for target midi2melody
midi2melody_EXTERNAL_OBJECTS =

midi2melody: CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o
midi2melody: CMakeFiles/midi2melody.dir/build.make
midi2melody: libmidifile.a
midi2melody: CMakeFiles/midi2melody.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable midi2melody"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/midi2melody.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/midi2melody.dir/build: midi2melody

.PHONY : CMakeFiles/midi2melody.dir/build

CMakeFiles/midi2melody.dir/requires: CMakeFiles/midi2melody.dir/src-programs/midi2melody.cpp.o.requires

.PHONY : CMakeFiles/midi2melody.dir/requires

CMakeFiles/midi2melody.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/midi2melody.dir/cmake_clean.cmake
.PHONY : CMakeFiles/midi2melody.dir/clean

CMakeFiles/midi2melody.dir/depend:
	cd /mnt/c/Users/Yingchen/Documents/GitHub/midifile && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles/midi2melody.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/midi2melody.dir/depend

