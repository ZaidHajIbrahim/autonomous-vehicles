#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Proton::qpid-proton" for configuration "RelWithDebInfo"
set_property(TARGET Proton::qpid-proton APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Proton::qpid-proton PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libqpid-proton.so.11.18.0"
  IMPORTED_SONAME_RELWITHDEBINFO "libqpid-proton.so.11"
  )

list(APPEND _IMPORT_CHECK_TARGETS Proton::qpid-proton )
list(APPEND _IMPORT_CHECK_FILES_FOR_Proton::qpid-proton "${_IMPORT_PREFIX}/lib/libqpid-proton.so.11.18.0" )

# Import target "Proton::core" for configuration "RelWithDebInfo"
set_property(TARGET Proton::core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Proton::core PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libqpid-proton-core.so.10.15.0"
  IMPORTED_SONAME_RELWITHDEBINFO "libqpid-proton-core.so.10"
  )

list(APPEND _IMPORT_CHECK_TARGETS Proton::core )
list(APPEND _IMPORT_CHECK_FILES_FOR_Proton::core "${_IMPORT_PREFIX}/lib/libqpid-proton-core.so.10.15.0" )

# Import target "Proton::proactor" for configuration "RelWithDebInfo"
set_property(TARGET Proton::proactor APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Proton::proactor PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libqpid-proton-proactor.so.1.10.1"
  IMPORTED_SONAME_RELWITHDEBINFO "libqpid-proton-proactor.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS Proton::proactor )
list(APPEND _IMPORT_CHECK_FILES_FOR_Proton::proactor "${_IMPORT_PREFIX}/lib/libqpid-proton-proactor.so.1.10.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
