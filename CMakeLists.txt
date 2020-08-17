cmake_minimum_required(VERSION 3.0.0)
project(mkfatfs VERSION 2.0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(Config.h.in Config.h)

include(CTest)
enable_testing()

include_directories(.)
include_directories(esp_idf/include)
include_directories(esp_idf/freertos/include)
include_directories(filesystem/diskio/include)
include_directories(filesystem/fatfs/include)
include_directories(filesystem/vfs/include)
include_directories(tclap)
include_directories(filesystem/littlefs/littlefs)
include_directories(filesystem/littlefs/littlefs/bd)

aux_source_directory(. DIR_SRCS)
aux_source_directory(esp_idf DIR_ESP_IDF)
aux_source_directory(esp_idf/freertos DIR_RTOS)
aux_source_directory(filesystem/diskio DIR_DISKIO)
aux_source_directory(filesystem/fatfs DIR_FATFS)
aux_source_directory(filesystem/vfs DIR_VFS)
aux_source_directory(filesystem/littlefs/littlefs DIR_LITTLEFS)
set(SRC_RAMBD filesystem/littlefs/littlefs/bd/lfs2_rambd.c )

add_executable(mkfatfs ${DIR_SRCS} ${DIR_ESP_IDF} ${DIR_RTOS} ${DIR_DISKIO} ${DIR_FATFS} ${DIR_VFS} ${DIR_LITTLEFS} ${SRC_RAMBD})

# add_subdirectory(esp_idf)
# add_subdirectory(filesystem/diskio)
# add_subdirectory(filesystem/fatfs)
# add_subdirectory(filesystem/vfs)
# add_subdirectory(esp_idf/freertos)

# add_executable(mkfatfs main.cpp pack.cpp)

# target_link_libraries(mkfatfs -Wl,--start-group esp-idf vfs diskio fatfs freertos -Wl,--end-group -Wl,--hash-style=sysv)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(mkfatfs PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
