# CMake generated Testfile for 
# Source directory: D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests
# Build directory: D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(spdlog-utests "D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/build/tests/Debug/spdlog-utests.exe")
  set_tests_properties(spdlog-utests PROPERTIES  _BACKTRACE_TRIPLES "D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;49;add_test;D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;54;spdlog_prepare_test;D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(spdlog-utests "D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/build/tests/Release/spdlog-utests.exe")
  set_tests_properties(spdlog-utests PROPERTIES  _BACKTRACE_TRIPLES "D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;49;add_test;D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;54;spdlog_prepare_test;D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(spdlog-utests "D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/build/tests/MinSizeRel/spdlog-utests.exe")
  set_tests_properties(spdlog-utests PROPERTIES  _BACKTRACE_TRIPLES "D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;49;add_test;D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;54;spdlog_prepare_test;D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(spdlog-utests "D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/build/tests/RelWithDebInfo/spdlog-utests.exe")
  set_tests_properties(spdlog-utests PROPERTIES  _BACKTRACE_TRIPLES "D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;49;add_test;D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;54;spdlog_prepare_test;D:/Development/Projects/Vortex/Dependencies/spdlog-1.5.0/tests/CMakeLists.txt;0;")
else()
  add_test(spdlog-utests NOT_AVAILABLE)
endif()
