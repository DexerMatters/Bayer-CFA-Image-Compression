# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_SOURCE_DIR = /home/dexer/Repos/cpp/bic

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dexer/Repos/cpp/bic

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/dexer/Repos/cpp/bic/CMakeFiles /home/dexer/Repos/cpp/bic//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/dexer/Repos/cpp/bic/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named bic

# Build rule for target.
bic: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 bic
.PHONY : bic

# fast build rule for target.
bic/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/build
.PHONY : bic/fast

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/main.cpp.s
.PHONY : main.cpp.s

src/DF-based.o: src/DF-based.cpp.o
.PHONY : src/DF-based.o

# target to build an object file
src/DF-based.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/src/DF-based.cpp.o
.PHONY : src/DF-based.cpp.o

src/DF-based.i: src/DF-based.cpp.i
.PHONY : src/DF-based.i

# target to preprocess a source file
src/DF-based.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/src/DF-based.cpp.i
.PHONY : src/DF-based.cpp.i

src/DF-based.s: src/DF-based.cpp.s
.PHONY : src/DF-based.s

# target to generate assembly for a file
src/DF-based.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/src/DF-based.cpp.s
.PHONY : src/DF-based.cpp.s

src/bmp.o: src/bmp.cpp.o
.PHONY : src/bmp.o

# target to build an object file
src/bmp.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/src/bmp.cpp.o
.PHONY : src/bmp.cpp.o

src/bmp.i: src/bmp.cpp.i
.PHONY : src/bmp.i

# target to preprocess a source file
src/bmp.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/src/bmp.cpp.i
.PHONY : src/bmp.cpp.i

src/bmp.s: src/bmp.cpp.s
.PHONY : src/bmp.s

# target to generate assembly for a file
src/bmp.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/src/bmp.cpp.s
.PHONY : src/bmp.cpp.s

src/demosaicing.o: src/demosaicing.cpp.o
.PHONY : src/demosaicing.o

# target to build an object file
src/demosaicing.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/src/demosaicing.cpp.o
.PHONY : src/demosaicing.cpp.o

src/demosaicing.i: src/demosaicing.cpp.i
.PHONY : src/demosaicing.i

# target to preprocess a source file
src/demosaicing.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/src/demosaicing.cpp.i
.PHONY : src/demosaicing.cpp.i

src/demosaicing.s: src/demosaicing.cpp.s
.PHONY : src/demosaicing.s

# target to generate assembly for a file
src/demosaicing.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/bic.dir/build.make CMakeFiles/bic.dir/src/demosaicing.cpp.s
.PHONY : src/demosaicing.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... bic"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... src/DF-based.o"
	@echo "... src/DF-based.i"
	@echo "... src/DF-based.s"
	@echo "... src/bmp.o"
	@echo "... src/bmp.i"
	@echo "... src/bmp.s"
	@echo "... src/demosaicing.o"
	@echo "... src/demosaicing.i"
	@echo "... src/demosaicing.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

