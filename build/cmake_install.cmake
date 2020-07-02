# Install script for directory: E:/3 micropython/2 micropython-esp32/mkfatfs

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/mkfatfs")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("E:/3 micropython/2 micropython-esp32/mkfatfs/build/esp_idf/cmake_install.cmake")
  include("E:/3 micropython/2 micropython-esp32/mkfatfs/build/filesystem/diskio/cmake_install.cmake")
  include("E:/3 micropython/2 micropython-esp32/mkfatfs/build/filesystem/fatfs/cmake_install.cmake")
  include("E:/3 micropython/2 micropython-esp32/mkfatfs/build/filesystem/vfs/cmake_install.cmake")
  include("E:/3 micropython/2 micropython-esp32/mkfatfs/build/esp_idf/freertos/cmake_install.cmake")

endif()
