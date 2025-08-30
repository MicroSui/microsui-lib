#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__) || defined(__unix__)

int microsui_connect_wifi(const char* ssid, const char* password) 
{
    (void)ssid; (void)password;

    // In desktops it's assumed that the OS is already connected to the Internet.

    return 0;
}

int microsui_disconnect_wifi() {
    return 0;
}

#endif