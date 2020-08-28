#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "pistache_static" for configuration "Release"
set_property(TARGET pistache_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(pistache_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libpistache.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS pistache_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_pistache_static "${_IMPORT_PREFIX}/lib/libpistache.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
