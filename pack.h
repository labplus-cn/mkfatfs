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
#include "esp_vfs_fat.h"
#include "user_vfs.h"
#include "esp_err.h"
#include "esp_log.h"
#include "FatPartition.h"

/**
* @brief This class is used to pack/upack filesystem.
*
*/
class Pack
{
public:
    Pack();
    ~Pack();
    int actionPack(std::string s_dirName, std::string s_imageName, int s_imageSize);
    int actionUnpack(std::string s_imageName, std::string s_dirName,int s_imageSize);

private:
    bool   fatfsMount(int s_imageSize);
    bool   fatfsUnmount();
    size_t getFileSize(FILE* fp);
    bool   dirExists(const char* path);
    bool   dirCreate(const char* path);
    int    parkDirToRamFS(const char* name);
    int    parkFileToRamFS(char* path_src, const char* path_des);
    bool   parkFilesToRamFS(const char* dirSrc, const char* dirDes);
    int    unparkFileFromRamFS(const char* path_src, const char* path_des);
    bool   unparkFilesFromRamFS(const char* dirSrc, const char* dirDes);

protected:

};




