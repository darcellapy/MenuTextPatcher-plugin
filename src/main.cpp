#include <coreinit/filesystem.h>
#include <coreinit/debug.h>

#include <wups.h>
#include <notifications/notifications.h>

#include "utils/logger.h"
#include "Notification.h"

WUPS_PLUGIN_NAME("EasyAllMessageMod");
WUPS_PLUGIN_DESCRIPTION("epik n ez way 2 replace AllMessage.szs with ur own 8) (help)");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Midn1ght");
WUPS_PLUGIN_LICENSE("GPLv2");

WUPS_USE_WUT_DEVOPTAB();

#define ALLMESSAGE_PATH "/vol/content/UsEnglish/Message/AllMessage.szs"
#define ALLMESSAGE_SD_PATH "/vol/external01/customtxts/AllMessage.szs"

FSMountSource mSource;
char mPath[128]= "";

INITIALIZE_PLUGIN() {
    WHBLogUdpInit();
    WHBLogCafeInit();
    
    if (NotificationModule_InitLibrary() != NOTIFICATION_MODULE_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("NotificationModule_InitLibrary failed :(");
    }
        StartNotificationThread("AllMessage.szs patch enabled");
}

DEINITIALIZE_PLUGIN() {
    WHBLogUdpDeinit();
    NotificationModule_DeInitLibrary();
}

ON_APPLICATION_START() {
    WHBLogUdpInit();
    WHBLogCafeInit();
    
    DEBUG_FUNCTION_LINE("EAMM: hihi");
}

DECL_FUNCTION(FSStatus, FSOpenFile, FSClient *client, FSCmdBlock *block, const char *path, const char *mode, FSFileHandle *handle, FSErrorFlag errorMask) {
    if (true) {
        if (strcmp(ALLMESSAGE_PATH, path) == 0) {
            FSGetMountSource(client, block, FS_MOUNT_SOURCE_SD, &mSource, FS_ERROR_FLAG_ALL);
            FSMount(client, block, &mSource, mPath, sizeof(mPath), FS_ERROR_FLAG_ALL);
            FSStatus res = real_FSOpenFile(client, block, ALLMESSAGE_SD_PATH, mode, handle, errorMask);
            if (res != FS_STATUS_OK) {
                OSFatal("--------------- Error ---------------\n\n"\
                        "Error loading AllMessage.szs\n\nPlease check if this file is in the correct directory:\n"\
                        "sd:/customtxts/AllMessage.szs");
            }

            return res;
        }
    }

    return real_FSOpenFile(client, block, path, mode, handle, errorMask);
}

WUPS_MUST_REPLACE(FSOpenFile, WUPS_LOADER_LIBRARY_COREINIT, FSOpenFile);
