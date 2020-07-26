
#pragma once

#include <Arduino.h>

#ifdef ESP8266
#include <Servo.h>
#else
#include <ESP32Servo.h>
#endif

#include "Base/Item.h"
#include "Base/Assigned.h"
#include "Base/Value.h"
#include "Base/ValueMap.h"

class Pwm : public Item,
            public PinAssigned,
            public ValueMap {
   public:
    Pwm(const String& name, const String& assign) : Item{name, assign},
                                                    PinAssigned{this},
                                                    ValueMap{VT_INT} {
        pinMode(getPin(), OUTPUT);
    }

   protected:
    void onValueUpdate(const String& value) override {
        analogWrite(getPin(), value.toFloat());
    }
};
