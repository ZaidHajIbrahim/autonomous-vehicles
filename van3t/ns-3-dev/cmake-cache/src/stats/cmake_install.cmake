# Install script for directory: /home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "default")
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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-stats-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-stats-default.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-stats-default.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/build/lib/libns3-dev-stats-default.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-stats-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-stats-default.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-stats-default.so"
         OLD_RPATH "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-stats-default.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ns3" TYPE FILE FILES
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/sqlite-data-output.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/sqlite-output.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/helper/file-helper.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/helper/gnuplot-helper.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/average.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/basic-data-calculators.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/boolean-probe.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/data-calculator.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/data-collection-object.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/data-collector.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/data-output-interface.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/double-probe.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/file-aggregator.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/get-wildcard-matches.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/gnuplot-aggregator.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/gnuplot.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/histogram.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/omnet-data-output.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/probe.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/stats.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/time-data-calculators.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/time-probe.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/time-series-adaptor.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/uinteger-16-probe.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/uinteger-32-probe.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/stats/model/uinteger-8-probe.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/build/include/ns3/stats-module.h"
    )
endif()

