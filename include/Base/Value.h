#pragma once

#include <Arduino.h>

#include "Item.h"
#include "Mapper.h"

class Value {
   public:
    Value() : _lastValue{""}, _lastTime{0} {}

    void setValue(const String& value) {
        onSetValue(value);
        _lastTime = millis();
    }

    const String getValue() {
        return onGetValue();
    }

    virtual bool hasValue() {
        return false;
    }

   protected:
    virtual void onValueUpdate(const String& value) {}

    virtual void onValueChange(const String& prev, const String& value) {}

    virtual const String onGetValue() {
        return "";
    }

   private:
    void onSetValue(const String& value) {
        if (!value.equals(_lastValue)) {
            onValueChange(_lastValue, value);
            _lastValue = value;
        }
        onValueUpdate(value);
    };

    String _lastValue;
    unsigned long _lastTime;
};

class ValueMap {
   public:
    ValueMap(Value* obj) : _obj{obj}, _mapper{NULL} {};

    void setMap(long in_min, long in_max, long out_min, long out_max) {
        if (_mapper) {
            delete _mapper;
        }
        _mapper = new Mapper{in_min, in_max, out_min, out_max};
    }

    int mapValue(const int value) {
        return _mapper->mapValue(value);
    }

   private:
    Value* _obj;
    Mapper* _mapper;
};