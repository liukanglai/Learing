# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/liukanglai/Learing/Code/CLionProjects/democ

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/liukanglai/Learing/Code/CLionProjects/democ/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/democ.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/democ.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/democ.dir/flags.make

CMakeFiles/democ.dir/OS/fork.c.o: CMakeFiles/democ.dir/flags.make
CMakeFiles/democ.dir/OS/fork.c.o: ../OS/fork.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/liukanglai/Learing/Code/CLionProjects/democ/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/democ.dir/OS/fork.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/democ.dir/OS/fork.c.o -c /home/liukanglai/Learing/Code/CLionProjects/democ/OS/fork.c

CMakeFiles/democ.dir/OS/fork.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/democ.dir/OS/fork.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/liukanglai/Learing/Code/CLionProjects/democ/OS/fork.c > CMakeFiles/democ.dir/OS/fork.c.i

CMakeFiles/democ.dir/OS/fork.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/democ.dir/OS/fork.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/liukanglai/Learing/Code/CLionProjects/democ/OS/fork.c -o CMakeFiles/democ.dir/OS/fork.c.s

# Object files for target democ
democ_OBJECTS = \
"CMakeFiles/democ.dir/OS/fork.c.o"

# External object files for target democ
democ_EXTERNAL_OBJECTS =

democ: CMakeFiles/democ.dir/OS/fork.c.o
democ: CMakeFiles/democ.dir/build.make
democ: CMakeFiles/democ.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/liukanglai/Learing/Code/CLionProjects/democ/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable democ"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/democ.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/democ.dir/build: democ
.PHONY : CMakeFiles/democ.dir/build

CMakeFiles/democ.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/democ.dir/cmake_clean.cmake
.PHONY : CMakeFiles/democ.dir/clean

CMakeFiles/democ.dir/depend:
	cd /home/liukanglai/Learing/Code/CLionProjects/democ/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/liukanglai/Learing/Code/CLionProjects/democ /home/liukanglai/Learing/Code/CLionProjects/democ /home/liukanglai/Learing/Code/CLionProjects/democ/cmake-build-debug /home/liukanglai/Learing/Code/CLionProjects/democ/cmake-build-debug /home/liukanglai/Learing/Code/CLionProjects/democ/cmake-build-debug/CMakeFiles/democ.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/democ.dir/depend

