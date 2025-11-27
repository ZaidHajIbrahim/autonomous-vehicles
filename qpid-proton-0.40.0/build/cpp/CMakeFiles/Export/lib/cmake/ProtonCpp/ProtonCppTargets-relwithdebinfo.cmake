#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Proton::cpp" for configuration "RelWithDebInfo"
set_property(TARGET Proton::cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Proton::cpp PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "Proton::core;Proton::proactor"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libqpid-proton-cpp.so.12.13.1"
  IMPORTED_SONAME_RELWITHDEBINFO "libqpid-proton-cpp.so.12"
  )

list(APPEND _IMPORT_CHECK_TARGETS Proton::cpp )
list(APPEND _IMPORT_CHECK_FILES_FOR_Proton::cpp "${_IMPORT_PREFIX}/lib/libqpid-proton-cpp.so.12.13.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
