#CFLAGS		?= -std=gnu99 -Os -Wall
#CXXFLAGS	?= -std=gnu++11 -Os -Wall
BUILD ?= build
IDF_DIR = esp_idf
IDF_INCLUDES += -I $(IDF_DIR)
IDF_INCLUDES += -I $(IDF_DIR)/freertos

FILESYSTEM_DIR = filesystem
IDF_INCLUDES += -I $(FILESYSTEM_DIR)/diskio
IDF_INCLUDES += -I $(FILESYSTEM_DIR)/fatfs
IDF_INCLUDES += -I $(FILESYSTEM_DIR)/vfs

TCLAP_DIR = tclap
IDF_INCLUDES += -I $(TCLAP_DIR)

# OS = 1
ifdef OS
ifeq ($(OS),Windows_NT)
  OS_IS_WIN = 1
endif
endif

ifdef OS_IS_WIN
	TARGET_OS := WINDOWS
	DIST_SUFFIX := windows
	ARCHIVE_CMD := 7z a
	ARCHIVE_EXTENSION := zip
	TARGET := mkfatfs.exe
	CC=gcc.exe
	CXX=g++.exe
	#MVA TARGET_CFLAGS := -mno-ms-bitfields
	CPATH := $(COMPONENT_INCLUDES)
	#TARGET_CFLAGS := -mno-ms-bitfields -std=gnu99 -Os -Wall -I $(COMPONENT_INCLUDES) -Itclap -Ifatfs -I. -D$(TARGET_OS)
	TARGET_CFLAGS := -mno-ms-bitfields -std=gnu99 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) $(IDF_INCLUDES)
	TARGET_CXXFLAGS	:= -std=gnu++11 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) $(IDF_INCLUDES)
	TARGET_LDFLAGS := -Wl,-static -static-libgcc

else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		TARGET_OS := LINUX
		UNAME_P := $(shell uname -p)
		ifeq ($(UNAME_P),x86_64)
			DIST_SUFFIX := linux64
		endif
		ifneq ($(filter %86,$(UNAME_P)),)
			DIST_SUFFIX := linux32
		endif
		CC=gcc
		CXX=g++
		TARGET_CFLAGS   = -std=gnu99 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) -DVERSION=\"$(VERSION)\" -D__NO_INLINE__  $(IDF_INCLUDES)
		TARGET_CXXFLAGS = -std=gnu++11 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) -DVERSION=\"$(VERSION)\" -D__NO_INLINE__  $(IDF_INCLUDES)
		TARGET_LDFLAGS  =
	endif
	ifeq ($(UNAME_S),Darwin)
		TARGET_OS := OSX
		DIST_SUFFIX := osx
		CC=clang
		CXX=clang++
		TARGET_CFLAGS   = -std=gnu99 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) -DVERSION=\"$(VERSION)\" -D__NO_INLINE__ -mmacosx-version-min=10.7 -arch x86_64 $(IDF_INCLUDES)
		TARGET_CXXFLAGS = -std=gnu++11 -Os -Wall -Itclap -Ifatfs -I. -D$(TARGET_OS) -DVERSION=\"$(VERSION)\" -D__NO_INLINE__ -mmacosx-version-min=10.7 -arch x86_64 -stdlib=libc++ $(IDF_INCLUDES)
		TARGET_LDFLAGS  = -arch x86_64 -stdlib=libc++
	endif
	ARCHIVE_CMD := tar czf
	ARCHIVE_EXTENSION := tar.gz
	TARGET := mkfatfs
endif

OBJ := $(BUILD)/main.o \
	$(BUILD)/$(IDF_DIR)/errno.o \
	$(BUILD)/$(IDF_DIR)/idf_reent.o \
	$(BUILD)/$(IDF_DIR)/lock.o \
	$(BUILD)/$(IDF_DIR)/freertos/semphr.o \
	$(BUILD)/$(FILESYSTEM_DIR)/fatfs/ccsbcs.o \
	$(BUILD)/$(FILESYSTEM_DIR)/fatfs/esp_vfs_fat.o \
	$(BUILD)/$(FILESYSTEM_DIR)/fatfs/ff.o \
	$(BUILD)/$(FILESYSTEM_DIR)/fatfs/syscall.o \
	$(BUILD)/$(FILESYSTEM_DIR)/diskio/diskio_RAM.o \
	$(BUILD)/$(FILESYSTEM_DIR)/diskio/diskio.o \
	$(BUILD)/$(FILESYSTEM_DIR)/diskio/FatPartition.o \
	$(BUILD)/$(FILESYSTEM_DIR)/vfs/user_vfs.o \
	$(BUILD)/$(FILESYSTEM_DIR)/vfs/vfs.o \
				   
VERSION ?= $(shell git describe --always)

.PHONY: all clean

all: $(TARGET)

$(TARGET):
	@echo "Building mkfatfs ..."
	$(CXX) $(TARGET_CXXFLAGS) -c main.cpp -o $(BUILD)/main.o
	$(CC) $(TARGET_CFLAGS) -c $(IDF_DIR)/errno.c -o $(BUILD)/$(IDF_DIR)/errno.o 
	$(CC) $(TARGET_CFLAGS) -c $(IDF_DIR)/idf_reent.c -o $(BUILD)/$(IDF_DIR)/idf_reent.o 
	$(CC) $(TARGET_CFLAGS) -c $(IDF_DIR)/lock.c -o $(BUILD)/$(IDF_DIR)/lock.o 
	$(CC) $(TARGET_CFLAGS) -c $(IDF_DIR)/freertos/semphr.c -o $(BUILD)/$(IDF_DIR)/freertos/semphr.o 
	$(CC) $(TARGET_CFLAGS) -c $(FILESYSTEM_DIR)/fatfs/ccsbcs.c -o $(BUILD)/$(FILESYSTEM_DIR)/fatfs/ccsbcs.o 
	$(CC) $(TARGET_CFLAGS) -c $(FILESYSTEM_DIR)/fatfs/esp_vfs_fat.c -o $(BUILD)/$(FILESYSTEM_DIR)/fatfs/esp_vfs_fat.o
	$(CC) $(TARGET_CFLAGS) -c $(FILESYSTEM_DIR)/fatfs/ff.c -o $(BUILD)/$(FILESYSTEM_DIR)/fatfs/ff.o 
	$(CC) $(TARGET_CFLAGS) -c $(FILESYSTEM_DIR)/fatfs/syscall.c -o $(BUILD)/$(FILESYSTEM_DIR)/fatfs/syscall.o 
	$(CXX) $(TARGET_CXXFLAGS) -c $(FILESYSTEM_DIR)/diskio/diskio_RAM.cpp -o $(BUILD)/$(FILESYSTEM_DIR)/diskio/diskio_RAM.o
	$(CC) $(TARGET_CFLAGS) -c $(FILESYSTEM_DIR)/diskio/diskio.c -o $(BUILD)/$(FILESYSTEM_DIR)/diskio/diskio.o
	$(CXX) $(TARGET_CXXFLAGS) -c $(FILESYSTEM_DIR)/diskio/FatPartition.cpp -o $(BUILD)/$(FILESYSTEM_DIR)/diskio/FatPartition.o
	$(CC) $(TARGET_CFLAGS) -c $(FILESYSTEM_DIR)/vfs/user_vfs.c -o $(BUILD)/$(FILESYSTEM_DIR)/vfs/user_vfs.o 
	$(CC) $(TARGET_CFLAGS) -c $(FILESYSTEM_DIR)/vfs/vfs.c -o $(BUILD)/$(FILESYSTEM_DIR)/vfs/vfs.o
	$(CXX) $(TARGET_CFLAGS) -o $(TARGET) $(OBJ) $(TARGET_LDFLAGS)
	
clean:
	@rm -f *.o
	@rm -f $(BUILD)/$(IDF_DIR)/*.o
	@rm -f $(BUILD)/$(IDF_DIR)/frertos/*.o
	@rm -f $(I$(BUILD)/$(FILESYSTEM_DIR)/fatfs/*.o
	@rm -f $(BUILD)/$(FILESYSTEM_DIR)/diskio/*.o
	@rm -f $(BUILD)/$(FILESYSTEM_DIR)/vfs/*.o
	@rm -f $(TARGET)
