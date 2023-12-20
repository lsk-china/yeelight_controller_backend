//
// Created by lsk on 12/20/23.
//

#ifndef YEELIGHT_BACKEND_YEELIGHT_H
#define YEELIGHT_BACKEND_YEELIGHT_H

#include "MiioDevice.h"

class YeeLight : MiioDevice{
public:
    enum ColorMode {
        RGB, HSV, TEMPERATURE
    };

    struct ThreeTuple {
        int x;
        int y;
        int z;
    };

    struct MainLightState {
        bool power;
        int brightness;
        ColorMode colorMode;
        int temperature;
        struct ThreeTuple rgb;
        struct ThreeTuple hsv;
        bool colorFlowingMode;
    };

    struct MoonLightState {
        bool inMoonLightMode;
        int brightness;
    };

    struct DeviceProperties {
        string name;
        bool updateDefaultOnChange;
        int delayMinuteBeforeOff;
        bool developerMode;
        struct MainLightState mainLightState;
        struct MoonLightState moonLightState;
    };

    YeeLight(string ip, string token);
    void syncProperties();
    struct DeviceProperties getProperties();
    void setTransition(int transition);
    int getTransition();
    void toggle();
    void toggle(bool power);
    void setBrightness(int brightness);
    void setMoonlightBrightness(int brightness);
    void setColorMode(ColorMode colorMode);
    void setTemperature(int temperature);
    void setHSV(struct ThreeTuple hsv);
    void setRGB(struct ThreeTuple rgb);
    void setMoonlightMode(bool moonlightMode);
    void setDeveloperMode(bool developerMode);
    void setName(string name);
    void setUpdateDefaultOnChange(bool updateDefaultOnChange);
    void setDelayMinuteBeforeOff(int delayMinuteBeforeOff);
    void setColorFlowMode(bool colorFlowMode);

    vector<string> properties = {"name",
                           "lan_ctrl",
                           "save_state",
                           "delayoff",
                           // Main Light
                           "power",
                           "bright",
                           "color_mode",
                           "rgb",
                           "hue",
                           "sat",
                           "ct",
                           "flowing",
                           // Moon Light
                           "active_mode",
                           "nl_br"
    };
private:
    ThreeTuple intToRGB(int rgb);
    int rgbToInt(ThreeTuple rgb);

    struct DeviceProperties deviceProperties;
    int transition = 0;
};


#endif //YEELIGHT_BACKEND_YEELIGHT_H
