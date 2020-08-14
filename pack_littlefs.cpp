//
//  pack.cpp
//
//  Created by zhao hui jiang on 07/03/2020.
//  Copyright (c). All rights reserved.
//
#include "pack_littlefs.h"
#include "FatPartition.h"

extern int g_debugLevel;

Pack_littlefs::Pack_littlefs()
{
    rambd_cfg.erase_value = -1;
    bd.cfg = &rambd_cfg;
}
Pack_littlefs::~Pack_littlefs()
{}

void Pack_littlefs::lfsConfig(int s_imageSize)
{
  memset(&s_fs, 0, sizeof(s_fs));
  memset(&s_cfg, 0, sizeof(s_cfg));

  rambd_cfg.buffer = &g_flashmem[0]; //(uint8_t *)lfs2_malloc(BLOCK_SIZE * BLOCK_COUNT);
  bd.buffer = (uint8_t *)rambd_cfg.buffer;
  s_cfg.context = &bd;
  s_cfg.read  = lfs2_rambd_read;
  s_cfg.prog  = lfs2_rambd_prog;
  s_cfg.erase = lfs2_rambd_erase;
  s_cfg.sync  = lfs2_rambd_sync;

  s_cfg.read_size = READ_SIZE;
  s_cfg.prog_size = PROC_SIZE;
  s_cfg.block_size =  BLOCK_SIZE;
  s_cfg.block_count = s_imageSize/BLOCK_SIZE;
  s_cfg.block_cycles = 100; // TODO - need better explanation
  s_cfg.cache_size = 4 * lfs2_max(s_cfg.read_size, s_cfg.prog_size);
  s_cfg.lookahead_size = LOOKAHEAD_SIZE;
  s_cfg.read_buffer = new uint8_t[s_cfg.cache_size];
  s_cfg.prog_buffer = new uint8_t[s_cfg.cache_size];
  s_cfg.lookahead_buffer = new uint8_t[s_cfg.lookahead_size];
}

/**
 * @brief Mount RAM littlefs filesystem(esp32's filesystem) to "/littlefsfs". so we can use vfs read/write it.
 *      mount to "/spiflash"，size：s_imageSize, return: s_ram_handle s_fs
 * @param 
 * @return True or false.
 */
bool Pack_littlefs::fsMount(std::string s_imageName, int s_imageSize, bool format) {
    size_t size = s_imageSize;

    if(s_imageSize == 0)
    {
        FILE* f_src = fopen(s_imageName.c_str(), "rb");
        if (!f_src) {
            std::cerr << "error: failed to open image file" << std::endl;
            return false;
        }

        fseek(f_src, 0L, SEEK_END);
        size = (size_t) ftell(f_src);

        fseek(f_src, 0L, SEEK_SET);
        g_flashmem.resize(size, 0xff); //set RAM littlefsfs size
        size_t sz = fread(&g_flashmem[0], 4, g_flashmem.size() / 4, f_src); //read *.bin to RAM littlefsfs.
        fclose(f_src);
    }
    else
    {
        g_flashmem.resize(size, 0xff); //set RAM littlefsfs size
    }
    
        
    lfsConfig(size);

    if(format)
        lfs2_format(&s_fs, &s_cfg);
    lfs2_mount(&s_fs, &s_cfg);

    // test
    //     int ret;
    //     uint8_t data1[100];
    //   ret = lfs2_file_open(&s_fs, &f, "main.py", LFS2_O_RDONLY);
    //   ret = lfs2_file_read(&s_fs, &f, data1, 100);
    //   lfs2_file_close(&s_fs, &f);

    //   for( int i = 0; i<100; i++)
    //    std::cout << data1[i] <<std::endl;

    // for(int i=0; i<16; i++)
    //     std::cout << "data: " << g_flashmem[i] << std::endl;
    return true;
}


/**
 * @brief Unount RAM littlefs filesystem(esp32's filesystem).
 * @param 
 * @return True or false.
 */
bool Pack_littlefs::fsUnmount() {
    if (lfs2_unmount(&s_fs) == 0) {
        if (g_debugLevel > 0) {
            std::cout << "Unmounted successfully" << std::endl;
        }
    } else {
        std::cerr << "Unmount failed" << std::endl;
        return false;
    }

  return true;
}

size_t Pack_littlefs::getFileSize(FILE* fp)
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
bool Pack_littlefs::dirExists(const char* path) {
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
bool Pack_littlefs::dirCreate(const char* path) {
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
 * @brief Create file in RAM littlefsfs.
 * @param path_src source Directory path, full path.
 * @param path_des des Directory path, full path.
 * @return True or false.
 */
int Pack_littlefs::parkFileToRamFS(char* path_src, const char* path_des) {
    FILE* f_src = fopen(path_src, "rb"); //open file in pc.
    if (!f_src) {
        std::cerr << "error: failed to open " << path_src << " for reading" << std::endl;
        return 1;
    }

    lfs2_file_t dst;
    int ret = lfs2_file_open(&s_fs, &dst, path_des, LFS2_O_CREAT | LFS2_O_TRUNC | LFS2_O_WRONLY);
    if (ret < 0) {
        std::cerr << "unable to open '" << path_des << "." << std::endl;
        return 1;
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
            lfs2_file_close(&s_fs, &dst);
            return 1;
        }
        int res = lfs2_file_write(&s_fs, &dst, &data_byte, 1);
        if (res < 0) {
            std::cerr << "lfs_write error(" << res << "): ";

            if (res == LFS2_ERR_NOSPC) {
                std::cerr << "File system is full." << std::endl;
            } else {
                std::cerr << "unknown";
            }
            std::cerr << std::endl;

            if (g_debugLevel > 0) {
                std::cout << "data left: " << left << std::endl;
            }
            fclose(f_src);
            lfs2_file_close(&s_fs, &dst);

            return 1;
        }
        left -= 1;
    }

    fclose(f_src);
    lfs2_file_close(&s_fs, &dst);

    return 0;
}

/**
 * @brief copy directorys/files in pc to vfs littlefs filesystem(esp32)
 * @param dirSrc Directory path in pc.
 * @param dirDes root Directory path of vfs littlefs(/littlefsfs).
 * @return True if exists otherwise false.
 *
 */
bool Pack_littlefs::parkFilesToRamFS(const char* dirSrc, const char* dirDes)
{ 
    DIR *dir;
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
        return false;
    }

#if defined(_WIN32)
    _finddata_t findData;
    dir_root = dir_pc + "\\*.*";
    handle = _findfirst(dir_root.c_str(), &findData);
    if (handle == -1)    
        return false;
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
            lfs2_mkdir(&s_fs, dir_full_path_d.c_str()); // Ignore error, we'll catch later if it's fatal
            parkFilesToRamFS(dir_full_path_s.c_str(), dir_full_path_d.c_str()); 
        }
        else 
        {
            // Add File to image.
            if (parkFileToRamFS((char*)dir_full_path_s.c_str(), dir_full_path_d.c_str()) != 0) {
                std::cerr << "error adding file!" << "\n" << std::endl;
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
                    lfs2_mkdir(&s_fs, dir_full_path_d.c_str());   //create directory in vfs littlefs filesystem.
                    parkFilesToRamFS(dir_full_path_s.c_str(), dir_full_path_d.c_str()); 
                }
                else
                {
                    std::cerr << "skipping " << ent->d_name << std::endl;
                    continue;
                }
            }
            else
            {
                // Add File to image.
                if (parkFileToRamFS((char*)dir_full_path_s.c_str(), dir_full_path_d.c_str()) != 0) {
                    std::cerr << "error adding file!" << std::endl;
                    if (g_debugLevel > 0) {
                        std::cout << std::endl;
                    }
                    break;
                }
            }
        } // end while
        closedir (dir);
    } 
#endif 
    return true;
}

/**
 * @brief ccpy all directorys/files from RAM littlefs filesystem to pc.
 * @param path_src: source file, in RAM file system
 * @param path_des: des file, to pc
 * @return True or false.
 */
bool Pack_littlefs::unparkFileFromRamFS(const char* path_src, lfs2_info *littlefsFile, const char* path_des) 
{
    std::vector<uint8_t> temp_buf;

    uint8_t buffer[littlefsFile->size];
    std::string filename = path_src + std::string("/") + littlefsFile->name;

    // Open file from littlefs file system.
    lfs2_file_t src;
    int ret = lfs2_file_open(&s_fs, &src, (char *)(filename.c_str()), LFS2_O_RDONLY);
    if (ret < 0) {
        std::cerr << "unable to open '" << filename.c_str() << "." << std::endl;
        return false;
    }

    // read content into buffer
    lfs2_file_read(&s_fs, &src, buffer, littlefsFile->size);

    // Close littlefs file.
    lfs2_file_close(&s_fs, &src);

    // Open file.
    FILE* dst = fopen(path_des, "wb");
    if (!dst) return false;

    // Write content into file.
    fwrite(buffer, sizeof(uint8_t), sizeof(buffer), dst);

    // Close file.
    fclose(dst);

    // Adjust time, if present
    // uint32_t ftime;
    // if (lfs2_getattr(&s_fs, (char *)(filename.c_str()), 't', (uint8_t *)&ftime, sizeof(ftime)) >= 0) {
    //     struct utimbuf ut;
    //     ut.actime = ftime;
    //     ut.modtime = ftime;
    //     utime(path_des, &ut);
    // }
    return true;
}

/**
 * @brief ccpy all directorys/files from RAM littlefs filesystem to pc.
 * @param dirSrc directory/file in RAM littlefsfs.
 *  @param dirDes directory/file in pc.
 * @return True or false.
 */
bool Pack_littlefs::unparkFilesFromRamFS(const char* dirSrc, std::string dirDes)
{
    lfs2_dir_t dir;
    lfs2_info ent;
    bool error = false;

    // Check if directory exists. If it does not then try to create it with permissions 755.
    if (! dirExists(dirDes.c_str()))
    {
        std::cout << "Directory " << dirDes << " does not exists. Try to create it." << std::endl;

        // Try to create directory on pc.
        if (! dirCreate(dirDes.c_str())) {
            return false;
        }
    }

    // Open directory.
    lfs2_dir_open(&s_fs, &dir, dirSrc);

    // Read content from directory.
    while (lfs2_dir_read(&s_fs, &dir, &ent)==1) {
        // Ignore special dir entries
        if ((strcmp(ent.name, ".") == 0) || (strcmp(ent.name, "..") == 0)) {
            continue;
        }

        // Check if content is a file.
        if ((int)(ent.type) == LFS2_TYPE_REG) {
            std::string name = (const char*)(ent.name);
            std::string dirDesFilePath = dirDes + name;

            // Unpack file to destination directory.
            if (! unparkFileFromRamFS(dirSrc, &ent, dirDesFilePath.c_str()) ) {
                std::cout << "Can not unpack " << ent.name << "!" << std::endl;
                return false;
            }

            // Output stuff.
            std::cout
                << dirSrc
                << ent.name
                << '\t'
                << " > " << dirDesFilePath
                << '\t'
                << "size: " << ent.size << " Bytes"
                << std::endl;
        } else if (ent.type == LFS2_TYPE_DIR) {
            char newPath[PATH_MAX];
            if (dirSrc[0]) {
                sprintf(newPath, "%s/%s/", dirSrc, ent.name);
            } else {
                sprintf(newPath, "%s/", ent.name);  //the root dir's dirSrc[0] = NULL
            }
            std::string newDest = dirDes + ent.name + "/";
            dirCreate(newDest.c_str());
            unparkFilesFromRamFS(newPath, newDest);
        }
        // Get next file handle.
    } // end while

    // Close directory.
    lfs2_dir_close(&s_fs, &dir);

    return (error) ? 1 : 0;
}

/**
 * @brief ccpy all directory/file to esp32's littlefs filesystem.
 * @param 
 * @return True or false.
 */
int Pack_littlefs::actionPack(std::string s_dirName, std::string s_imageName, int s_imageSize) {
    int ret = 0; //0 - ok

    // 1. resize g_flashmem and fill 0xff, it will used for RAM littlefs filesystem.
    g_flashmem.resize(s_imageSize, 0xff);

    // 2. mount g_flashmem(in RAM) as a littlefs filesystem, mount point is BASE_PATH(root directory of the RAM filesystem). 
    fsMount(s_imageName, s_imageSize, true);

    // 3. copy all directorys/files to the RAM littlefs filesystem.
    ret = parkFilesToRamFS(s_dirName.c_str(), "");

    // 4. unmount the RAM littlefs filesystem.
    fsUnmount();

    // 5. open *.bin file witch read from esp32. 
    FILE* fdres = fopen(s_imageName.c_str(), "wb"); 
    if (!fdres) {
        std::cerr << "error: failed to open image file" << std::endl;
        return 1;
    }

    // 6. copy all data in g_flashmem to *.bin file.
    std::cout << "g_flashmem[0]: " << g_flashmem[0] << "size: " << g_flashmem.size() << std::endl;
    fwrite(&g_flashmem[0], 4, g_flashmem.size()/4, fdres); //write all data in RAM littlefsfs to *.bin.
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
int Pack_littlefs::actionUnpack(std::string s_imageName, std::string s_dirName, int s_imageSize)
{
    int ret = 0;
    
    /* 1. copy *.bin file content to g_flashmem and mount it.so we can use lfs2fs operate it.*/
    fsMount(s_imageName, s_imageSize, false);

    /* 2. create dir in pc.. */
    dirCreate(s_dirName.c_str());

    /* 3. unpack files to pc dir. */
    if (s_dirName.back() != '/') {
        s_dirName += "/";
    }
    if (!unparkFilesFromRamFS("", s_dirName)) 
    {
        ret = 1;
    }

    // unmount file system
    fsUnmount();

    return ret;
}

