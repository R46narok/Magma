# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Development\Software\CLion 2020.3.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Development\Software\CLion 2020.3.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Development\Projects\SmartCity

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Development\Projects\SmartCity\cmake-build-debug

# Include any dependencies generated for this target.
include src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\depend.make

# Include the progress variables for this target.
include src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\progress.make

# Include the compile flags for this target's objects.
include src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\flags.make

src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\Buffer.cpp.obj: src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\flags.make
src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\Buffer.cpp.obj: ..\src\SmartCity.Graphics\Buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Development\Projects\SmartCity\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/SmartCity.Graphics/CMakeFiles/SmartCity.Graphics.dir/Buffer.cpp.obj"
	cd D:\Development\Projects\SmartCity\cmake-build-debug\src\SmartCity.Graphics
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\SmartCity.Graphics.dir\Buffer.cpp.obj /FdCMakeFiles\SmartCity.Graphics.dir\ /FS -c D:\Development\Projects\SmartCity\src\SmartCity.Graphics\Buffer.cpp
<<
	cd D:\Development\Projects\SmartCity\cmake-build-debug

src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\Buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SmartCity.Graphics.dir/Buffer.cpp.i"
	cd D:\Development\Projects\SmartCity\cmake-build-debug\src\SmartCity.Graphics
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe > CMakeFiles\SmartCity.Graphics.dir\Buffer.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Development\Projects\SmartCity\src\SmartCity.Graphics\Buffer.cpp
<<
	cd D:\Development\Projects\SmartCity\cmake-build-debug

src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\Buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SmartCity.Graphics.dir/Buffer.cpp.s"
	cd D:\Development\Projects\SmartCity\cmake-build-debug\src\SmartCity.Graphics
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\SmartCity.Graphics.dir\Buffer.cpp.s /c D:\Development\Projects\SmartCity\src\SmartCity.Graphics\Buffer.cpp
<<
	cd D:\Development\Projects\SmartCity\cmake-build-debug

src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\Window.cpp.obj: src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\flags.make
src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\Window.cpp.obj: ..\src\SmartCity.Graphics\Window.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Development\Projects\SmartCity\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/SmartCity.Graphics/CMakeFiles/SmartCity.Graphics.dir/Window.cpp.obj"
	cd D:\Development\Projects\SmartCity\cmake-build-debug\src\SmartCity.Graphics
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\SmartCity.Graphics.dir\Window.cpp.obj /FdCMakeFiles\SmartCity.Graphics.dir\ /FS -c D:\Development\Projects\SmartCity\src\SmartCity.Graphics\Window.cpp
<<
	cd D:\Development\Projects\SmartCity\cmake-build-debug

src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\Window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SmartCity.Graphics.dir/Window.cpp.i"
	cd D:\Development\Projects\SmartCity\cmake-build-debug\src\SmartCity.Graphics
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe > CMakeFiles\SmartCity.Graphics.dir\Window.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Development\Projects\SmartCity\src\SmartCity.Graphics\Window.cpp
<<
	cd D:\Development\Projects\SmartCity\cmake-build-debug

src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\Window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SmartCity.Graphics.dir/Window.cpp.s"
	cd D:\Development\Projects\SmartCity\cmake-build-debug\src\SmartCity.Graphics
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\SmartCity.Graphics.dir\Window.cpp.s /c D:\Development\Projects\SmartCity\src\SmartCity.Graphics\Window.cpp
<<
	cd D:\Development\Projects\SmartCity\cmake-build-debug

# Object files for target SmartCity.Graphics
SmartCity_Graphics_OBJECTS = \
"CMakeFiles\SmartCity.Graphics.dir\Buffer.cpp.obj" \
"CMakeFiles\SmartCity.Graphics.dir\Window.cpp.obj"

# External object files for target SmartCity.Graphics
SmartCity_Graphics_EXTERNAL_OBJECTS =

bin\SmartCity.Graphics.dll: src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\Buffer.cpp.obj
bin\SmartCity.Graphics.dll: src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\Window.cpp.obj
bin\SmartCity.Graphics.dll: src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\build.make
bin\SmartCity.Graphics.dll: src\SmartCity.Domain\SmartCity.Domain.lib
bin\SmartCity.Graphics.dll: submodules\glfw\src\glfw3.lib
bin\SmartCity.Graphics.dll: src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Development\Projects\SmartCity\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ..\..\bin\SmartCity.Graphics.dll"
	cd D:\Development\Projects\SmartCity\cmake-build-debug\src\SmartCity.Graphics
	"D:\Development\Software\CLion 2020.3.3\bin\cmake\win\bin\cmake.exe" -E vs_link_dll --intdir=CMakeFiles\SmartCity.Graphics.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x64\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x64\mt.exe --manifests  -- C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\link.exe /nologo @CMakeFiles\SmartCity.Graphics.dir\objects1.rsp @<<
 /out:..\..\bin\SmartCity.Graphics.dll /implib:SmartCity.Graphics.lib /pdb:D:\Development\Projects\SmartCity\cmake-build-debug\bin\SmartCity.Graphics.pdb /dll /version:0.0 /machine:x64 /debug /INCREMENTAL  ..\SmartCity.Domain\SmartCity.Domain.lib ..\..\submodules\glfw\src\glfw3.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib  
<<
	cd D:\Development\Projects\SmartCity\cmake-build-debug

# Rule to build all files generated by this target.
src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\build: bin\SmartCity.Graphics.dll

.PHONY : src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\build

src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\clean:
	cd D:\Development\Projects\SmartCity\cmake-build-debug\src\SmartCity.Graphics
	$(CMAKE_COMMAND) -P CMakeFiles\SmartCity.Graphics.dir\cmake_clean.cmake
	cd D:\Development\Projects\SmartCity\cmake-build-debug
.PHONY : src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\clean

src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\Development\Projects\SmartCity D:\Development\Projects\SmartCity\src\SmartCity.Graphics D:\Development\Projects\SmartCity\cmake-build-debug D:\Development\Projects\SmartCity\cmake-build-debug\src\SmartCity.Graphics D:\Development\Projects\SmartCity\cmake-build-debug\src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src\SmartCity.Graphics\CMakeFiles\SmartCity.Graphics.dir\depend

