
#include "WeatherGui.h"

String WeatherGui::weather = "";
String WeatherGui::windspeed = "";
String WeatherGui::humidity = "";
String WeatherGui::temp = "";     
String WeatherGui::forecastWeather3 = "";
String WeatherGui::forecastWeatherTomorrow = "";

void WeatherGui::loop() {
    
}

void WeatherGui::initGui() {
    if(gui.equals(WEATHER_GUI)) {
        display.fillScreen(BLACK); 
        Display::drawTopBar("Wetter");
        if(client.connected()) {
            if(weather != "") {
                updateWeather(weather);
                updateWindspeed(windspeed);
                updateHumidity(humidity);
                updateTemp(temp);
                updateForecastWeatherTomorrow(forecastWeatherTomorrow);
                updateForecastWeather3(forecastWeather3);
                //display.fillRect(DISPLAY_LENGTH*2/3, DISPLAY_HEIGHT*1/4, 5, DISPLAY_HEIGHT*2/4, GREY);
            } else {
                dPrint("Wetterdaten", DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2, 4, LIGHTGREY, 4);
                dPrint("fehlen", DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2 + 35, 4, RED, 4);
            }
            
        } else {
            dPrint("Internet", DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2, 4, LIGHTGREY, 4);
            dPrint("fehlgeschlagen", DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2 + 35, 3, RED, 4);
        }
    }  
}

void WeatherGui::updateWeather(String _weather) {
    if(weather == "") {
        if(gui.equals(WEATHER_GUI)) {
            display.fillScreen(BLACK);
            dPrint("Wetter", MENU_ARROW_BACK_START_X - 5, 20, 2, GREY, 5);      
        }
        // dPrint("Wetter", DISPLAY_LENGTH/2, DISPLAY_HEIGHT/6, 5, BLUE, 4);
    }
    weather = _weather;
    if(gui.equals(WEATHER_GUI)) {       
        display.fillRect(10, DISPLAY_HEIGHT*1/3, 100, 70, BLACK);
        dPrint("Aktuell", 10, DISPLAY_HEIGHT*1/3, 2, GREY);
        drawWeatherIcon(_weather, 10, DISPLAY_HEIGHT*1/3 + 15); 
    }
    if(gui.equals(OVERVIEW_GUI)) {
        OverviewGui::drawWeatherData(temp, weather, humidity);
    }
   
}

void WeatherGui::updateWindspeed(String _speed) {
    windspeed = _speed;
    if(gui.equals(WEATHER_GUI)) { 
        display.fillRect(10, DISPLAY_HEIGHT*2/3, 100, 80, BLACK);
        drawWeatherIcon("Wind", 10, DISPLAY_HEIGHT*2/3);
        if(windspeed.length() > 4)
            windspeed == windspeed[0] + windspeed[1] + "";
        dPrint(windspeed, 30, DISPLAY_HEIGHT*2/3 + 30, 3, WHITE);
        dPrint("km/h", 50, DISPLAY_HEIGHT*2/3 + 52, 2, GREY);
    }
}

void WeatherGui::updateHumidity(String _humidity) {
    humidity = _humidity;
    if(gui.equals(WEATHER_GUI)) {
        display.fillRect(110, DISPLAY_HEIGHT*1/3, 100, 80, BLACK);
        dPrint("Feucht.", DISPLAY_LENGTH/2, DISPLAY_HEIGHT*1/3, 2, GREY, 1);
        dPrint(_humidity + "%", DISPLAY_LENGTH/2, DISPLAY_HEIGHT*1/3 + 30, 3, WHITE, 4);
    }
}

void WeatherGui::updateTemp(String _temp) {
    temp = _temp;
    if(gui.equals(WEATHER_GUI)) {
        display.fillRect(110, DISPLAY_HEIGHT*2/3, 100, 80, BLACK);
        dPrint("Temperatur", DISPLAY_LENGTH/2, DISPLAY_HEIGHT*2/3, 2, GREY, 1);      
        dPrint(_temp, DISPLAY_LENGTH/2, DISPLAY_HEIGHT*2/3 + 30, 3, WHITE, 4);
    }
}

void WeatherGui::updateForecastWeatherTomorrow(String _weather) {
    forecastWeather3 = _weather;
    if(gui.equals(WEATHER_GUI)) {

        display.fillRect(230, DISPLAY_HEIGHT*2/3, 100, 80, BLACK);
        drawWeatherIcon(_weather, 230, DISPLAY_HEIGHT*2/3 + 15);
        dPrint("Morgen", 230, DISPLAY_HEIGHT*2/3, 2, GREY);
    }
}
void WeatherGui::updateForecastWeather3(String _weather) {
    forecastWeatherTomorrow = _weather;
    if(gui.equals(WEATHER_GUI)) {
        display.fillRect(230, DISPLAY_HEIGHT*1/3, 100, 80, BLACK);
        drawWeatherIcon(_weather, 230, DISPLAY_HEIGHT*1/3 + 15);
        dPrint("in 3h", 230, DISPLAY_HEIGHT*1/3, 2, GREY);
    }
}

void WeatherGui::drawWeatherIcon(String _weather, int x, int y) {
    if(_weather == "Rain") {
        display.pushImage(x, y, 61, 55, weatherStrongRainIcon);
    } else if(_weather == "Atmosphere") {
        display.pushImage(x, y, 59, 57, weathreAtmosphereIcon);
    } else if(_weather == "Thunderstorm") {
        display.pushImage(x, y, 67, 59, weatherStormIcon);
    } else if(_weather == "Drizzle") {
        display.pushImage(x, y, 56, 58, weatherRainIcon);
    } else if(_weather == "Snow") {
        display.pushImage(x, y, 57, 54, weatherSnowIcon);
    } else if(_weather == "Clear") {
        display.pushImage(x, y, 58, 56, weatherClearIcon);
    } else if(_weather == "Clouds") {
        display.pushImage(x, y, 63, 56, weatherIconClouds);
    }
     else if(_weather == "Wind") {
        display.pushImage(x, y, 61, 55, weatherWindIcon);
    }
}
