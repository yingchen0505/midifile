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
include CMakeFiles/asciimidi.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/asciimidi.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/asciimidi.dir/flags.make

CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o: CMakeFiles/asciimidi.dir/flags.make
CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o: src-programs/asciimidi.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o -c /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/asciimidi.cpp

CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/asciimidi.cpp > CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.i

CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/asciimidi.cpp -o CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.s

CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o.requires:

.PHONY : CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o.requires

CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o.provides: CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o.requires
	$(MAKE) -f CMakeFiles/asciimidi.dir/build.make CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o.provides.build
.PHONY : CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o.provides

CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o.provides.build: CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o


# Object files for target asciimidi
asciimidi_OBJECTS = \
"CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o"

# External object files for target asciimidi
asciimidi_EXTERNAL_OBJECTS =

asciimidi: CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o
asciimidi: CMakeFiles/asciimidi.dir/build.make
asciimidi: libmidifile.a
asciimidi: CMakeFiles/asciimidi.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable asciimidi"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/asciimidi.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/asciimidi.dir/build: asciimidi

.PHONY : CMakeFiles/asciimidi.dir/build

CMakeFiles/asciimidi.dir/requires: CMakeFiles/asciimidi.dir/src-programs/asciimidi.cpp.o.requires

.PHONY : CMakeFiles/asciimidi.dir/requires

CMakeFiles/asciimidi.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/asciimidi.dir/cmake_clean.cmake
.PHONY : CMakeFiles/asciimidi.dir/clean

CMakeFiles/asciimidi.dir/depend:
	cd /mnt/c/Users/Yingchen/Documents/GitHub/midifile && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles/asciimidi.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/asciimidi.dir/depend

