#include "http_router.h"

// Platform selection
#if defined(ESP_PLATFORM) || defined(ARDUINO_ARCH_ESP32)
  // ESP-IDF o Arduino-ESP32
  #include "impl/http/http_esp32.cpp"

#elif defined(_WIN32) || defined(__linux__) || defined(__APPLE__) || defined(__unix__)
  // cURL (Windows/Linux/macOS/other unix like systems)
  #include "impl/http/http_curl.inc"

#else
  #error "Not supported platform: define guards or add an implementation."
#endif
