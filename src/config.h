#ifndef _CONFIG_H
#define _CONFIG_H

class Config {
public:
    static void Init();

    // wups config items
    static bool enable_allmessage_patch;
    static bool enable_notifications;
};

#endif
