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
#include "tclap/CmdLine.h"
#include "tclap/UnlabeledValueArg.h"
#include <dirent.h>
#include "esp_err.h"
#include "esp_vfs_fat.h"
#include "FatPartition.h"

static const char *BASE_PATH = "/";

int g_debugLevel = 0;

enum Action { ACTION_NONE, ACTION_PACK, ACTION_UNPACK, ACTION_LIST, ACTION_VISUALIZE };
static Action s_action = ACTION_NONE;

static std::string s_dirName;
static std::string s_imageName;
static int s_imageSize;

static wl_handle_t s_wl_handle;
static FATFS* s_fs = NULL;

int addFiles(const char* dirname, const char* subPath) {
    DIR *dir;
    struct dirent *ent;
    bool error = false;
    std::string dirPath = dirname;
    dirPath += subPath;

    // Open directory
    if ((dir = opendir (dirPath.c_str())) != NULL) {

        // Read files from directory.
        while ((ent = readdir (dir)) != NULL) {
            // Ignore dir itself.
            if (ent->d_name[0] == '.')				
                continue;            	

            std::string fullpath = dirPath;
            fullpath += ent->d_name;
            struct stat path_stat;
            stat (fullpath.c_str(), &path_stat);

            if (!S_ISREG(path_stat.st_mode)) {
                // Check if path is a directory.
                if (S_ISDIR(path_stat.st_mode)) {
                    // Prepare new sub path.
                    std::string newSubPath = subPath;
                    newSubPath += ent->d_name;
					
					// WHITECAT BEGIN
					addDir(newSubPath.c_str());
					// WHITECAT END
					
                    newSubPath += "/";

                    if (addFiles(dirname, newSubPath.c_str()) != 0)
                    {
                        std::cerr << "Error for adding content from " << ent->d_name << "!" << std::endl;
                    }

                    continue;
                }
                else
                {
                    std::cerr << "skipping " << ent->d_name << std::endl;
                    continue;
                }
            }

            // Filepath with dirname as root folder.
            std::string filepath = subPath;
            filepath += ent->d_name;
            std::cout << "adding to image: " << filepath << std::endl;

            // Add File to image.
            if (addFile((char*)filepath.c_str(), fullpath.c_str()) != 0) {
                std::cerr << "error adding file!" << std::endl;
                error = true;
                if (g_debugLevel > 0) {
                    std::cout << std::endl;
                }
                break;
            }
        } // end while
        closedir (dir);
    } else {
        std::cerr << "warning: can't read source directory: \"" << dirPath << "\"" << std::endl;
        return 1;
    }

    return (error) ? 1 : 0;
}


int checkFile(char* name, const char* path) {
    //spiffs_metadata_t meta;

    FILE* src = fopen(path, "rb");
    if (!src) {
        std::cerr << "error: failed to open " << path << " for reading" << std::endl;
        return 1;
    }


    std::string nameInFat = BASE_PATH;
    nameInFat += name;

    const int flags = O_RDONLY;
    int fd = emulate_esp_vfs_open(nameInFat.c_str(), flags, 0);
    if (fd < 0) {
        std::cerr << "error: failed to open \"" << nameInFat << "\" for reading" << std::endl;
        return 0; //0 does not stop copying files
    }


    // read file size
    fseek(src, 0, SEEK_END);
    size_t size = ftell(src);
    fseek(src, 0, SEEK_SET);

    if (g_debugLevel > 0) {
        std::cout << "file size: " << size << std::endl;
    }

    size_t left = size;
    uint8_t data_byte;
    uint8_t data_byte2;
    while (left > 0){
        if (1 != fread(&data_byte, 1, 1, src)) {
            std::cerr << "fread error!" << std::endl;
            fclose(src);
            emulate_esp_vfs_close(fd);
            return 1;
        }

        ssize_t res = emulate_esp_vfs_read(fd, &data_byte2, 1);
        if (res < 0) {
            std::cerr << "esp_vfs_read() error, offset=" << (size-left) << std::endl;
            if (g_debugLevel > 0) {
                std::cout << "data left: " << left << std::endl;
            }
            fclose(src);
            emulate_esp_vfs_close(fd);
            return 1;
        }

        if (data_byte != data_byte2) {
            std::cerr << "Verification failed at offset=" << (size-left) << " src="  << data_byte << " dst=" << data_byte2 << std::endl;
            if (g_debugLevel > 0) {
                std::cout << "data left: " << left << std::endl;
            }
            fclose(src);
            emulate_esp_vfs_close(fd);
            return 1;
	}

        left -= 1;
    }

    emulate_esp_vfs_close(fd);

    // Get the system time to file timestamps
//    meta.atime = time(NULL);
//    meta.ctime = meta.atime;
//    meta.mtime = meta.atime;
//    SPIFFS_update_meta(&s_fs, name, &meta);

    fclose(src);

    return 0;
}



int checkFiles(const char* dirname, const char* subPath) {
    DIR *dir;
    struct dirent *ent;
    bool error = false;
    std::string dirPath = dirname;
    dirPath += subPath;

    // Open directory
    if ((dir = opendir (dirPath.c_str())) != NULL) {

        // Read files from directory.
        while ((ent = readdir (dir)) != NULL) {
            // Ignore dir itself.
            if (ent->d_name[0] == '.')				
                continue;            	

            std::string fullpath = dirPath;
            fullpath += ent->d_name;
            struct stat path_stat;
            stat (fullpath.c_str(), &path_stat);

            if (!S_ISREG(path_stat.st_mode)) {
                // Check if path is a directory.
                if (S_ISDIR(path_stat.st_mode)) {
                    // Prepare new sub path.
                    std::string newSubPath = subPath;
                    newSubPath += ent->d_name;
					
					// WHITECAT BEGIN
					//addDir(newSubPath.c_str());
					// WHITECAT END
					
                    newSubPath += "/";

                    if (checkFiles(dirname, newSubPath.c_str()) != 0)
                    {
                        std::cerr << "Error checking content from " << ent->d_name << "!" << std::endl;
                    }

                    continue;
                }
                else
                {
                    std::cerr << "skipping " << ent->d_name << std::endl;
                    continue;
                }
            }

            // Filepath with dirname as root folder.
            std::string filepath = subPath;
            filepath += ent->d_name;
            std::cout << "checking: " << filepath << std::endl;

            // Check file
            if (checkFile((char*)filepath.c_str(), fullpath.c_str()) != 0) {
                std::cerr << "error checking file!" << std::endl;
                error = true;
                if (g_debugLevel > 0) {
                    std::cout << std::endl;
                }
                break;
            }
        } // end while
        closedir (dir);
    } else {
        std::cerr << "warning: can't read source directory: \"" << dirPath << "\"" << std::endl;
        return 1;
    }

    return (error) ? 1 : 0;
}



/*
void listFiles() {
    spiffs_DIR dir;
    spiffs_dirent ent;

    SPIFFS_opendir(&s_fs, 0, &dir);
    spiffs_dirent* it;
    while (true) {
        it = SPIFFS_readdir(&dir, &ent);
        if (!it)
            break;

        std::cout << it->size << '\t' << it->name << std::endl;
    }
    SPIFFS_closedir(&dir);
}
*/





/**
 * @brief Unpack file from file system.
 * @param spiffsFile SPIFFS dir entry pointer.
 * @param destPath Destination file path path.
 * @return True or false.
 *
 * @author Pascal Gollor (http://www.pgollor.de/cms/)
 */
/*
bool unpackFile(spiffs_dirent *spiffsFile, const char *destPath) {
    u8_t buffer[spiffsFile->size];
    std::string filename = (const char*)(spiffsFile->name);

    // Open file from spiffs file system.
    spiffs_file src = SPIFFS_open(&s_fs, (char *)(filename.c_str()), SPIFFS_RDONLY, 0);

    // read content into buffer
    SPIFFS_read(&s_fs, src, buffer, spiffsFile->size);

    // Close spiffs file.
    SPIFFS_close(&s_fs, src);

    // Open file.
    FILE* dst = fopen(destPath, "wb");

    // Write content into file.
    fwrite(buffer, sizeof(u8_t), sizeof(buffer), dst);

    // Close file.
    fclose(dst);
    return true;
}
*/


/**
 * @brief Unpack files from file system.
 * @param sDest Directory path as std::string.
 * @return True or false.
 *
 * @author Pascal Gollor (http://www.pgollor.de/cms/)
 *
 * todo: Do unpack stuff for directories.
 */
bool unpackFiles(std::string sDest) {
/*
    spiffs_DIR dir;
    spiffs_dirent ent;

    // Add "./" to path if is not given.
    if (sDest.find("./") == std::string::npos && sDest.find("/") == std::string::npos) {
        sDest = "./" + sDest;
    }

    // Check if directory exists. If it does not then try to create it with permissions 755.
    if (! dirExists(sDest.c_str())) {
        std::cout << "Directory " << sDest << " does not exists. Try to create it." << std::endl;

        // Try to create directory.
        if (! dirCreate(sDest.c_str())) {
            return false;
        }
    }

    // Open directory.
    SPIFFS_opendir(&s_fs, 0, &dir);

    // Read content from directory.
    spiffs_dirent* it = SPIFFS_readdir(&dir, &ent);
    while (it) {
        // Check if content is a file.
        if ((int)(it->type) == 1) {
            std::string name = (const char*)(it->name);
            std::string sDestFilePath = sDest + name;
            size_t pos = name.find_last_of("/");

            // If file is in sub directory?
            if (pos > 0) {
                // Subdir path.
                std::string path = sDest;
                path += name.substr(0, pos);

                // Create subddir if subdir not exists.
                if (!dirExists(path.c_str())) {
                    if (!dirCreate(path.c_str())) {
                        return false;
                    }
                }
            }

            // Unpack file to destination directory.
            if (! unpackFile(it, sDestFilePath.c_str()) ) {
                std::cout << "Can not unpack " << it->name << "!" << std::endl;
                return false;
            }

            // Output stuff.
            std::cout
                << it->name
                << '\t'
                << " > " << sDestFilePath
                << '\t'
                << "size: " << it->size << " Bytes"
                << std::endl;
        }

        // Get next file handle.
        it = SPIFFS_readdir(&dir, &ent);
    } // end while

    // Close directory.
    SPIFFS_closedir(&dir);
*/
    return true;
}

// Actions


/**
 * @brief Unpack action.
 * @return 0 success, 1 error
 *
 * @author Pascal Gollor (http://www.pgollor.de/cms/)
 */
int actionUnpack(void) {
    int ret = 0;
/*
    g_flashmem.resize(s_imageSize, 0xff);

    // open spiffs image
    FILE* fdsrc = fopen(s_imageName.c_str(), "rb");
    if (!fdsrc) {
        std::cerr << "error: failed to open image file" << std::endl;
        return 1;
    }

    // read content into g_flashmem
    ret = fread(&g_flashmem[0], 4, g_flashmem.size()/4, fdsrc);

    // close fiel handle
    fclose(fdsrc);

    // mount file system
    spiffsMount();

    // unpack files
    if (! unpackFiles(s_dirName)) {
        ret = 1;
    }

    // unmount file system
    spiffsUnmount();
*/
    return ret;
}


int actionList() {
    int ret = 0;
/*
    g_flashmem.resize(s_imageSize, 0xff);

    FILE* fdsrc = fopen(s_imageName.c_str(), "rb");
    if (!fdsrc) {
        std::cerr << "error: failed to open image file" << std::endl;
        return 1;
    }

    ret = fread(&g_flashmem[0], 4, g_flashmem.size()/4, fdsrc);
    fclose(fdsrc);
    spiffsMount();
    listFiles();
    spiffsUnmount();
    
    ret = 0;
*/
    return ret;
}

int actionVisualize() {
    int ret = 0;
/*
    g_flashmem.resize(s_imageSize, 0xff);

    FILE* fdsrc = fopen(s_imageName.c_str(), "rb");
    if (!fdsrc) {
        std::cerr << "error: failed to open image file" << std::endl;
        return 1;
    }

    ret = fread(&g_flashmem[0], 4, g_flashmem.size()/4, fdsrc);
    fclose(fdsrc);

    spiffsMount();
    //SPIFFS_vis(&s_fs);
    uint32_t total, used;
    SPIFFS_info(&s_fs, &total, &used);
    std::cout << "total: " << total <<  std::endl << "used: " << used << std::endl;
    spiffsUnmount();

    ret = 0;
*/
    return ret;
}
