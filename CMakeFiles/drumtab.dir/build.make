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
include CMakeFiles/drumtab.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/drumtab.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/drumtab.dir/flags.make

CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o: CMakeFiles/drumtab.dir/flags.make
CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o: src-programs/drumtab.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o -c /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/drumtab.cpp

CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/drumtab.cpp > CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.i

CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/Yingchen/Documents/GitHub/midifile/src-programs/drumtab.cpp -o CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.s

CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o.requires:

.PHONY : CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o.requires

CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o.provides: CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o.requires
	$(MAKE) -f CMakeFiles/drumtab.dir/build.make CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o.provides.build
.PHONY : CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o.provides

CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o.provides.build: CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o


# Object files for target drumtab
drumtab_OBJECTS = \
"CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o"

# External object files for target drumtab
drumtab_EXTERNAL_OBJECTS =

drumtab: CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o
drumtab: CMakeFiles/drumtab.dir/build.make
drumtab: libmidifile.a
drumtab: CMakeFiles/drumtab.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable drumtab"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/drumtab.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/drumtab.dir/build: drumtab

.PHONY : CMakeFiles/drumtab.dir/build

CMakeFiles/drumtab.dir/requires: CMakeFiles/drumtab.dir/src-programs/drumtab.cpp.o.requires

.PHONY : CMakeFiles/drumtab.dir/requires

CMakeFiles/drumtab.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/drumtab.dir/cmake_clean.cmake
.PHONY : CMakeFiles/drumtab.dir/clean

CMakeFiles/drumtab.dir/depend:
	cd /mnt/c/Users/Yingchen/Documents/GitHub/midifile && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile /mnt/c/Users/Yingchen/Documents/GitHub/midifile/CMakeFiles/drumtab.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/drumtab.dir/depend

