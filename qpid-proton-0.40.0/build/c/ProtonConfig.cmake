#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

# Name: Proton
# Description: Qpid Proton C library
# Version: 0.40.0
# URL: http://qpid.apache.org/proton/


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was ProtonConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

# Use original install prefix when loaded through a "/usr move"
# cross-prefix symbolic link such as /lib -> /usr/lib.
get_filename_component(_realCurr "${CMAKE_CURRENT_LIST_DIR}" REALPATH)
get_filename_component(_realOrig "/usr/lib/cmake/Proton" REALPATH)
if(_realCurr STREQUAL _realOrig)
  set(PACKAGE_PREFIX_DIR "/usr")
endif()
unset(_realOrig)
unset(_realCurr)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################
if (NOT Proton_USE_STATIC_LIBS)
  include("${CMAKE_CURRENT_LIST_DIR}/ProtonTargets.cmake")
else()
  include("${CMAKE_CURRENT_LIST_DIR}/ProtonTargetsStatic.cmake")
endif()

set(Proton_VERSION 0.40.0)

# find dependencies, because static libs don't transitively pull them
if (Proton_USE_STATIC_LIBS)
    set(CMAKE_MODULE_PATH_OLD ${CMAKE_MODULE_PATH})
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

    set(CyrusSASL_FOUND TRUE)
    if (CyrusSASL_FOUND)
        find_package (CyrusSASL REQUIRED)
    endif()

    set(OPENSSL_FOUND TRUE)
    set(OpenSSL_FOUND TRUE)
    if (OPENSSL_FOUND OR OpenSSL_FOUND)
        find_package (OpenSSL REQUIRED)
    endif()

    find_package (Threads REQUIRED)

    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH_OLD})
endif()

set_and_check (Proton_INCLUDE_DIRS ${PACKAGE_PREFIX_DIR}/include)
set (Proton_LIBRARIES Proton::qpid-proton)
set (Proton_FOUND True)

set_and_check (Proton_Core_INCLUDE_DIRS ${PACKAGE_PREFIX_DIR}/include)
set (Proton_Core_LIBRARIES Proton::core)
set (Proton_Core_FOUND True)

set (HAS_PROACTOR True)
if (HAS_PROACTOR)
  set_and_check (Proton_Proactor_INCLUDE_DIRS ${PACKAGE_PREFIX_DIR}/include)
  set (Proton_Proactor_LIBRARIES Proton::proactor)
  set (Proton_Proactor_FOUND True)
endif()

set (HAS_TLS )
if (HAS_TLS)
  set_and_check (Proton_Tls_INCLUDE_DIRS ${PACKAGE_PREFIX_DIR}/include)
  set (Proton_Tls_LIBRARIES Proton::tls)
  set (Proton_Tls_FOUND True)
endif()

function(warn_components_not_found _NAME)
  if (${_NAME}_FIND_QUIETLY)
    return()
  endif()

  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      MESSAGE(STATUS "Requested ${_NAME} component ${comp} is NOT FOUND")
    endif()
  endforeach()
endfunction()

check_required_components(Proton)
warn_components_not_found(Proton)
