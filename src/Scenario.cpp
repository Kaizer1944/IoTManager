#include "Scenario.h"

#include "Global.h"
#include "Cmd.h"

#include "Base/KeyValue.h"
#include "Base/StringQueue.h"

#include "ScenBlock.h"

static const char* MODULE = "Scenario";

namespace Scenario {

std::vector<ScenBlock*> _items;

std::list<String> _events;

bool _ready = false;

void process(KeyValue* obj) {
    process(obj->getKey());
}

void process(const String str) {
    if (config.general()->isScenarioEnabled()) {
        _events.push_front(str);
    }
}

bool isBlockEnabled(size_t num) {
    return _items.at(num)->isEnabled();
}

void enableBlock(size_t num, boolean value) {
    _items.at(num)->enable(value);
}

bool extractBlock(const String& buf, size_t& startIndex, String& block) {
    int endIndex = buf.indexOf("end", startIndex);
    if (endIndex < 0) {
        return false;
    }
    block = buf.substring(startIndex, endIndex);
    startIndex = endIndex + 4;
    return true;
}

void clear() {
    for (auto item : _items) {
        delete item;
    }
    _items.clear();
}

void load() {
    auto file = LittleFS.open(DEVICE_SCENARIO_FILE, FILE_READ);
    if (!file) {
        pm.error("open " + String(DEVICE_SCENARIO_FILE));
        return;
    }
    String condition = "";
    String commands = "";
    bool in_block = false;
    while (file.available()) {
        String line = file.readStringUntil('\n');
        Serial.println(line);
        if (line.startsWith("//")) {
            continue;
        }
        if (in_block) {
            if (line.startsWith("end")) {
                Serial.println("commands: " + commands);
                _items.push_back(new ScenBlock(condition, commands));
                condition = "";
                commands = "";
                in_block = false;
                continue;
            }
            commands += line;
            commands += '\n';
        } else {
            condition = line;
            Serial.println("condition: " + condition);
            in_block = true;
        }
    }
    file.close();
}

void init() {
    clear();

    load();

    if (_items.size()) {
        pm.info("items: " + String(_items.size()));
    } else {
        pm.info("disabled");
        config.general()->enableScenario(false);
    }
}

void loop() {
    if (!config.general()->isScenarioEnabled()) {
        return;
    }
    if (!_events.size()) {
        return;
    }

    String event = _events.front();
    String value = runtime.get(event.c_str());

    for (auto item : _items) {
        if (item->isEnabled()) {
            if (item->checkCondition(event, value)) {
                addCommands(item->getCommands());
            }
        }
    }

    _events.pop_front();
}

}  // namespace Scenario
