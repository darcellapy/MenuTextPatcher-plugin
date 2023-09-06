#include "config.h"
#include "utils/logger.h"

#include <coreinit/title.h>
#include <sysapp/launch.h>

#include <wups.h>
#include <wups/config/WUPSConfigItemBoolean.h>
#include <wups/config/WUPSConfigItemStub.h>

bool Config::enable_message_patch = true;
bool Config::enable_notifications = true;
bool needRelaunch = false;

void Config::Init() {
    WUPSStorageError storageRes = WUPS_OpenStorage();
    if (storageRes != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to open storage %s (%d)", WUPS_GetStorageStatusStr(storageRes), storageRes);
    }
    else {
        // Try to get value from storage
        if ((storageRes = WUPS_GetBool(nullptr, "enable_message_patch", &enable_message_patch)) == WUPS_STORAGE_ERROR_NOT_FOUND) {
            // Add the value to the storage if it's missing.
            if (WUPS_StoreBool(nullptr, "enable_message_patch", enable_message_patch) != WUPS_STORAGE_ERROR_SUCCESS) {
                DEBUG_FUNCTION_LINE("Failed to store bool");
            }
        }
        if ((storageRes = WUPS_GetBool(nullptr, "enable_notifications", &enable_notifications)) == WUPS_STORAGE_ERROR_NOT_FOUND) {
            // Add the value to the storage if it's missing.
            if (WUPS_StoreBool(nullptr, "enable_notifications", enable_notifications) != WUPS_STORAGE_ERROR_SUCCESS) {
                DEBUG_FUNCTION_LINE("Failed to store bool");
            }
        }
        else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS) {
            DEBUG_FUNCTION_LINE("Failed to get bool %s (%d)", WUPS_GetStorageStatusStr(storageRes), storageRes);
        }

        // Close storage
        if (WUPS_CloseStorage() != WUPS_STORAGE_ERROR_SUCCESS) {
            DEBUG_FUNCTION_LINE("Failed to close storage");
        }
    }
}

static void enable_message_patch_changed(ConfigItemBoolean *item, bool new_value) {
    DEBUG_FUNCTION_LINE("New value in enable_message_patch_changed: %d", new_value);
    
    Config::enable_message_patch = new_value;
    needRelaunch = true;
    WUPS_StoreBool(nullptr, "enable_message_patch", Config::enable_message_patch);
}

static void enable_notifications_changed(ConfigItemBoolean *item, bool new_value) {
    DEBUG_FUNCTION_LINE("New value in enable_notifications_changed: %d", new_value);
    
    Config::enable_notifications = new_value;
    needRelaunch = true;
    WUPS_StoreBool(nullptr, "enable_notifications", Config::enable_notifications);
}

WUPS_GET_CONFIG() {
    // We open the storage so we can persist the configuration the user did.
    if (WUPS_OpenStorage() != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to open storage");
        return 0;
    }

    WUPSConfigHandle config;
    WUPSConfig_CreateHandled(&config, "MenuTextPatcher");

    WUPSConfigCategoryHandle cat;
    WUPSConfig_AddCategoryByNameHandled(config, "Patching", &cat);

    WUPSConfigItemBoolean_AddToCategoryHandled(config, cat, "Enable text patching", "Enable text patching", Config::enable_message_patch, &enable_message_patch_changed);
    WUPSConfigItemBoolean_AddToCategoryHandled(config, cat, "Toggle Notifications", "Toggle Notifications", Config::enable_notifications, &enable_notifications_changed);

    return config;
}

WUPS_CONFIG_CLOSED() {
    // Save all changes
    if (WUPS_CloseStorage() != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to close storage");
    }

    // Relaunch the currently running title
    if (needRelaunch) {
        _SYSLaunchTitleWithStdArgsInNoSplash(OSGetTitleID(), nullptr);
        needRelaunch = false;
    }
}
