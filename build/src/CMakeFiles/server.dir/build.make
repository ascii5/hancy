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
include src/CMakeFiles/server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/server.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/server.dir/flags.make

src/CMakeFiles/server.dir/main.cpp.o: src/CMakeFiles/server.dir/flags.make
src/CMakeFiles/server.dir/main.cpp.o: ../src/main.cpp
src/CMakeFiles/server.dir/main.cpp.o: src/CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/server.dir/main.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/server.dir/main.cpp.o -MF CMakeFiles/server.dir/main.cpp.o.d -o CMakeFiles/server.dir/main.cpp.o -c /home/chenranran/Program/hancy/src/main.cpp

src/CMakeFiles/server.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/main.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/main.cpp > CMakeFiles/server.dir/main.cpp.i

src/CMakeFiles/server.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/main.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/main.cpp -o CMakeFiles/server.dir/main.cpp.s

src/CMakeFiles/server.dir/log/log.cpp.o: src/CMakeFiles/server.dir/flags.make
src/CMakeFiles/server.dir/log/log.cpp.o: ../src/log/log.cpp
src/CMakeFiles/server.dir/log/log.cpp.o: src/CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/server.dir/log/log.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/server.dir/log/log.cpp.o -MF CMakeFiles/server.dir/log/log.cpp.o.d -o CMakeFiles/server.dir/log/log.cpp.o -c /home/chenranran/Program/hancy/src/log/log.cpp

src/CMakeFiles/server.dir/log/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/log/log.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/log/log.cpp > CMakeFiles/server.dir/log/log.cpp.i

src/CMakeFiles/server.dir/log/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/log/log.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/log/log.cpp -o CMakeFiles/server.dir/log/log.cpp.s

src/CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o: src/CMakeFiles/server.dir/flags.make
src/CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o: ../src/CGImysql/sql_connection_pool.cpp
src/CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o: src/CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o -MF CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o.d -o CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o -c /home/chenranran/Program/hancy/src/CGImysql/sql_connection_pool.cpp

src/CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/CGImysql/sql_connection_pool.cpp > CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.i

src/CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/CGImysql/sql_connection_pool.cpp -o CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.s

src/CMakeFiles/server.dir/timer/lst_timer.cpp.o: src/CMakeFiles/server.dir/flags.make
src/CMakeFiles/server.dir/timer/lst_timer.cpp.o: ../src/timer/lst_timer.cpp
src/CMakeFiles/server.dir/timer/lst_timer.cpp.o: src/CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/server.dir/timer/lst_timer.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/server.dir/timer/lst_timer.cpp.o -MF CMakeFiles/server.dir/timer/lst_timer.cpp.o.d -o CMakeFiles/server.dir/timer/lst_timer.cpp.o -c /home/chenranran/Program/hancy/src/timer/lst_timer.cpp

src/CMakeFiles/server.dir/timer/lst_timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/timer/lst_timer.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/timer/lst_timer.cpp > CMakeFiles/server.dir/timer/lst_timer.cpp.i

src/CMakeFiles/server.dir/timer/lst_timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/timer/lst_timer.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/timer/lst_timer.cpp -o CMakeFiles/server.dir/timer/lst_timer.cpp.s

src/CMakeFiles/server.dir/http/http_conn.cpp.o: src/CMakeFiles/server.dir/flags.make
src/CMakeFiles/server.dir/http/http_conn.cpp.o: ../src/http/http_conn.cpp
src/CMakeFiles/server.dir/http/http_conn.cpp.o: src/CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/server.dir/http/http_conn.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/server.dir/http/http_conn.cpp.o -MF CMakeFiles/server.dir/http/http_conn.cpp.o.d -o CMakeFiles/server.dir/http/http_conn.cpp.o -c /home/chenranran/Program/hancy/src/http/http_conn.cpp

src/CMakeFiles/server.dir/http/http_conn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/http/http_conn.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/http/http_conn.cpp > CMakeFiles/server.dir/http/http_conn.cpp.i

src/CMakeFiles/server.dir/http/http_conn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/http/http_conn.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/http/http_conn.cpp -o CMakeFiles/server.dir/http/http_conn.cpp.s

src/CMakeFiles/server.dir/webserver/webserver.cpp.o: src/CMakeFiles/server.dir/flags.make
src/CMakeFiles/server.dir/webserver/webserver.cpp.o: ../src/webserver/webserver.cpp
src/CMakeFiles/server.dir/webserver/webserver.cpp.o: src/CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/server.dir/webserver/webserver.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/server.dir/webserver/webserver.cpp.o -MF CMakeFiles/server.dir/webserver/webserver.cpp.o.d -o CMakeFiles/server.dir/webserver/webserver.cpp.o -c /home/chenranran/Program/hancy/src/webserver/webserver.cpp

src/CMakeFiles/server.dir/webserver/webserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/webserver/webserver.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/webserver/webserver.cpp > CMakeFiles/server.dir/webserver/webserver.cpp.i

src/CMakeFiles/server.dir/webserver/webserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/webserver/webserver.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/webserver/webserver.cpp -o CMakeFiles/server.dir/webserver/webserver.cpp.s

src/CMakeFiles/server.dir/config/config.cpp.o: src/CMakeFiles/server.dir/flags.make
src/CMakeFiles/server.dir/config/config.cpp.o: ../src/config/config.cpp
src/CMakeFiles/server.dir/config/config.cpp.o: src/CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/server.dir/config/config.cpp.o"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/server.dir/config/config.cpp.o -MF CMakeFiles/server.dir/config/config.cpp.o.d -o CMakeFiles/server.dir/config/config.cpp.o -c /home/chenranran/Program/hancy/src/config/config.cpp

src/CMakeFiles/server.dir/config/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/config/config.cpp.i"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenranran/Program/hancy/src/config/config.cpp > CMakeFiles/server.dir/config/config.cpp.i

src/CMakeFiles/server.dir/config/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/config/config.cpp.s"
	cd /home/chenranran/Program/hancy/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenranran/Program/hancy/src/config/config.cpp -o CMakeFiles/server.dir/config/config.cpp.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/main.cpp.o" \
"CMakeFiles/server.dir/log/log.cpp.o" \
"CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o" \
"CMakeFiles/server.dir/timer/lst_timer.cpp.o" \
"CMakeFiles/server.dir/http/http_conn.cpp.o" \
"CMakeFiles/server.dir/webserver/webserver.cpp.o" \
"CMakeFiles/server.dir/config/config.cpp.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

../bin/server: src/CMakeFiles/server.dir/main.cpp.o
../bin/server: src/CMakeFiles/server.dir/log/log.cpp.o
../bin/server: src/CMakeFiles/server.dir/CGImysql/sql_connection_pool.cpp.o
../bin/server: src/CMakeFiles/server.dir/timer/lst_timer.cpp.o
../bin/server: src/CMakeFiles/server.dir/http/http_conn.cpp.o
../bin/server: src/CMakeFiles/server.dir/webserver/webserver.cpp.o
../bin/server: src/CMakeFiles/server.dir/config/config.cpp.o
../bin/server: src/CMakeFiles/server.dir/build.make
../bin/server: ../lib/libserverlib.a
../bin/server: src/CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chenranran/Program/hancy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable ../../bin/server"
	cd /home/chenranran/Program/hancy/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/server.dir/build: ../bin/server
.PHONY : src/CMakeFiles/server.dir/build

src/CMakeFiles/server.dir/clean:
	cd /home/chenranran/Program/hancy/build/src && $(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/server.dir/clean

src/CMakeFiles/server.dir/depend:
	cd /home/chenranran/Program/hancy/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chenranran/Program/hancy /home/chenranran/Program/hancy/src /home/chenranran/Program/hancy/build /home/chenranran/Program/hancy/build/src /home/chenranran/Program/hancy/build/src/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/server.dir/depend
