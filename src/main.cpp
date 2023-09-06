/*
 *   Copyright (C) 2023 Midn1ght
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <coreinit/filesystem.h>
#include <coreinit/debug.h>

#include <wups.h>
#include <notifications/notifications.h>

#include "utils/logger.h"
#include "config.h"
#include "Notification.h"

WUPS_PLUGIN_NAME("MenuTextPatcher");
WUPS_PLUGIN_DESCRIPTION("Patch the Wii U menu text with your own easily");
WUPS_PLUGIN_VERSION("v2.5");
WUPS_PLUGIN_AUTHOR("Midn1ght");
WUPS_PLUGIN_LICENSE("GPLv2");
WUPS_USE_STORAGE("mtp");

WUPS_USE_WUT_DEVOPTAB();

// original system paths
// allmessage.szs paths
#define US_ENGLISH_ALLMESSAGE_PATH "/vol/content/UsEnglish/Message/AllMessage.szs"
#define JP_JAPANESE_ALLMESSAGE_PATH "/vol/content/JpJapanese/Message/AllMessage.szs"
#define EU_SPANISH_ALLMESSAGE_PATH "/vol/content/EuSpanish/Message/AllMessage.szs/"
#define EU_ENGLISH_ALLMESSAGE_PATH "/vol/content/EuEnglish/Message/AllMessage.szs/"

// hbm.pack paths
// #define HBM_PACK_PATH "/vol/content/Common/Package/Hbm.pack"

// scope.msbt.zlb paths
// #define US_ENGLISH_SCOPE_MSBT_ZLB_PATH "/vol/content/UsEnglish/Message/scope.msbt.zlb"
// #define JP_JAPANESE_SCOPE_MSBT_ZLB_PATH "/vol/content/JpJapanese/Message/scope.msbt.zlb"
// #define EU_SPANISH_SCOPE_MSBT_ZLB_PATH "/vol/content/EuSpanish/Message/scope.msbt.zlb"
// #define EU_ENGLISH_SCOPE_MSBT_ZLB_PATH "/vol/content/EuEnglish/Message/scope.msbt.zlb"

// target paths in the sd card
#define ALLMESSAGE_SD_PATH "/vol/external01/customtxts/AllMessage.szs"
// #define HBM_PACK_SD_PATH "/vol/external01/customtxts/Hbm.pack"
// #define SCOPE_MSBT_ZLB_SD_PATH "/vol/external01/customtxts/scope.msbt.zlb"

FSMountSource mSource;
char mPath[128]= "";

INITIALIZE_PLUGIN() {
    WHBLogUdpInit();
    WHBLogCafeInit();
    Config::Init();
    
    if (NotificationModule_InitLibrary() != NOTIFICATION_MODULE_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("NotificationModule_InitLibrary failed :(");
    }
    
        if (Config::enable_message_patch) {
            if (Config::enable_notifications) {
                StartNotificationThread("Menu text patch enabled");
                /*
                if (Config::enable_Hbm_and_scope) {
                    StartNotificationThread("Hbm.pack and scope.msbt.zlb patch enabled");
                } else {
                    StartNotificationThread("Hbm.pack and scope.msbt.zlb patch disabled");
            }
            */
        } else {
            if (Config::enable_notifications) {
                StartNotificationThread("Menu text patch disabled");
            }
        }
    }
}

DEINITIALIZE_PLUGIN() {
    WHBLogUdpDeinit();
    NotificationModule_DeInitLibrary();
}

ON_APPLICATION_START() {
    WHBLogUdpInit();
    WHBLogCafeInit();
    
    DEBUG_FUNCTION_LINE("MenuTextPatcher: At your service!");
}

DECL_FUNCTION(FSStatus, FSOpenFile, FSClient *client, FSCmdBlock *block, const char *path, const char *mode, FSFileHandle *handle, FSErrorFlag errorMask) {
    // patch allmessage.szs
    if (Config::enable_message_patch) {
        if (strcmp(US_ENGLISH_ALLMESSAGE_PATH, path) == 0) {
            FSGetMountSource(client, block, FS_MOUNT_SOURCE_SD, &mSource, FS_ERROR_FLAG_ALL);
            FSMount(client, block, &mSource, mPath, sizeof(mPath), FS_ERROR_FLAG_ALL);
            FSStatus res = real_FSOpenFile(client, block, ALLMESSAGE_SD_PATH, mode, handle, errorMask);
            if (res != FS_STATUS_OK) {
                StartNotificationThread("Warning: AllMessage.szs is missing!");
            }   
            
            return res;      
        } else if (strcmp(JP_JAPANESE_ALLMESSAGE_PATH, path) == 0) {
            FSGetMountSource(client, block, FS_MOUNT_SOURCE_SD, &mSource, FS_ERROR_FLAG_ALL);
            FSMount(client, block, &mSource, mPath, sizeof(mPath), FS_ERROR_FLAG_ALL);
            FSStatus res = real_FSOpenFile(client, block, ALLMESSAGE_SD_PATH, mode, handle, errorMask);
            if (res != FS_STATUS_OK) {
                StartNotificationThread("Warning: AllMessage.szs is missing!");
            }   
            
            return res;      
        } else if (strcmp(EU_SPANISH_ALLMESSAGE_PATH, path) == 0) {
            FSGetMountSource(client, block, FS_MOUNT_SOURCE_SD, &mSource, FS_ERROR_FLAG_ALL);
            FSMount(client, block, &mSource, mPath, sizeof(mPath), FS_ERROR_FLAG_ALL);
            FSStatus res = real_FSOpenFile(client, block, ALLMESSAGE_SD_PATH, mode, handle, errorMask);
            if (res != FS_STATUS_OK) {
                StartNotificationThread("Warning: AllMessage.szs is missing!");
            }
            
            return res;       
        } else if (strcmp(EU_ENGLISH_ALLMESSAGE_PATH, path) == 0) {
            FSGetMountSource(client, block, FS_MOUNT_SOURCE_SD, &mSource, FS_ERROR_FLAG_ALL);
            FSMount(client, block, &mSource, mPath, sizeof(mPath), FS_ERROR_FLAG_ALL);
            FSStatus res = real_FSOpenFile(client, block, ALLMESSAGE_SD_PATH, mode, handle, errorMask);
            if (res != FS_STATUS_OK) {
                StartNotificationThread("Warning: AllMessage.szs is missing!");
            }
            
            return res;
        }
    
    }
    
    return real_FSOpenFile(client, block, path, mode, handle, errorMask);
}
        
WUPS_MUST_REPLACE(FSOpenFile, WUPS_LOADER_LIBRARY_COREINIT, FSOpenFile);
