/*
 Display.cpp - Display-Library for Librarys.
*/

#include "Display.h"
#include "manager/Manager.h"

using namespace general;

  TSPoint Display::p;  
  unsigned long Display::lastTouch;

  void Display::setup() {
    debug(DEBUG, SETUP, "Display SETUP started");
    
    display.begin();
    debug(INFO, SETUP, "Display connection started");
    display.fillScreen(BACKGROUND_COLOR);
    display.setTextWrap(false);
    display.setRotation(ROTATION);

    debug(DEBUG, SETUP, "Display SETUP completed");
    debug(DEBUG, SETUP, "");
    drawLogo();
  }

  void Display::loop() {
    handleTouch();
    initDisplay();
    
    if(Manager::currentCycleTime - Manager::lastLoop >= LOOP_TIME) {
      Manager::lastLoop = Manager::currentCycleTime;

      Meassure::loop();
      boolean changed = DisplayV1::getGraphData();

      if(mode.getValue() == MENU) {
        Manager::loop();
      } else if(mode.getValue() == CHART) {
        if(gui.getValue() == CO2_GUI) {
          if(version.equals(V1)) {
            DisplayV1::loop(changed);
          } else if(version.equals(V2)) {
            DisplayV2::loop();
          } else if(version.equals(V3)) {
            DisplayV3::loop();
          }
        } else if(gui.getValue() == STOPWATCH_GUI) {
          StopwatchGui::loop();
/*         } else if(gui.getValue() == TIMER_GUI) {
     
        } else if(gui.getValue() == DECIBEL_GUI) {
          
        } else if(gui.getValue() == RANDOM_STUDENT_GUI) {
          */
        } 
      } else if(mode.getValue() == MAINTENANCE) {
        maintenanceMode();
      }
    }
  }

  void Display::initDisplay() {
    if(gui.hasChanged() || mode.hasChanged() || (version.hasChanged() && !mode.equals(MENU)) || theme.hasChanged() || language.hasChanged()) {
      if(mode.getValue() == CHART) {
        Serial.println("initDisplay");
        if(gui.equals(CO2_GUI)) {
          if(version.equals(V1)) {
            DisplayV1::setup();
            DisplayV1::loop(false);
          } else if(version.equals(V2)) {
            DisplayV2::setup();
            DisplayV2::loop();
          } else if(version.equals(V3)) {
            DisplayV3::setup();
            DisplayV3::loop();
          }
        }  
        initAllGuis(); 
      } else if(mode.getValue() == MENU && (theme.hasChanged() || mode.hasChanged() || language.hasChanged())) {
        ledcDetachPin(PIEZO);
        Menu::setup();
      } else if(mode.getValue() == LOADINGSCREEN) {
        Serial.println("Restart");
        ESP.restart();
      } else if(mode.getValue() == RESET) {
        mode.setValue(mode.getOldValue());
        Menu::reset();
        initDisplay();
      }
      version.setValue(version.getValue(), false);
      theme.setValue(theme.getValue(), false);
      language.setValue(language.getValue(), false);
      mode.setValue(mode.getValue(), false);
      gui.setValue(gui.getValue(), false);
    }
  }
  
  void Display::handleTouch() {
    if(!p.isTouching()) {
      // a point object holds x y and z coordinates
      p = ts.getPoint();
      // we have some minimum pressure we consider 'valid'
      // pressure of 0 means no pressing!
      //template:: p.isTouching(PLACEHOLDER_PLACE_START_X, PLACEHOLDER_PLACE_END_X, PLACEHOLDER_PLACE_START_Y, PLACEHOLDER_PLACE_END_Y)
      if(p.isTouching() && millis()-lastTouch >= TOUCH_COOLDOWN && millis() > lastTouch) {
        p.calibrate();
        if(!p.isTouching())
          return;
        lastTouch = millis();
        if(p.isTouching(MENU_ARROW_BACK_START_X-10, MENU_ARROW_BACK_END_X+10, MENU_ARROW_BACK_START_Y-10, MENU_ARROW_BACK_END_Y+10)) {
          if(gui.equals(GUI_MENU)) {
            if(mode.equals(MENU)) {
              MenuGui::initGui();
              mode.setValue(CHART);
              debug(INFO, SETUP, "change Mode to chart");
            }        
          } else {
            gui.setValue(GUI_MENU);           
          }           
          lastTouch+=500;
          Manager::lastModeChange = millis();
          return;
        } 
        if(p.isTouching(MENU_ARROW_RESET_START_X-3, MENU_ARROW_RESET_END_X+3, MENU_ARROW_RESET_START_Y-3, MENU_ARROW_RESET_END_Y+3)) {
          if(mode.equals(MENU) && requestDecision("Einstellungs Reset", "Willst du fortfahren?")) {
            if(debugSetup.getValue())
              debug(WARNING, SETUP, "reset");
            Menu::reset();
            Menu::setup();
            
          } else if(mode.equals(CHART)) {
            if(gui.equals(CO2_GUI)) {
              Meassure::resetStartTime(true);
              DisplayV1::resetGraph();
              if(version.equals(V1))
                DisplayV1::setup(); 
            }            
          }
          if(gui.equals(GUI_MENU)) {
            if(mode.equals(CHART)) {
              if(debugSetup.getValue())
                debug(INFO, SETUP, "change Mode");
              if(mode.equals(MENU))
                mode.setValue(CHART);
              else if(mode.equals(CHART))
                mode.setValue(MENU);
              Manager::lastModeChange = millis();
            }
          }
          return;
        } 
        if(mode.equals(MENU)) {
          Menu::handleTouch(p);
          //Stopwatch GUI
        } 
        if(p.isTouching(DISPLAY_LENGTH/2, DISPLAY_LENGTH, DISPLAY_HEIGHT/2, DISPLAY_HEIGHT)) {
          if(gui.equals(STOPWATCH_GUI) && mode.equals(CHART)) {
            StopwatchGui::toggleStopwatch();
            return;
          }          
          
        } if(p.isTouching(0, DISPLAY_LENGTH/2, DISPLAY_HEIGHT/2, DISPLAY_HEIGHT)) {
          if(gui.equals(STOPWATCH_GUI) && mode.equals(CHART)) {
            StopwatchGui::resetStopwatch();
          } 
        //Timer Gui    
        } if(p.isTouching(45, 100, 15, 100)) {
          if(gui.equals(TIMER_GUI)) {
            Serial.println("oben l");
            TimerGui::updateDigit(1, 1);
            return;
          }
        } if(p.isTouching(190, 240, 14, 100)) {
          if(gui.equals(TIMER_GUI)) {
            Serial.println("oben r");
            TimerGui::updateDigit(2, 15);
            return;
          }
          
        } 
        if(p.isTouching(40, 100, 101, 170)) {
          if(gui.equals(TIMER_GUI)) {
            Serial.println("unten l");
            TimerGui::updateDigit(1, -1);
            return;
          }
        } 
        if(p.isTouching(190, 240, 101, 170)) {
          if(gui.equals(TIMER_GUI)) {
            TimerGui::updateDigit(2, -15);
            Serial.println("unten r");
            return;
          }
        }
        if(gui.equals(TIMER_GUI)) {
          if(p.isTouching(0, 160, DISPLAY_HEIGHT-66, 240)) {
            TimerGui::resetTimer();
            Serial.println("reset timer");
            return;
          }
          if(p.isTouching(161, 320, DISPLAY_HEIGHT-66, 240 )) {
            TimerGui::toggleTimer();
            Serial.println("toggle timer");
            return;
          }
        }
        //-----------------
        if(mode.equals(CHART)) {
          gui.shiftValue();
          if(gui.equals(CO2_GUI)) {
            if(version.equals(V1)) {
              DisplayV1::setup();
              DisplayV1::loop(false);
            } if(version.equals(V2)) {
              DisplayV2::setup();
              DisplayV2::loop();
            } if(version.equals(V3)) {
              DisplayV3::setup();
              DisplayV3::loop();
            }
            return;
          }     
          initAllGuis();
          lastTouch+=500;
        } 
      }
    } else {
      p = ts.getPoint();
    }
  }

void Display::initAllGuis() {
  StopwatchGui::initGui();
  WeatherGui::initGui();
  TimerGui::initGui();
  MenuGui::initGui();
}

void Display::drawInfoScreen(int time) {
  display.fillScreen(BLACK);
  dPrint(device_class, 160, 10, 5, LIGHT_BLUE, 1);
  dPrint("Software:", 170, 70, 3, GREY, 5);
  dPrint(software_version, 170, 70, 3, LIGHTGREY, 3);

  dPrint("Mapping:", 200, 100, 3, GREY, 5);
  if((SENSORMAPMIN == 0) && (SENSORMAPMAX == 0)) {
    dPrint("AUS", 200, 100, 3, RED, 3);
  } else {
  	dPrint("AN", 200, 100, 3, GREEN, 3);
  }

  dPrint("Connection:", 200, 130, 3, GREY, 5);
  if(client.connected()) {
     dPrint("online", 200, 130, 3, GREEN, 3);
  } else if(WiFi.status() != WL_CONNECTED) {
    dPrint("WLAN", 200, 130, 3, RED, 3);
  } else {
    dPrint("MQTT", 200, 130, 3, RED, 3);
  }   

  dPrint("Wlan:", 150, 160, 3, GREY, 5);
  dPrint(ssid, 150, 160, 3, LIGHTGREY, 3);
  
  delay(time);
  general::mode.setValue(LOADINGSCREEN);
  general::mode.setValue(general::mode.getOldValue());  
}

void Display::drawTutorial() {

  display.fillScreen(BLACK);
  display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
  dPrint("1436", PPM_MARGIN_LEFT, PPM_Y, PPM_SIZE, COLOR_STATUS_RISK, 6); 
  if(general::theme.getValue())
    display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowResetDark, BLACK);
  else
    display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, arrowResetLight, WHITE);
  dPrint("00:00", TIMER_X, TIMER_Y, TIMER_SIZE, WHITE, 8);
  display.drawLine(DISPLAY_LENGTH-1, TOP_BAR_HEIGHT, DISPLAY_LENGTH-1, TOP_BAR_HEIGHT+DATABOX_BAR_THICKNESS-1, COLOR_STATUS_RISK);
  display.fillRect(0, DATABOX_BAR_Y, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS, COLOR_STATUS_RISK);  
  display.fillRect(0, TOP_BAR_HEIGHT, DISPLAY_LENGTH, DATABOX_BAR_THICKNESS/* TOP_BAR_THICKNESS */, COLOR_STATUS_RISK);
  dPrint("Tutorial", DISPLAY_LENGTH/2, STATUS_MARGIN_TOP, 3, GREY, 1);


  dPrint("Timer", DISPLAY_LENGTH - 10, DATABOX_BAR_Y - 20, 2, GREY, 2); 
  dPrint("Luftqualit" +ae+ "t", 10, DATABOX_BAR_Y - 20, 2, GREY, 0);   
  dPrint("Men" +ue, 310, TOP_BAR_HEIGHT + 12, 2, GREY, 2);
  dPrint("Timer-Reset", 10, TOP_BAR_HEIGHT + 12, 2, GREY, 0);
  delay(10000);
  drawAuthors();
}

void Display::drawAuthors() {
    display.fillScreen(BLACK);
    dPrint("created by", 160, 100, 2, GREY, 4);
    dPrint("Erfinder-AG", 160, 130, 3, LIGHT_BLUE, 4);
    delay(3000);
    
}



