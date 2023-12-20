//
// Created by lsk on 12/20/23.
//

#include "YeeLight.h"
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <string>

using namespace rapidjson;

int limit(int x, int max, int min) {
    x = x > max ? max : x;
    x = x < min ? min : x;
    return x;
}

YeeLight::YeeLight(string ip, string token) : MiioDevice(ip, token) {
    syncProperties();
}

YeeLight::DeviceProperties YeeLight::getProperties() {
    return this->deviceProperties;
}

void YeeLight::toggle() {
    string power = !this->deviceProperties.mainLightState.power ? "on" : "off";
    if (this->transition != 0) {
        this->sendCommand("set_power", {power, "smooth", to_string(transition)});
    } else {
        this->sendCommand("set_power", {power});
    }
    this->deviceProperties.mainLightState.power = !this->deviceProperties.mainLightState.power;
}

void YeeLight::toggle(bool power) {
    if (this->transition != 0) {
        this->sendCommand("set_power", {power ? "on" : "off", "smooth", to_string(transition)});
    } else {
        this->sendCommand("set_power", {power ? "on": "off"});
    }
    this->deviceProperties.mainLightState.power = power;
}

void YeeLight::setBrightness(int brightness) {
    brightness = limit(brightness, 0, 100);
    if (this->transition != 0) {
        this->sendCommand("set_bright", {to_string(brightness), "smooth", to_string(transition)});
    } else {
        this->sendCommand("set_bright", {to_string(brightness)});
    }
    this->deviceProperties.mainLightState.brightness = brightness;
}

void YeeLight::setMoonlightBrightness(int brightness) {
    // TODO: moonlight brightness
}

void YeeLight::setColorMode(YeeLight::ColorMode colorMode) {
    string mode;
    switch (colorMode) {
        case RGB:
            mode = "0";
            break;
        case HSV:
            mode = "1";
            break;
        case TEMPERATURE:
            mode = "2";
            break;
    }
    this->sendCommand("set_power", {"on", "smooth", to_string(transition), mode});
    this->deviceProperties.mainLightState.colorMode = colorMode;
}

void YeeLight::setTemperature(int temperature) {
    temperature = limit(temperature, 1700, 6500);
    if (this->transition != 0) {
        this->sendCommand("set_ct_abx", {to_string(temperature), "smooth", to_string(transition)});
    } else {
        this->sendCommand("set_ct_abx", {to_string(temperature), "sudden", "0"});
    }
    this->deviceProperties.mainLightState.temperature = temperature;
}

void YeeLight::setHSV(YeeLight::ThreeTuple hsv) {
    // TODO
}

void YeeLight::setRGB(YeeLight::ThreeTuple rgb) {
    // TODO
}

void YeeLight::setMoonlightMode(bool moonlightMode) {
    // TODO
}

void YeeLight::setDeveloperMode(bool developerMode) {
    // TODO
}

void YeeLight::setName(string name) {
    // TODO
}

void YeeLight::setUpdateDefaultOnChange(bool updateDefaultOnChange) {
    // TODO
}

void YeeLight::setDelayMinuteBeforeOff(int delayMinuteBeforeOff) {
    // TODO
}

void YeeLight::setColorFlowMode(bool colorFlowMode) {
    // TODO
}

int YeeLight::rgbToInt(YeeLight::ThreeTuple rgb) {
    return rgb.x << 16 | rgb.y << 8 | rgb.z;
}

YeeLight::ThreeTuple YeeLight::intToRGB(int rgb) {
    ThreeTuple threeTuple;
    threeTuple.x = (rgb >> 16) & 0xFF;
    threeTuple.y = (rgb >> 8)  & 0xFF;
    threeTuple.z = rgb         & 0xFF;
    return threeTuple;
}

void YeeLight::setTransition(int transition) {
    this->transition = transition < 0 ? 0 : transition;
}

int YeeLight::getTransition() {
    return this->transition;
}

void YeeLight::syncProperties() {
    Document doc;
    doc.Parse(this->sendCommand("get_prop", properties).c_str());
    deviceProperties.name = doc["result"][0]["value"].GetString();
    deviceProperties.developerMode = doc["result"][1]["value"].GetInt() == 1;
    deviceProperties.updateDefaultOnChange = doc["result"][2]["value"].GetInt() == 1;
    deviceProperties.delayMinuteBeforeOff = doc["result"][3]["value"].GetInt();
    deviceProperties.mainLightState.power = "on" == doc["result"][4]["value"].GetString();
    deviceProperties.mainLightState.brightness = doc["result"][5]["value"].GetInt();
    switch (doc["result"][6]["value"].GetInt()) {
        case 0:
            deviceProperties.mainLightState.colorMode = RGB;
            break;
        case 1:
            deviceProperties.mainLightState.colorMode = HSV;
            break;
        case 2:
            deviceProperties.mainLightState.colorMode = TEMPERATURE;
            break;
    }
    deviceProperties.mainLightState.rgb = intToRGB(doc["result"][7]["value"].GetInt());
    deviceProperties.mainLightState.hsv.x = doc["result"][8]["value"].GetInt();
    deviceProperties.mainLightState.hsv.y = doc["result"][9]["value"].GetInt();
    deviceProperties.mainLightState.hsv.z = doc["result"][5]["value"].GetInt();
    deviceProperties.mainLightState.temperature = doc["result"][10]["value"].GetInt();
    deviceProperties.mainLightState.colorFlowingMode = doc["result"][11]["value"].GetInt() == 1;
    deviceProperties.moonLightState.inMoonLightMode = doc["result"][12]["value"].GetInt() == 1;
    deviceProperties.moonLightState.brightness = doc["result"][13]["value"].GetInt();
}
