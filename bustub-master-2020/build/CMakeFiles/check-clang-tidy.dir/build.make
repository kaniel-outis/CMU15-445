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
CMAKE_SOURCE_DIR = /bustub

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /bustub/build

# Utility rule file for check-clang-tidy.

# Include the progress variables for this target.
include CMakeFiles/check-clang-tidy.dir/progress.make

CMakeFiles/check-clang-tidy:
	../build_support/run_clang_tidy.py -clang-tidy-binary /usr/bin/clang-tidy-8 -p /bustub/build

check-clang-tidy: CMakeFiles/check-clang-tidy
check-clang-tidy: CMakeFiles/check-clang-tidy.dir/build.make

.PHONY : check-clang-tidy

# Rule to build all files generated by this target.
CMakeFiles/check-clang-tidy.dir/build: check-clang-tidy

.PHONY : CMakeFiles/check-clang-tidy.dir/build

CMakeFiles/check-clang-tidy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/check-clang-tidy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/check-clang-tidy.dir/clean

CMakeFiles/check-clang-tidy.dir/depend:
	cd /bustub/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /bustub /bustub /bustub/build /bustub/build /bustub/build/CMakeFiles/check-clang-tidy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/check-clang-tidy.dir/depend

