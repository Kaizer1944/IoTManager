#ifndef SPIFFSEditor_H_
#define SPIFFSEditor_H_

#include <ESPAsyncWebServer.h>

#if USE_LITTLEFS
#include <LittleFS.h>
#endif

class SPIFFSEditor : public AsyncWebHandler {
   private:
    fs::FS _fs;
    String _username;
    String _password;
    bool _authenticated;
    uint32_t _startTime;

   public:
#ifdef ESP32
    SPIFFSEditor(const fs::FS& fs, const String& username = String(), const String& password = String());
#else
    #if USE_LITTLEFS        
        SPIFFSEditor(const String& username = String(), const String& password = String(), const fs::FS& fs = LittleFS);
    #else 
        SPIFFSEditor(const fs::FS& fs, const String& username = String(), const String& password = String());        
    #endif
#endif
    virtual bool canHandle(AsyncWebServerRequest* request) override final;
    virtual void handleRequest(AsyncWebServerRequest* request) override final;
    virtual void handleUpload(AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data, size_t len, bool final) override final;
    virtual bool isRequestHandlerTrivial() override final { return false; }
};

#endif
