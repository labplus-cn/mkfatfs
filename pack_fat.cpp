//
//  pack.cpp
//
//  Created by zhao hui jiang on 07/03/2020.
//  Copyright (c). All rights reserved.
//
#include "pack_fat.h"

static const char *BASE_PATH = "/fatfs";
static RAM_handle_t s_ram_handle;
static FATFS* s_fs = NULL;

struct dirent_1 {
    int d_ino;          /*!< file number */
    uint8_t d_type;     /*!< not defined in POSIX, but present in BSD and Linux */
    uint8_t no_use[3];
// #define DT_UNKNOWN  0
// #define DT_REG      1
// #define DT_DIR      2
    char d_name[256];   /*!< zero-terminated file name */
}dirent_t;

Pack_fat::Pack_fat()
{}
Pack_fat::~Pack_fat()
{}

/**
 * @brief Mount RAM fat filesystem(esp32's filesystem) to "/fatfs". so we can use vfs read/write it.
 *      mount to "/spiflash"，size：s_imageSize, return: s_ram_handle s_fs
 * @param 
 * @return True or false.
 */
bool Pack_fat::fsMount(int s_imageSize) {
  bool result;
  esp_vfs_fat_mount_config_t mountConfig;
  mountConfig.max_files = 4; //Max files.
  mountConfig.format_if_mount_failed = true;
  result = (ESP_OK == emulate_esp_vfs_fat_spiflash_mount(BASE_PATH, &mountConfig, &s_ram_handle, &s_fs, s_imageSize));

  return result;
}


/**
 * @brief Unount RAM fat filesystem(esp32's filesystem).
 * @param 
 * @return True or false.
 */
bool Pack_fat::fsUnmount() {
  bool result;

  result = (ESP_OK == emulate_esp_vfs_fat_spiflash_unmount(BASE_PATH, s_ram_handle));

  if (result) {
    if (g_debugLevel > 0) {
      std::cout << "Unmounted successfully" << std::endl;
    }
  } else {
    std::cerr << "Unmount failed" << std::endl;
  }

  return result;
}

size_t Pack_fat::getFileSize(FILE* fp)
{
    fseek(fp, 0L, SEEK_END);
    size_t size = (size_t) ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return size;
}

/**
 * @brief Check if directory exists.
 * @param path Directory path.
 * @return True if exists otherwise false.
 *
 */
bool Pack_fat::dirExists(const char* path) {
    DIR *d = opendir(path);

    if (d) {
        closedir(d);
        return true;
    }

    return false;
}

/**
 * @brief Create directory if it not exists.
 * @param path Directory path.
 * @return True or false.
 */
bool Pack_fat::dirCreate(const char* path) {
    // Check if directory also exists.
    if (dirExists(path)) {
	    return false;
    }

    // platform stuff...
    #if defined(_WIN32)
        if (mkdir(path) != 0) {
    #else
        if (mkdir(path, S_IRWXU | S_IXGRP | S_IRGRP | S_IROTH | S_IXOTH) != 0) {
    #endif
            std::cerr << "Can not create directory!!!" << std::endl;
            return false;
        }

    return true;
}

/**
 * @brief Create directory in RAM fatfs.
 * @param path Directory name, full path.
 * @return True or false.
 */
int Pack_fat::parkDirToRamFS(const char* name) {
    std::string fileName = name;
    fileName += "/.";

    if (g_debugLevel > 0) {
      std::cout << "creating dir: " << fileName << std::endl;
    }

    std::string nameInFat = BASE_PATH;
    nameInFat += name;
    int res = emulate_vfs_mkdir(nameInFat.c_str(), O_CREAT);
    if (res < 0) {
      std::cerr << "failed to create dir" << std::endl;
    }
    return 0;
}

/**
 * @brief Create file in RAM fatfs.
 * @param path_src source Directory path, full path.
 * @param path_des des Directory path, full path.
 * @return True or false.
 */
int Pack_fat::parkFileToRamFS(char* path_src, const char* path_des) {
    FILE* f_src = fopen(path_src, "rb"); //open file in pc.
    if (!f_src) {
        std::cerr << "error: failed to open " << path_src << " for reading" << std::endl;
        return 1;
    }

    std::string nameInFat = path_des;
    const int flags = O_CREAT | O_TRUNC | O_RDWR;
    int fd = emulate_esp_vfs_open(nameInFat.c_str(), flags, 0); //if not exist, create it.
    if (fd < 0) {
        std::cerr << "error: failed to open \"" << nameInFat << "\" for writing" << std::endl;
        return 0; //0 does not stop copying files
    }

    // read src file size
    fseek(f_src, 0, SEEK_END);
    size_t size = ftell(f_src);
    fseek(f_src, 0, SEEK_SET);

    if (g_debugLevel > 0) {
        std::cout << "file size: " << size << std::endl;
    }

    size_t left = size;
    uint8_t data_byte;
    while (left > 0){
        if (1 != fread(&data_byte, 1, 1, f_src)) {
            std::cerr << "fread error!" << std::endl;
            fclose(f_src);
            emulate_esp_vfs_close(fd);
            return 1;
        }
        ssize_t res = emulate_esp_vfs_write(fd, &data_byte, 1);
        if (res < 0) {
            std::cerr << "esp_vfs_write() error" << std::endl;
            if (g_debugLevel > 0) {
                std::cout << "data left: " << left << std::endl;
            }
            fclose(f_src);
            emulate_esp_vfs_close(fd);
            return 1;
        }
        left -= 1;
    }

    emulate_esp_vfs_close(fd);

    fclose(f_src);

    return 0;
}

/**
 * @brief copy directorys/files in pc to vfs fat filesystem(esp32)
 * @param dirSrc Directory path in pc.
 * @param dirDes root Directory path of vfs fat(/fatfs).
 * @return True if exists otherwise false.
 *
 */
bool Pack_fat::parkFilesToRamFS(const char* dirSrc, const char* dirDes)
{  
    DIR *dir;
    bool error = false;

    intptr_t handle;
    std::string dir_root;
    std::string dir_pc = dirSrc;
    std::string dir_RAM_fs = dirDes;
    std::string dir_full_path_s;
    std::string dir_full_path_d;

    // Open directory
    if ((dir = opendir (dir_pc.c_str())) != NULL) 
    {
        closedir (dir);
    }else{
        std::cerr << "warning: can't read source directory: \"" << dir_pc.c_str() << "\"" << std::endl;
        return error;
    }
    
#if defined(_WIN32)
    _finddata_t findData;
    dir_root = dir_pc + "\\*.*";
    handle = _findfirst(dir_root.c_str(), &findData);
    if (handle == -1)    
        return error;
    do
    {
        dir_full_path_s = dir_pc + "\\" + findData.name;
        dir_full_path_d = dir_RAM_fs + "/" + findData.name;
        if (findData.attrib & _A_SUBDIR) //目录
        {
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
                continue;

            std::cout << "\n" << "dir_RAM_fs: "<< dir_RAM_fs.c_str() << "  dir_pc: "<< dir_pc.c_str() << std::endl;
            std::cout << "Sub dir: "<< findData.name << "  full dir_full_path_s: "<< dir_full_path_s.c_str() << "   full dir_full_path_d: "<< dir_full_path_d.c_str() << "\n" << std::endl;
            emulate_vfs_mkdir(dir_full_path_d.c_str(), 1);
            parkFilesToRamFS(dir_full_path_s.c_str(), dir_full_path_d.c_str()); 
        }
        else 
        {
            // Add File to image.
            if (parkFileToRamFS((char*)dir_full_path_s.c_str(), dir_full_path_d.c_str()) != 0) {
                std::cerr << "error adding file!" << "\n" << std::endl;
                error = true;
                if (g_debugLevel > 0) {
                    std::cout << std::endl;
                }
                break;
            }
            std::cout << "Pack file, dir_full_path_s: " << dir_full_path_s.c_str() << "dir_full_path_d: "<< dir_full_path_d.c_str() << "  file name: "<< findData.name << std::endl;
        }

    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    
#else
    struct dirent *ent;
    dir_root = dir_pc + "/";

    // Open directory
    if ((dir = opendir (dir_root.c_str())) != NULL) {

        // Read files from directory.
        while ((ent = readdir (dir)) != NULL) 
        {
            dir_full_path_s = dir_pc + "/" +  ent->d_name;
            dir_full_path_d = dir_RAM_fs + "/" +  ent->d_name;

            if (ent->d_name[0] == '.') // Ignore dir itself.			
                continue;            	

            std::cout << "\n" << "dir_RAM_fs: "<< dir_RAM_fs.c_str() << "  dir_pc: "<< dir_pc.c_str() << std::endl;
            std::cout << "Sub dir: "<< ent->d_name << "  full dir_full_path_s: "<< dir_full_path_s.c_str() << "   full dir_full_path_d: "<< dir_full_path_d.c_str() << "\n" << std::endl;
           
            struct stat path_stat;
            stat (dir_full_path_s.c_str(), &path_stat);

            if (!S_ISREG(path_stat.st_mode)) {
                if (S_ISDIR(path_stat.st_mode)) { // Check if path is a directory.
                    emulate_vfs_mkdir(dir_full_path_d.c_str(), 1);  //create directory in vfs fat filesystem.
                    parkFilesToRamFS(dir_full_path_s.c_str(), dir_full_path_d.c_str()); 
                }
                else
                {
                    std::cerr << "skipping " << ent->d_name << std::endl;
                    continue;
                }
            }

            // Add File to image.
            if (parkFileToRamFS((char*)dir_full_path_s.c_str(), dir_full_path_d.c_str()) != 0) {
                std::cerr << "error adding file!" << std::endl;
                error = true;
                if (g_debugLevel > 0) {
                    std::cout << std::endl;
                }
                break;
            }
        } // end while
        closedir (dir);
    } 
#endif 
    return error;
}

/**
 * @brief ccpy all directorys/files from RAM fat filesystem to pc.
 * @param path_src: source file, in RAM file system
 * @param path_des: des file, to pc
 * @return True or false.
 */
int Pack_fat::unparkFileFromRamFS(const char* path_src, const char* path_des) 
{
    std::vector<uint8_t> temp_buf;

    const int flags = O_RDONLY; 
    int f_src = emulate_esp_vfs_open(path_src, flags, 0 ); //源文件
    if (f_src < 0) {
        std::cerr << "error: failed to open" << path_src << "\" for writing" << std::endl;
        return 0; //0 does not stop copying files
    }

    FILE* f_des = fopen(path_des, "wb");
    if (!f_des) {
        std::cerr << "error: failed to open " << path_des << " for reading" << std::endl;
        return 1;
    }

    size_t size = emulate_esp_vfs_lseek(f_src, 0, SEEK_END);
    emulate_esp_vfs_lseek(f_src, 0, SEEK_SET);
    std::cout << "file size: " << size << std::endl;
    if (g_debugLevel > 0) {
        std::cout << "file size: " << size << std::endl;
    }

    temp_buf.resize(size);
    emulate_esp_vfs_read(f_src, (void *)&temp_buf[0], size);
    fwrite ( (const void *)&temp_buf[0], 1, size, f_des );

    emulate_esp_vfs_close(f_src);
    fclose(f_des);

    return 0;
}

/**
 * @brief ccpy all directorys/files from RAM fat filesystem to pc.
 * @param dirSrc directory/file in RAM fatfs.
 *  @param dirDes directory/file in pc.
 * @return True or false.
 */
bool Pack_fat::unparkFilesFromRamFS(const char* dirSrc, const char* dirDes)
{
    DIR *dir;
    struct dirent_1 *ent;
    bool error = false;
    std::string dir_pc = dirDes;
    std::string dir_RAM_fs = dirSrc;
    std::string dir_full_path_s;
    std::string dir_full_path_d;
    std::cout << "dir_RAM_fs: "<< dir_RAM_fs.c_str() << "  dir_pc: "<< dir_pc.c_str() << std::endl;

    // Open directory
    if ((dir = emulate_vfs_opendir(dir_RAM_fs.c_str())) != NULL) {

        // Read files from directory.
        while ((ent = (struct dirent_1 *)emulate_vfs_readdir (dir)) != NULL) { //处理目录中的每一项
            // Ignore dir itself.         	
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            {
                continue;
            }
               
            std::cout << "ent->d_name: "<< ent->d_name  << std::endl;
            dir_full_path_s = dir_RAM_fs + "/" + ent->d_name;
            #if defined(_WIN32)
                dir_full_path_d = dir_pc + "\\" + ent->d_name;
            #else
                dir_full_path_d = dir_pc + "/" + ent->d_name;
            #endif
            std::cout << "RAM dir: "<< dir_full_path_s.c_str() << std::endl;
            struct stat path_stat;
            emulate_esp_vfs_stat (dir_full_path_s.c_str(), &path_stat); //get file info.
            std::cout << "file mode: "<< path_stat.st_mode << std::endl;

            #if defined(_WIN32)
                if (S_ISDIR(path_stat.st_mode)) 
            #else
                if (path_stat.st_mode & 0x4000) 
            #endif
            {
                std::cout << "Sub dir: "<< ent->d_name << "  full dir_full_path_s: "<< dir_full_path_s.c_str() << "   full dir_full_path_d: "<< dir_full_path_d.c_str() << "\n" << std::endl;
                dirCreate(dir_full_path_d.c_str());
                if (unparkFilesFromRamFS(dir_full_path_s.c_str(), dir_full_path_d.c_str()) != 0)
                {
                    std::cerr << "Error for unpark content from " << ent->d_name << "!" << std::endl;
                }
            }
            #if defined(_WIN32)
                else if (S_ISREG(path_stat.st_mode)) //normal file
            #else
                else if (path_stat.st_mode & 0x8000) 
            #endif          
            { 
                // Add File to image.
                std::cerr << "unpark file......!" << std::endl;
                if (unparkFileFromRamFS(dir_full_path_s.c_str(), dir_full_path_d.c_str()) != 0) {
                    std::cerr << "error unpark file!" << std::endl;
                    error = true;
                    if (g_debugLevel > 0) {
                        std::cout << std::endl;
                    }
                    break;
                }
            }
        } // end while
        emulate_vfs_closedir (dir);
    }
    else {
        std::cerr << "warning: can't read source directory: \"" << dir_RAM_fs.c_str() << "\"" << std::endl;
        return 1;
    }

    return (error) ? 1 : 0;
}

/**
 * @brief ccpy all directory/file to esp32's fat filesystem.
 * @param 
 * @return True or false.
 */
int Pack_fat::actionPack(std::string s_dirName, std::string s_imageName, int s_imageSize) {
    int ret = 0; //0 - ok

    // 1. resize g_flashmem and fill 0xff, it will used for RAM fat filesystem.
    g_flashmem.resize(s_imageSize, 0xff);

    // 2. mount g_flashmem(in RAM) as a fat filesystem, mount point is BASE_PATH(root directory of the RAM filesystem). 
    if (fsMount(s_imageSize)) { 
      if (g_debugLevel > 0) {
        std::cout << "Mounted successfully" << std::endl;
      }
    } else {
      std::cerr << "Mount failed" << std::endl;
      return 1;
    }  

    // 3. copy all directorys/files to the RAM fat filesystem.
    ret = parkFilesToRamFS(s_dirName.c_str(), BASE_PATH);

    // 4. unmount the RAM fat filesystem.
    fsUnmount();

    // 5. open *.bin file witch read from esp32. 
    FILE* fdres = fopen(s_imageName.c_str(), "wb"); 
    if (!fdres) {
        std::cerr << "error: failed to open image file" << std::endl;
        return 1;
    }

    // 6. copy all data in g_flashmem to *.bin file.
    std::cout << "g_flashmem[0]: " << g_flashmem[0] << "size: " << g_flashmem.size() << std::endl;
    fwrite(&g_flashmem[0], 4, g_flashmem.size()/4, fdres); //write all data in RAM fatfs to *.bin.
    fclose(fdres);

    if (g_debugLevel > 0) {
      std::cout << "Image file is written to \"" << s_imageName << "\"" << std::endl;
    }

    return ret;
}

/**
 * @brief Unpack action.
 *  unpack all the directorys/files in *bin(read from esp32) to a pc directory
 * @param s_imageName *.bin file name.
 * @param s_imageSize *.bin file's size.
 * @param s_dirName directory in pc, we will read all files to it.
 * @return 0 success, 1 error
 */
int Pack_fat::actionUnpack(std::string s_imageName, std::string s_dirName,int s_imageSize)
{
    int ret = 0;

    // 1. open fatfs image file(bin) 
    FILE* f_src = fopen(s_imageName.c_str(), "rb");
    if (!f_src) {
        std::cerr << "error: failed to open image file" << std::endl;
        return 1;
    }

    if (s_imageSize == 0) {
        s_imageSize = getFileSize(f_src); 
    }
    g_flashmem.resize(s_imageSize, 0xff); //set RAM fatfs size

    // 2. read content in *.bin into g_flashmem, --> to RAM file system, like memcopy.
    size_t sz = fread(&g_flashmem[0], 4, g_flashmem.size() / 4, f_src); //read *.bin to RAM fatfs.
    // close file handle
    fclose(f_src);

    // 3. mount RAM(g_flashmem) file system， so we can use vfs read file in RAM file system.
    if (fsMount(s_imageSize)) {
      if (g_debugLevel > 0) {
        std::cout << "Mounted successfully" << std::endl;
      }
    } else {
      std::cerr << "Mount failed" << std::endl;
      return 1;
    } 
    // std::cout << "g_flashmem: "<< g_flashmem[3] << std::endl; //test
    // 4. fine dir_pc dir, if no create it.
    dirCreate(s_dirName.c_str());

    // 5. unpack files,s_dirName:RAM fatfs --> pc
    if (!unparkFilesFromRamFS(BASE_PATH, s_dirName.c_str())) {
        ret = 1;
    }

    // unmount file system
    fsUnmount();

    return ret;
}

