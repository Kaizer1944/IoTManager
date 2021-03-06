#pragma once

#include <ESPAsyncWebServer.h>
#include "FileSystem.h"

class FSEditor : public AsyncWebHandler {
   private:
    fs::FS _fs;
    String _username;
    String _password;
    bool _authenticated;
    uint32_t _startTime;

   private:
    void getDirList(const String& path, String& output);

   public:
#ifdef ESP32
    FSEditor(const fs::FS& fs, const String& username = String(), const String& password = String());
#else
    FSEditor(const String& username = String(), const String& password = String(), const fs::FS& fs = FileFS);
#endif
    virtual bool canHandle(AsyncWebServerRequest* request) override final;
    virtual void handleRequest(AsyncWebServerRequest* request) override final;
    virtual void handleUpload(AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data, size_t len, bool final) override final;
    virtual bool isRequestHandlerTrivial() override final {
        return false;
    }
};