//
//  main.cpp
//  mkfatfs
//
//  Created by Victor Mizikov on 20/09/2017.
//  Copyright (c) 2017 Victor Mizikov. All rights reserved.
//
#include "Config.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <time.h>
#include <memory>
#include <cstdlib>
#include "esp_err.h"
#include "esp_log.h"
#define TCLAP_SETBASE_ZERO 1
#include "tclap/CmdLine.h"
#include "tclap/UnlabeledValueArg.h"
#include "pack.h"

#define APP_VERSION mkfatfs_VERSION

int g_debugLevel = 0;

enum Action { ACTION_NONE, ACTION_PACK, ACTION_UNPACK, ACTION_LIST, ACTION_VISUALIZE };
static Action s_action = ACTION_NONE;

static std::string s_dirName;
static std::string s_imageName;
static int s_imageSize;
Pack pack = Pack();

void processArgs(int argc, const char** argv) {
    TCLAP::CmdLine cmd("", ' ', APP_VERSION);
    TCLAP::ValueArg<std::string> packArg( "c", "create", "create fatFS image from a directory", true, "", "pack_dir");
    TCLAP::ValueArg<std::string> unpackArg( "u", "unpack", "unpack fatFS image to a directory", true, "", "dest_dir");
    TCLAP::SwitchArg listArg( "l", "list", "list files in fatFS image", false);
    TCLAP::SwitchArg visualizeArg( "i", "visualize", "visualize fatFS image", false);
    TCLAP::UnlabeledValueArg<std::string> outNameArg( "image_file", "fatFS image file", true, "", "image_file"  );
    TCLAP::ValueArg<int> imageSizeArg( "s", "size", "fs image size, in bytes", false, 0x10000, "number" );
    TCLAP::ValueArg<int> debugArg( "d", "debug", "Debug level. 0 means no debug output.", false, 0, "0-5" );

    cmd.add( imageSizeArg );
    cmd.add(debugArg);
    std::vector<TCLAP::Arg*> args = {&packArg, &unpackArg, &listArg, &visualizeArg};
    cmd.xorAdd( args );
    cmd.add( outNameArg );
    cmd.parse( argc, argv );

    if (debugArg.getValue() > 0) {
        std::cout << "Debug output enabled" << std::endl;
        g_debugLevel = debugArg.getValue();
    }

    if (packArg.isSet()) {
        s_dirName = packArg.getValue();
        s_action = ACTION_PACK;
    } else if (unpackArg.isSet()) {
        s_dirName = unpackArg.getValue();
        s_action = ACTION_UNPACK;
    } else if (listArg.isSet()) {
        s_action = ACTION_LIST;
    } else if (visualizeArg.isSet()) {
        s_action = ACTION_VISUALIZE;
    }

    s_imageName = outNameArg.getValue();
    s_imageSize = imageSizeArg.getValue();
}

int main(int argc, const char * argv[]) {
    try {
        processArgs(argc, argv);
    } catch(...) {
        std::cerr << "Invalid arguments" << std::endl;
        return 1;
    }

    switch (s_action) {
    case ACTION_PACK:
        return pack.actionPack(s_dirName, s_imageName, s_imageSize);
        break;
    case ACTION_UNPACK:
        return pack.actionUnpack(s_imageName, s_dirName, s_imageSize);
         break;
    case ACTION_LIST:

        break;
    case ACTION_VISUALIZE:

        break;
    default:
        break;
    }

    return 1;
}
