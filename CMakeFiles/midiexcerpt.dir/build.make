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
include CMakeFiles/midiexcerpt.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/midiexcerpt.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/midiexcerpt.dir/flags.make

CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o: CMakeFiles/midiexcerpt.dir/flags.make
CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o: src-programs/midiexcerpt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o -c /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/midiexcerpt.cpp

CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/midiexcerpt.cpp > CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.i

CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/midiexcerpt.cpp -o CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.s

CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o.requires:

.PHONY : CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o.requires

CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o.provides: CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o.requires
	$(MAKE) -f CMakeFiles/midiexcerpt.dir/build.make CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o.provides.build
.PHONY : CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o.provides

CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o.provides.build: CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o


# Object files for target midiexcerpt
midiexcerpt_OBJECTS = \
"CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o"

# External object files for target midiexcerpt
midiexcerpt_EXTERNAL_OBJECTS =

midiexcerpt: CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o
midiexcerpt: CMakeFiles/midiexcerpt.dir/build.make
midiexcerpt: libmidifile.a
midiexcerpt: CMakeFiles/midiexcerpt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable midiexcerpt"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/midiexcerpt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/midiexcerpt.dir/build: midiexcerpt

.PHONY : CMakeFiles/midiexcerpt.dir/build

CMakeFiles/midiexcerpt.dir/requires: CMakeFiles/midiexcerpt.dir/src-programs/midiexcerpt.cpp.o.requires

.PHONY : CMakeFiles/midiexcerpt.dir/requires

CMakeFiles/midiexcerpt.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/midiexcerpt.dir/cmake_clean.cmake
.PHONY : CMakeFiles/midiexcerpt.dir/clean

CMakeFiles/midiexcerpt.dir/depend:
	cd /mnt/c/Users/Yingchen/Documents/GitHub/midifile && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles/midiexcerpt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/midiexcerpt.dir/depend

