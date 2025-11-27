# Install script for directory: /home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/c

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton.so.11.18.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton.so.11"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton.so.11.18.0"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton.so.11"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton.so.11.18.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton.so.11"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-core.so.10.15.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-core.so.10"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton-core.so.10.15.0"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton-core.so.10"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-core.so.10.15.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-core.so.10"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-core.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-core.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-core.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton-core.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-core.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-core.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-core.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-proactor.so.1.10.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-proactor.so.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton-proactor.so.1.10.1"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton-proactor.so.1"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-proactor.so.1.10.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-proactor.so.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c:"
           NEW_RPATH "")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-proactor.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-proactor.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-proactor.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton-proactor.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-proactor.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-proactor.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-proactor.so"
         OLD_RPATH "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqpid-proton-proactor.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/proton" TYPE FILE FILES
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/annotations.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/cid.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/codec.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/condition.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/connection.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/connection_driver.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/cproton.i"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/delivery.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/disposition.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/engine.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/error.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/event.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/handlers.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/import_export.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/link.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/listener.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/log.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/logger.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/message.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/messenger.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/netaddr.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/object.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/proactor.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/raw_connection.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/reactor.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/sasl.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/sasl_plugin.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/selectable.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/session.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/ssl.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/terminus.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/tls.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/transport.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/type_compat.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/types.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/url.h"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/include/proton/version.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton-core.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/libqpid-proton-proactor.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Proton/ProtonTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Proton/ProtonTargets.cmake"
         "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/CMakeFiles/Export/lib/cmake/Proton/ProtonTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Proton/ProtonTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Proton/ProtonTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Proton" TYPE FILE FILES "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/CMakeFiles/Export/lib/cmake/Proton/ProtonTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Proton" TYPE FILE FILES "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/CMakeFiles/Export/lib/cmake/Proton/ProtonTargets-relwithdebinfo.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Proton" TYPE FILE FILES
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/ProtonConfig.cmake"
    "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/ProtonConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Proton" TYPE FILE FILES "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/c/../tools/cmake/Modules/FindCyrusSASL.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/docs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/examples/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/tests/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/build/c/tools/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/proton/examples/c" TYPE DIRECTORY FILES "/home/ed-5355-3/DAT295/autonomous-vehicles/qpid-proton-0.40.0/c/examples/" USE_SOURCE_PERMISSIONS REGEX "/ProtonConfig\\.cmake$" EXCLUDE REGEX "/testme$" EXCLUDE)
endif()

