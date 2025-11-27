# Install script for directory: /home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core

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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-core-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-core-default.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-core-default.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/build/lib/libns3-dev-core-default.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-core-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-core-default.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3-dev-core-default.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ns3" TYPE FILE FILES
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/realtime-simulator-impl.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/wall-clock-synchronizer.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/int64x64-128.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/helper/csv-reader.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/helper/event-garbage-collector.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/helper/random-variable-stream-helper.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/abort.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/ascii-file.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/ascii-test.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/assert.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/attribute-accessor-helper.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/attribute-construction-list.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/attribute-container-accessor-helper.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/attribute-container.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/attribute-helper.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/attribute.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/boolean.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/breakpoint.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/build-profile.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/calendar-scheduler.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/callback.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/command-line.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/config.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/default-deleter.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/default-simulator-impl.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/deprecated.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/des-metrics.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/double.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/empty.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/enum.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/event-id.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/event-impl.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/fatal-error.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/fatal-impl.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/global-value.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/hash-fnv.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/hash-function.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/hash-murmur3.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/hash.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/heap-scheduler.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/int-to-type.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/int64x64-double.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/int64x64.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/integer.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/length.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/list-scheduler.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/log-macros-disabled.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/log-macros-enabled.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/log.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/make-event.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/map-scheduler.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/math.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/names.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/node-printer.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/nstime.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/object-base.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/object-factory.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/object-map.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/object-ptr-container.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/object-vector.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/object.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/pair.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/pointer.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/priority-queue-scheduler.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/ptr.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/random-variable-stream.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/ref-count-base.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/rng-seed-manager.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/rng-stream.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/scheduler.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/show-progress.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/simple-ref-count.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/simulation-singleton.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/simulator-impl.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/simulator.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/singleton.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/string.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/synchronizer.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/system-path.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/system-wall-clock-ms.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/system-wall-clock-timestamp.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/test.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/time-printer.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/timer-impl.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/timer.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/trace-source-accessor.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/traced-callback.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/traced-value.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/trickle-timer.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/tuple.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/type-id.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/type-name.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/type-traits.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/uinteger.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/unix-fd-reader.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/unused.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/valgrind.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/vector.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/src/core/model/watchdog.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/build/include/ns3/config-store-config.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/build/include/ns3/core-config.h"
    "/home/ed-5355-1/DAT295/autonomous-vehicles/van3t/ns-3-dev/build/include/ns3/core-module.h"
    )
endif()

