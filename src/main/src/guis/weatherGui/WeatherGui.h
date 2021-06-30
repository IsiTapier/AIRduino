/*
  WeatherGui.h - WeatherGui-Library for Airduino.
*/

#ifndef WeatherGui_h
#define WeatherGui_h

#include "Arduino.h"
#include "../../util/Util.h"
#include "../../manager/Manager.h"

class WeatherGui {
  public:
    static void loop();
    static void initGui();
    static void updateWeather(String weather);
    static void updateWindspeed(String id);
    static void updateHumidity(String id);
    static void updateTemp(String id);
    static void updateForecastWeatherTomorrow(String weather);
    static void updateForecastWeather3(String weather);
    static void drawWeatherIcon(String weather, int x, int y);

    static String weather;
    static String windspeed;
    static String humidity;
    static String temp;   
    static String forecastWeather3;
    static String forecastWeatherTomorrow;
  private:
    

};

#endif

