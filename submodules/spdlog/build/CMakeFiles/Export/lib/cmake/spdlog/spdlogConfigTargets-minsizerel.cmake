#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "spdlog::spdlog" for configuration "MinSizeRel"
set_property(TARGET spdlog::spdlog APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(spdlog::spdlog PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_MINSIZEREL "CXX"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/spdlog.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS spdlog::spdlog )
list(APPEND _IMPORT_CHECK_FILES_FOR_spdlog::spdlog "${_IMPORT_PREFIX}/lib/spdlog.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
