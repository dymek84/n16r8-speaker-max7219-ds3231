
#include <Arduino.h>
#include <WiFi.h>
#include <FastLED.h>
// #include <time.h> // time() ctime()
#include <ArduinoJson.h>
#include <Wire.h>
// #include <TimeLib.h>

#include <SPI.h>
#include <RotaryEncoder.h>
#include <OneButton.h>
#include "RTClib.h"
#include <Preferences.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define LED_PIN 4
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define NUM_LEDS 64

#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 11

#define PAUSE_TIME 1000
#define SCROLL_SPEED 50
// 18 17 19
#define CLK_PIN 17
#define DATA_PIN 18
#define CS_PIN 19
RTC_DS3231 rtc;
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
CRGB leds[NUM_LEDS];
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
void setup()
{
    Serial.begin(115200);
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(50);
    P.begin();

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1)
            delay(10);
    }
    else
    {
        Serial.println("RTC is set");
    }
    if (rtc.lostPower())
    {
        Serial.println("RTC lost power, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
}
void loop()
{
    DateTime now = rtc.now();
    P.displayText(now.timestamp(DateTime::TIMESTAMP_FULL).c_str(), PA_CENTER, SCROLL_SPEED, PAUSE_TIME, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}