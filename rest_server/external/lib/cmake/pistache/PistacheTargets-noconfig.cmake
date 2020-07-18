#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "pistache_shared" for configuration ""
set_property(TARGET pistache_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(pistache_shared PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libpistache.so.0.0.002-git20200708"
  IMPORTED_SONAME_NOCONFIG "libpistache.so.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS pistache_shared )
list(APPEND _IMPORT_CHECK_FILES_FOR_pistache_shared "${_IMPORT_PREFIX}/lib/libpistache.so.0.0.002-git20200708" )

# Import target "pistache_static" for configuration ""
set_property(TARGET pistache_static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(pistache_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libpistache.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS pistache_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_pistache_static "${_IMPORT_PREFIX}/lib/libpistache.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)