# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cookiecoolkid/project/nit

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cookiecoolkid/project/nit/build

# Include any dependencies generated for this target.
include CMakeFiles/nit.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/nit.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/nit.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/nit.dir/flags.make

CMakeFiles/nit.dir/sha1.cpp.o: CMakeFiles/nit.dir/flags.make
CMakeFiles/nit.dir/sha1.cpp.o: ../sha1.cpp
CMakeFiles/nit.dir/sha1.cpp.o: CMakeFiles/nit.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cookiecoolkid/project/nit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/nit.dir/sha1.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/nit.dir/sha1.cpp.o -MF CMakeFiles/nit.dir/sha1.cpp.o.d -o CMakeFiles/nit.dir/sha1.cpp.o -c /home/cookiecoolkid/project/nit/sha1.cpp

CMakeFiles/nit.dir/sha1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nit.dir/sha1.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cookiecoolkid/project/nit/sha1.cpp > CMakeFiles/nit.dir/sha1.cpp.i

CMakeFiles/nit.dir/sha1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nit.dir/sha1.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cookiecoolkid/project/nit/sha1.cpp -o CMakeFiles/nit.dir/sha1.cpp.s

CMakeFiles/nit.dir/apis.cpp.o: CMakeFiles/nit.dir/flags.make
CMakeFiles/nit.dir/apis.cpp.o: ../apis.cpp
CMakeFiles/nit.dir/apis.cpp.o: CMakeFiles/nit.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cookiecoolkid/project/nit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/nit.dir/apis.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/nit.dir/apis.cpp.o -MF CMakeFiles/nit.dir/apis.cpp.o.d -o CMakeFiles/nit.dir/apis.cpp.o -c /home/cookiecoolkid/project/nit/apis.cpp

CMakeFiles/nit.dir/apis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nit.dir/apis.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cookiecoolkid/project/nit/apis.cpp > CMakeFiles/nit.dir/apis.cpp.i

CMakeFiles/nit.dir/apis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nit.dir/apis.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cookiecoolkid/project/nit/apis.cpp -o CMakeFiles/nit.dir/apis.cpp.s

CMakeFiles/nit.dir/main.cpp.o: CMakeFiles/nit.dir/flags.make
CMakeFiles/nit.dir/main.cpp.o: ../main.cpp
CMakeFiles/nit.dir/main.cpp.o: CMakeFiles/nit.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cookiecoolkid/project/nit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/nit.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/nit.dir/main.cpp.o -MF CMakeFiles/nit.dir/main.cpp.o.d -o CMakeFiles/nit.dir/main.cpp.o -c /home/cookiecoolkid/project/nit/main.cpp

CMakeFiles/nit.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nit.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cookiecoolkid/project/nit/main.cpp > CMakeFiles/nit.dir/main.cpp.i

CMakeFiles/nit.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nit.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cookiecoolkid/project/nit/main.cpp -o CMakeFiles/nit.dir/main.cpp.s

# Object files for target nit
nit_OBJECTS = \
"CMakeFiles/nit.dir/sha1.cpp.o" \
"CMakeFiles/nit.dir/apis.cpp.o" \
"CMakeFiles/nit.dir/main.cpp.o"

# External object files for target nit
nit_EXTERNAL_OBJECTS =

nit: CMakeFiles/nit.dir/sha1.cpp.o
nit: CMakeFiles/nit.dir/apis.cpp.o
nit: CMakeFiles/nit.dir/main.cpp.o
nit: CMakeFiles/nit.dir/build.make
nit: CMakeFiles/nit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cookiecoolkid/project/nit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable nit"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/nit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/nit.dir/build: nit
.PHONY : CMakeFiles/nit.dir/build

CMakeFiles/nit.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/nit.dir/cmake_clean.cmake
.PHONY : CMakeFiles/nit.dir/clean

CMakeFiles/nit.dir/depend:
	cd /home/cookiecoolkid/project/nit/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cookiecoolkid/project/nit /home/cookiecoolkid/project/nit /home/cookiecoolkid/project/nit/build /home/cookiecoolkid/project/nit/build /home/cookiecoolkid/project/nit/build/CMakeFiles/nit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/nit.dir/depend

