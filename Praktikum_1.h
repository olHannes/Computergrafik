#pragma once
#include <iostream>
#include <tuple>
#include <cmath>

struct RGB {
    float Red = 0.0f;
    float Green = 0.0f;
    float Blue = 0.0f;
};

struct CMY {
    float Cyan = 0.0f;
    float Magenta = 0.0f;
    float Yellow = 0.0f;
};

struct HSV {
    float Hue = 0.0f;
    float Saturation = 0.0f;
    float Value = 0.0f;
};

using namespace std;

class TranslateColorValues {
public:
    TranslateColorValues() = default;
    ~TranslateColorValues() = default;

    static RGB CMYtoRGB(const CMY& cmy);
    static HSV CMYtoHSV(const CMY& cmy);
    static RGB HSVtoRGB(const HSV& hsv);
    static CMY HSVtoCMY(const HSV& hsv);

    static void printCMY(const CMY& cmy);
    static void printHSV(const HSV& hsv);
    static void printRGB(const RGB& rgb);

    void handleInputTask01();
    RGB handleInputTask02();


private:
    bool checkValuesForChoice(float v1, float v2, float v3, int choice);
    bool checkValues(float value);
};
