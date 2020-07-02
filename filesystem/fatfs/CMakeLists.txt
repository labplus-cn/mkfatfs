# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)

include_directories(include)
include_directories(../../../esp_idf/include)
include_directories(../../../esp_idf/freertos/include)

# 生成链接库
add_library (fatfs ${DIR_LIB_SRCS})
