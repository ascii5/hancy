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
CMAKE_SOURCE_DIR = /home/chenranran/Program/hancy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chenranran/Program/hancy/build

# Include any dependencies generated for this target.
include src/CMakeFiles/serverlib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/serverlib.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/serverlib.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/serverlib.dir/flags.make

src/CMakeFiles/serverlib.dir/log/log.cpp.o: src/CMakeFiles/serverlib.dir/flags.make
src/CMakeFiles/serverlib.dir/log/log.cpp.o: ../src/log/log.cpp
src/CMakeFiles/serverlib.dir/log/log.cpp.o: src/CMakeFiles/serverlib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/serverlib.dir/log/log.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/serverlib.dir/log/log.cpp.o -MF CMakeFiles/serverlib.dir/log/log.cpp.o.d -o CMakeFiles/serverlib.dir/log/log.cpp.o -c /home/chenranran/Program/hancy/src/log/log.cpp

src/CMakeFiles/serverlib.dir/log/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverlib.dir/log/log.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/log/log.cpp > CMakeFiles/serverlib.dir/log/log.cpp.i

src/CMakeFiles/serverlib.dir/log/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverlib.dir/log/log.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/log/log.cpp -o CMakeFiles/serverlib.dir/log/log.cpp.s

src/CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.o: src/CMakeFiles/serverlib.dir/flags.make
src/CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.o: ../src/CGImysql/sql_connection_pool.cpp
src/CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.o: src/CMakeFiles/serverlib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.o -MF CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.o.d -o CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.o -c /home/chenranran/Program/hancy/src/CGImysql/sql_connection_pool.cpp

src/CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/CGImysql/sql_connection_pool.cpp > CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.i

src/CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/CGImysql/sql_connection_pool.cpp -o CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.s

src/CMakeFiles/serverlib.dir/timer/lst_timer.cpp.o: src/CMakeFiles/serverlib.dir/flags.make
src/CMakeFiles/serverlib.dir/timer/lst_timer.cpp.o: ../src/timer/lst_timer.cpp
src/CMakeFiles/serverlib.dir/timer/lst_timer.cpp.o: src/CMakeFiles/serverlib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/serverlib.dir/timer/lst_timer.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/serverlib.dir/timer/lst_timer.cpp.o -MF CMakeFiles/serverlib.dir/timer/lst_timer.cpp.o.d -o CMakeFiles/serverlib.dir/timer/lst_timer.cpp.o -c /home/chenranran/Program/hancy/src/timer/lst_timer.cpp

src/CMakeFiles/serverlib.dir/timer/lst_timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverlib.dir/timer/lst_timer.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/timer/lst_timer.cpp > CMakeFiles/serverlib.dir/timer/lst_timer.cpp.i

src/CMakeFiles/serverlib.dir/timer/lst_timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverlib.dir/timer/lst_timer.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/timer/lst_timer.cpp -o CMakeFiles/serverlib.dir/timer/lst_timer.cpp.s

src/CMakeFiles/serverlib.dir/http/http_conn.cpp.o: src/CMakeFiles/serverlib.dir/flags.make
src/CMakeFiles/serverlib.dir/http/http_conn.cpp.o: ../src/http/http_conn.cpp
src/CMakeFiles/serverlib.dir/http/http_conn.cpp.o: src/CMakeFiles/serverlib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/serverlib.dir/http/http_conn.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/serverlib.dir/http/http_conn.cpp.o -MF CMakeFiles/serverlib.dir/http/http_conn.cpp.o.d -o CMakeFiles/serverlib.dir/http/http_conn.cpp.o -c /home/chenranran/Program/hancy/src/http/http_conn.cpp

src/CMakeFiles/serverlib.dir/http/http_conn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverlib.dir/http/http_conn.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/http/http_conn.cpp > CMakeFiles/serverlib.dir/http/http_conn.cpp.i

src/CMakeFiles/serverlib.dir/http/http_conn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverlib.dir/http/http_conn.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/http/http_conn.cpp -o CMakeFiles/serverlib.dir/http/http_conn.cpp.s

src/CMakeFiles/serverlib.dir/webserver/webserver.cpp.o: src/CMakeFiles/serverlib.dir/flags.make
src/CMakeFiles/serverlib.dir/webserver/webserver.cpp.o: ../src/webserver/webserver.cpp
src/CMakeFiles/serverlib.dir/webserver/webserver.cpp.o: src/CMakeFiles/serverlib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/serverlib.dir/webserver/webserver.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/serverlib.dir/webserver/webserver.cpp.o -MF CMakeFiles/serverlib.dir/webserver/webserver.cpp.o.d -o CMakeFiles/serverlib.dir/webserver/webserver.cpp.o -c /home/chenranran/Program/hancy/src/webserver/webserver.cpp

src/CMakeFiles/serverlib.dir/webserver/webserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverlib.dir/webserver/webserver.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/webserver/webserver.cpp > CMakeFiles/serverlib.dir/webserver/webserver.cpp.i

src/CMakeFiles/serverlib.dir/webserver/webserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverlib.dir/webserver/webserver.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/webserver/webserver.cpp -o CMakeFiles/serverlib.dir/webserver/webserver.cpp.s

src/CMakeFiles/serverlib.dir/config/config.cpp.o: src/CMakeFiles/serverlib.dir/flags.make
src/CMakeFiles/serverlib.dir/config/config.cpp.o: ../src/config/config.cpp
src/CMakeFiles/serverlib.dir/config/config.cpp.o: src/CMakeFiles/serverlib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/serverlib.dir/config/config.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/serverlib.dir/config/config.cpp.o -MF CMakeFiles/serverlib.dir/config/config.cpp.o.d -o CMakeFiles/serverlib.dir/config/config.cpp.o -c /home/chenranran/Program/hancy/src/config/config.cpp

src/CMakeFiles/serverlib.dir/config/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverlib.dir/config/config.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/config/config.cpp > CMakeFiles/serverlib.dir/config/config.cpp.i

src/CMakeFiles/serverlib.dir/config/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverlib.dir/config/config.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/config/config.cpp -o CMakeFiles/serverlib.dir/config/config.cpp.s

# Object files for target serverlib
serverlib_OBJECTS = \
"CMakeFiles/serverlib.dir/log/log.cpp.o" \
"CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.o" \
"CMakeFiles/serverlib.dir/timer/lst_timer.cpp.o" \
"CMakeFiles/serverlib.dir/http/http_conn.cpp.o" \
"CMakeFiles/serverlib.dir/webserver/webserver.cpp.o" \
"CMakeFiles/serverlib.dir/config/config.cpp.o"

# External object files for target serverlib
serverlib_EXTERNAL_OBJECTS =

../lib/libserverlib.a: src/CMakeFiles/serverlib.dir/log/log.cpp.o
../lib/libserverlib.a: src/CMakeFiles/serverlib.dir/CGImysql/sql_connection_pool.cpp.o
../lib/libserverlib.a: src/CMakeFiles/serverlib.dir/timer/lst_timer.cpp.o
../lib/libserverlib.a: src/CMakeFiles/serverlib.dir/http/http_conn.cpp.o
../lib/libserverlib.a: src/CMakeFiles/serverlib.dir/webserver/webserver.cpp.o
../lib/libserverlib.a: src/CMakeFiles/serverlib.dir/config/config.cpp.o
../lib/libserverlib.a: src/CMakeFiles/serverlib.dir/build.make
../lib/libserverlib.a: src/CMakeFiles/serverlib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library ../../lib/libserverlib.a"
	cd /home/chenranran/Program/hancy/build/src && $(CMAKE_COMMAND) -P CMakeFiles/serverlib.dir/cmake_clean_target.cmake
	cd /home/chenranran/Program/hancy/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/serverlib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/serverlib.dir/build: ../lib/libserverlib.a
.PHONY : src/CMakeFiles/serverlib.dir/build

src/CMakeFiles/serverlib.dir/clean:
	cd /home/chenranran/Program/hancy/build/src && $(CMAKE_COMMAND) -P CMakeFiles/serverlib.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/serverlib.dir/clean

src/CMakeFiles/serverlib.dir/depend:
	cd /home/chenranran/Program/hancy/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chenranran/Program/hancy /home/chenranran/Program/hancy/src /home/chenranran/Program/hancy/build /home/chenranran/Program/hancy/build/src /home/chenranran/Program/hancy/build/src/CMakeFiles/serverlib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/serverlib.dir/depend

