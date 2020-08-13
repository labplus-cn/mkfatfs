#pragma once

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <time.h>
#include <memory>
#include <cstdlib>
#include <stdio.h>
#if defined(_WIN32)
    #include <io.h>
#endif
#include <dirent.h>
#include "lfs2.h"
#include "lfs2_rambd.h"

#define READ_SIZE 32
#define PROC_SIZE 32
#define LOOKAHEAD_SIZE 32
#define BLOCK_SIZE 4096
#define BLOCK_COUNT 512

/**
* @brief This class is used to pack/upack filesystem.
*
*/
class Pack_littlefs
{
public:
    Pack_littlefs();
    ~Pack_littlefs();
    int actionPack(std::string s_dirName, std::string s_imageName, int s_imageSize);
    int actionUnpack(std::string s_imageName, std::string s_dirName,int s_imageSize);

private:
    void   lfsConfig(int s_imageSize);
    bool   fsMount(std::string s_imageName, int s_imageSize, bool format=false);
    bool   fsUnmount();
    size_t getFileSize(FILE* fp);
    bool   dirExists(const char* path);
    bool   dirCreate(const char* path);
    int    parkFileToRamFS(char* path_src, const char* path_des);
    bool   parkFilesToRamFS(const char* dirSrc, const char* dirDes);
    bool   unparkFileFromRamFS(const char* path_src, lfs2_info *littlefsFile, const char* path_des);
    bool   unparkFilesFromRamFS(const char* dirSrc, std::string dirDes);

    lfs2_t s_fs;
    lfs2_config s_cfg;
    lfs2_file_t f;
    lfs2_rambd_config rambd_cfg = {
        .erase_value = -1,
        // .buffer = &g_flashmem[0],
    };
    lfs2_rambd_t bd = {
        .cfg = &rambd_cfg
    };

protected:

};




