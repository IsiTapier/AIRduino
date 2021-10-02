/*
 Display.cpp - Display-Library for Librarys.
*/

#include "Display.h"
#include "manager/Manager.h"

using namespace general;

  TSPoint Display::p;  
  unsigned long Display::lastTouch;
  int Display::lastGui = GUI_MENU;

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
    DisplayVX::peepLoop();

    Meassure::loop();
    boolean changed = DisplayV1::getGraphData();
    
    TimerGui::peep();


    if(mode.getValue() == MENU) {
      Menu::loop();
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
      //  StopwatchGui::loop();
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

      if(p.isTouching(MENU_ARROW_BACK_START_X-15, MENU_ARROW_BACK_END_X+10, MENU_ARROW_BACK_START_Y-10, MENU_ARROW_BACK_END_Y+15)) {
        if(mode.equals(CHART)) {
          if(gui.equals(GUI_MENU)) {
            gui.setValue(lastGui);
          } else {
            lastGui = gui.getValue();
            gui.setValue(GUI_MENU);
          }
        } else if(mode.equals(MENU)) {
          mode.setValue(CHART);
          gui.setValue(lastGui);
        }
        Manager::lastModeChange = millis();
        return;
      }

      if(p.isTouching(MENU_ARROW_RESET_START_X-3, MENU_ARROW_RESET_END_X+10, MENU_ARROW_RESET_START_Y-3, MENU_ARROW_RESET_END_Y+10)) {
          //Reset Settings
        if(mode.equals(MENU) && requestDecision("Einstellungs Reset", "Willst du fortfahren?")) {
          if(debugSetup.getValue())
            debug(WARNING, SETUP, "reset");
          Menu::reset();
          Menu::setup();
          
        //Reset Time Counter of CO2 Gui
        } else if(mode.equals(CHART)) {
          if(gui.equals(CO2_GUI)) {
            Meassure::resetStartTime(true);
            DisplayV1::resetGraph();
            if(version.equals(V1))
              DisplayV1::setup();
            return;
          }                     
        }     
        
        if(!gui.equals(CO2_GUI)) {
          if(mode.equals(CHART)) {
            if(debugSetup.getValue())
              debug(INFO, SETUP, "change Mode");
            if(mode.equals(MENU)) {
              gui.setValue(lastGui);
              mode.setValue(CHART);
            } else if(mode.equals(CHART)) {
              lastGui = gui.getValue();
              mode.setValue(MENU);
            }
              
            Manager::lastModeChange = millis();
          }
        }
      } 

      if(mode.equals(MENU)) {
        Menu::handleTouch(p);
      }
      if(mode.equals(CHART)) {
        TimerGui::handleTouch(p);
        TimeGui::handleTouch(p);
        DecibelGui::registerTouch(p);
        StopwatchGui::handleTouch(p);
        MenuGui::handleTouch(p);
        RandomStudentGui::handleTouch(p);
      }
      //-----------------
      if(mode.equals(CHART)) {        
        if(gui.equals(CO2_GUI)) {
          version.shiftValue();
          if(version.equals(V1)) {
            DisplayV1::setup();
            DisplayV1::loop(false);
          } 
          if(version.equals(V2)) {
            DisplayV2::setup();
            DisplayV2::loop();
          }
          if(version.equals(V3)) {
            DisplayV3::setup();
            DisplayV3::loop();
          }    
          return;
        }     
      }
      lastTouch+=50;
    }
  } else {
    p = ts.getPoint();
  }
}

void Display::initAllGuis() {
  OverviewGui::initGui();
  StopwatchGui::initGui();
  WeatherGui::initGui();
  TimerGui::initGui();
  MenuGui::initGui();
  RandomStudentGui::initGui();
  DecibelGui::initGui();
  TimeGui::initGui();
  ClockGui::initGui();
  
}

void Display::drawInfoScreen(int time) {
  display.fillScreen(BLACK);
  dPrint(device_class, 160, 10, 5, LIGHT_BLUE, 1);
  dPrint("Software:", 10, 70, 3, GREY, 0);
  dPrint(software_version, 9*6*3+10, 70, 3, LIGHTGREY, 0);

  dPrint("Mapping:", 10, 100, 3, GREY, 0);
  if((SENSORMAPMIN == 0) && (SENSORMAPMAX == 0)) {
    dPrint("AUS", 8*6*3+10, 100, 3, RED, 0);
  } else {
  	dPrint("AN", 8*6*3+10, 100, 3, GREEN, 0);
  }

  dPrint("Connection:", 10, 130, 3, GREY, 0);
  if(client.connected()) {
    dPrint("online", 11*6*3+10, 130, 3, GREEN, 0);
  } else if(WiFi.status() != WL_CONNECTED) {
    dPrint("WLAN", 11*6*3+10, 130, 3, RED, 0);
  } else {
    dPrint("MQTT", 11*6*3+10, 130, 3, RED, 0);
  }   

  dPrint("Wlan:", 10, 160, 3, GREY, 0);
  dPrint(ssid, 5*6*3+10, 160, 3, LIGHTGREY, 0);
  
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

void Display::drawTopBar(String title, String oldTitle) {
  dPrint(title, MENU_ARROW_BACK_START_X - 5, 20, 2, GREY, 5, BLACK, oldTitle);
  // display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
  display.pushImage(MENU_ARROW_BACK_START_X, MENU_ARROW_BACK_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, homeDark, BLACK);
  display.pushImage(MENU_ARROW_RESET_START_X, MENU_ARROW_RESET_START_Y, MENU_ICON_LENGTH, MENU_ICON_HEIGHT, gear, WHITE);
}

void Display::screenShot() {
  /* int output[DISPLAY_LENGTH][DISPLAY_HEIGHT];
  uint16_t _array[DISPLAY_LENGTH*DISPLAY_HEIGHT] PROGMEM;
  display.readRectRGB(0, 0, DISPLAY_LENGTH, DISPLAY_HEIGHT, _array)
  for(int y = 0; y <= DISPLAY_HEIGHT; y++) {
    for(int x = 0; x <= DISPLAY_LENGTH; x++) {
      
      
    }
  }
  Serial.print(); */
}

