/*
  Settings.cpp - Settings-Library for Librarys.
*/

#include "Arduino.h"
#include "Settings.h"
#include "../Util.h"

  

  //titles
  const std::vector<String> titlesVersion = {"Version", "version", "version", "versi","versi"+oa+"n"};
  const std::vector<String> titlesTheme = {"Thema", "theme", "th"+eg+"me", "tema", "tema"};
  const std::vector<String> titlesSound = {"TON", "sound", "son", "swara", "sonido"};
  const std::vector<String> titlesBlink = {"Blinken", "blink", "clignoter", "lampu kilat", "destello"};
  const std::vector<String> titlesData = {"Daten", "data", "donn"+ea+"es", "data", "datos"};
  const std::vector<String> titlesCalibrateTouch = {"Touch kalibrieren", "calibrate Touch", "calibrage tactile", "tutul kalibrasi", "calibrar el tacto"};
  const std::vector<String> titlesReportSystem = {"Report System"};

  const std::vector<String> titlesLanguage = {"Sprache", "language", "langue", "basa", "idioma"};
  const std::vector<String> titlesKernMode = {"Kern Modus", "Kern mode", "mode de Kern", "mode Kern", "modo de Kern"};
  const std::vector<String> titlesGraphSpeed = {"Graph Dauer", "Graph Speed", "vitesse de graphe", "kacepetan grafik", "velocidad gr"+aa+"fica"};
  const std::vector<String> titlesSegments = {"Menu Felder", "menu inputs", "entr"+ea+"es de menu", "kothak menu", "entradas de men"+ua};
  const std::vector<String> titlesBlinkThickness = {"Blink Dicke", "blink thickness", "grosseur de clignoter", "ketebalan kedhip", "espesor de parpadeo"};
  const std::vector<String> titlesVentilatingTimeout = {"Time-out", "timeout", "temps mort", "wektu entek", "se acab"+oa+" el tiempo"};

  const std::vector<String> titlesDebugPriority = {"Debug", "debug", "d"+ea+"boguer", "debug", "depurar"};
  const std::vector<String> titlesDebugSetup = {"Debug Setup", "debug setup", "d"+ea+"boguer installation", "persiyapan debug", "depurar de configuracti"+oa+"n"};
  const std::vector<String> titlesDebugSensor = {"Debug Sensor", "debug sensor", "d"+ea+"boguer capteur", "sensor debug", "depurar de sensor"};
  const std::vector<String> titlesDebugDisplay = {"Debug Display", "debug display", "d"+ea+"boguer affichage", "tampilan debug", "depurar de mostrar"};
  const std::vector<String> titlesDebugMenu = {"Debug Menu", "debug menu", "d"+ea+"boguer menu", "menu debug", "depurar de men"+ua};
  const std::vector<String> titlesDebugTouch = {"Debug Touch", "debug touch", "d"+ea+"boguer toucher", "tutul debug", "depurar de tocar"};

  const std::vector<String> titlesDebugDatabase = {"Debug Datenbank", "debug database"};
  const std::vector<String> titlesDevelopperSettings = {"Develop Settings"};
  const std::vector<String> titlesAutoTimeReset = {"auto Zeit reset", "auto time reset"};

  const std::vector<String> titlesMode = {"Modus", "mode", "mode", "pantun", "moda"};
  const std::vector<String> titlesMaintenanceMode = {"Wartungsmodus", "maintance mode", "mode de maintenance", "mode pangop"+ag+"nan", "mode de mantenimiento"};

  const std::vector<String> titlesColoredDesign = {"farbiges Design", "colored design", "design color"+ea, "desain warna", "dise"+nw+"o coloreado"};
  const std::vector<String> titlesColoredChart = {"farbiger Graph", "colored chart", "graphe color"+ea, "denah warna", "carta de colores"};
  const std::vector<String> titlesColoredBar = {"farbige Leiste", "colored bar", "barre color"+ea+"e", "bar warna", "baara de color"};
  const std::vector<String> titlesColoredState = {"farbiger Status", "colored state", ea+"tat color"+ea, "negara warna", "estado de color"};
  const std::vector<String> titlesColoredTime = {"farbige Zeit", "colored time", "temp color"+ea, "wektu warna", "tiempo de color"};
  const std::vector<String> titlesColoredValue = {"farbiger Wert", "colored value", "valeur color"+ea+"e", "regane warna", "valor de color"};

  const std::vector<String> titlesColoredSlider = {"farbiger Toggle", "colord slider", "glissi"+eg+"re color"+ea, "slider warna", "control deslizante de color"};
  const std::vector<String> titlesShowTopBar = {"show top-bar"};

  const std::vector<String> titlesCalibrateMin = {"calibrate Min"};
  const std::vector<String> titlesCalibrateMax = {"calibrate Max"};
  const std::vector<String> titlesAutoMinCali = {"auto Min Calibration"};


  const std::vector<String> titles = {};

  //names
  const std::vector<std::vector<String>> namesVersion = {{"Version 1", "version 1", "version 1", "versi 1", "versi"+oa+"n 1"}, {"Version 2", "version 2", "version 2", "versi 2", "versi"+oa+"n 2"}, {"Version 3", "version 3", "version 3", "versi 3", "versi"+oa+"n 3"}};
  const std::vector<std::vector<String>> namesTheme = {{"heller Modus", "light mode", "mode clair", "mode cahya", "modo de luz"}, {"dunkel Modus", "dark mode", "mode noir", "mode peteng", "modo oscuro"}};
  const std::vector<std::vector<String>> namesSound = {};
  const std::vector<std::vector<String>> namesBlink = {};
  const std::vector<std::vector<String>> namesData = {{"generierte Daten", "generated data", "donn"+ea+"es g"+ea+"n"+ea+"r"+ea+"es", "data sing digawe", "datos generados"}, {"Sensor Daten", "sensor data", "donn"+ea+"es du capteur", "data sensor", "datos del sensor"}};
  const std::vector<std::vector<String>> namesCalibrateTouch = {};

  const std::vector<std::vector<String>> namesLanguage = {{"deutsch"/*, "german", "allemand", "jerman", "alem"+aa+"n"*/}, {/*"englisch",*/ "english"/*, "anglais", "basa inggris", "ingl"+ea+"s"*/}, {/*"franz"+oe+"sisch", "french",*/ "fran"+cC+"aise"/*, "prancis", "franc"+ea+"s"*/}, {"java"/*, "java", "javanais", "wong jawa", "javan"+ea+"s"*/}, {/*"spanisch", "spanish", "espagnol", "spanyol",*/"espa"+nw+"ol"}, {/*"spezial",*/ "special"/*, "special", "khusus", "especial", "ESPecial"*/}};
  const std::vector<std::vector<String>> namesKernMode = {};
  const std::vector<std::vector<String>> namesGraphSpeed = {};
  const std::vector<std::vector<String>> namesSegments = {};
  const std::vector<std::vector<String>> namesBlinkThickness = {};
  const std::vector<std::vector<String>> namesVentilatingTimeout = {};

  const std::vector<std::vector<String>> namesDebugPriority = {{"Keinen", "none", "rien"}, {"Fehler", "error", "fautes"}, {"Warnungen", "warnings", "alerte"}, {"Wichtig", "important", "important"}, {"Debug", "debug", "d"+ea+"boguer"}, {"Info", "info", "info"}, {"Spam", "spam", "pourriel"}};
  const std::vector<std::vector<String>> namesDebugSetup = {};
  const std::vector<std::vector<String>> namesDebugSensor = {};
  const std::vector<std::vector<String>> namesDebugDisplay = {};
  const std::vector<std::vector<String>> namesDebugMenu = {};
  const std::vector<std::vector<String>> namesDebugTouch = {};

  const std::vector<std::vector<String>> namesDebugDatabase = {};
  const std::vector<std::vector<String>> namesDevelopperSettings = {};
  const std::vector<std::vector<String>> namesAutoTimeReset = {};

  const std::vector<std::vector<String>> namesMode = {};
  const std::vector<std::vector<String>> namesMaintenanceMode = {};

  const std::vector<std::vector<String>> namesColoredDesign = {};
  const std::vector<std::vector<String>> namesColoredChart = {};
  const std::vector<std::vector<String>> namesColoredBar = {};
  const std::vector<std::vector<String>> namesColoredState = {};
  const std::vector<std::vector<String>> namesColoredTime = {};
  const std::vector<std::vector<String>> namesColoredValue = {};

  const std::vector<std::vector<String>> namesColoredSlider = {};
  const std::vector<std::vector<String>> namesShowTopBar = {};

  const std::vector<std::vector<String>> namesCalibrateMin = {};
  const std::vector<std::vector<String>> namesCalibrateMax = {};
  const std::vector<std::vector<String>> names = {};
  
namespace general {
  Setting empty(EMPTY);  

  Setting version(MULTISELECT, "version", titlesVersion, 2, "'Version' changed", 1, 3, namesVersion, false);
  Setting theme(SELECT, "theme", titlesTheme, 1, "'Theme' changed", 0, 1, namesTheme);
  Setting sound(SWITCH, "sound", titlesSound, 1, "'Piezo' changed");
  Setting blink(SWITCH, "", titlesBlink, 1, "'Blink' changed");
  Setting data(SWITCH, "", titlesData, 1, "Data gettering changed", 0, 1, namesData); //TODO: connect to database
  Setting calibrateTouch(ACTIVATOR, "", titlesCalibrateTouch, 0);
  
  Setting language(MULTISELECT, "language", titlesLanguage, 0, "'Status' changed", 0, 5, namesLanguage);
  // Setting kernMode(SWITCH, "", titlesKernMode, 0, "'Kern Modus' changed");
  Setting graph_speed(NUMBER, "graph_speed", titlesGraphSpeed, 5, "'Graph Speed' changed", 1, 9);
  Setting segments(NUMBER, "segments", titlesSegments, 6, "'Segments' changed", 1, 6);
  Setting blink_thickness(NUMBER, "", titlesBlinkThickness, 2, "'Blink Boarder Thickness' changed", 1, 9);
  Setting ventilating_timeout(NUMBER, "ventilating_timeout", titlesVentilatingTimeout, 5, "'ventilatingTimeout' changed", 1, 9);

  Setting debugPriority(MULTISELECT, "", titlesDebugPriority, 5, "'Debug' changed", 0, 6, namesDebugPriority);
  Setting debugSetup(SWITCH, "", titlesDebugSetup, 1, "'Debug Setup' changed");
  Setting debugSensor(SWITCH, "", titlesDebugSensor, 1, "'Debug Sensor' changed");
  Setting debugDisplay(SWITCH, "", titlesDebugDisplay, 0, "'Debug Display' changed");
  Setting debugMenu(SWITCH, "", titlesDebugMenu, 0, "'Debug Menu' changed");
  Setting debugTouch(SWITCH, "", titlesDebugTouch, 1, "'Debug Touch' changed");
  
  Setting debugDatabase(SWITCH, "", titlesDebugDatabase, 0, "'Debug Database' changed");
  Setting developperSettings(SWITCH, "", titlesDevelopperSettings, 0, "'Develop settings' changed");
  Setting autoTimeReset(SWITCH, "", titlesAutoTimeReset, 0, "'Auto Time Reset' changed");
  
  Setting mode(MULTISELECT, "", titlesMode, MENU, "'Mode' changed", 0, 4);
  Setting gui(MULTISELECT, "", titles, CO2_GUI, "'gui' changed", 0, CALIBRATE_GUI);
  Setting maintenance_mode(MULTISELECT, "maintenance_mode", titlesMaintenanceMode, 0, "'Wartungsmodus' changed", 0, 3);
  Setting reportSystem(ACTIVATOR, "", titlesReportSystem, 4);
  Setting manualReconnect(ACTIVATOR, "", {"reconnect"}, 12);
  Setting manualRestart(ACTIVATOR, "", {"restart"}, 15);
}

namespace colorModes {
  Setting c_design(SWITCH, "", titlesColoredDesign, 0, "'farbiges Disign' changed");
  Setting c_chart(SWITCH, "coloredChart", titlesColoredChart, 1, "'farbiger Graph' changed");
  Setting c_bar(SWITCH, "", titlesColoredBar, 1, "'farbige bar' changed");
  Setting c_state(SWITCH, "", titlesColoredState, 0, "'farbiger Status' changed");
  Setting c_time(SWITCH, "", titlesColoredTime, 1, "'farbige Zeit' changed");
  Setting c_value(SWITCH, "", titlesColoredValue, 1, "'farbiger Wert' changed");

  Setting c_slider(SWITCH, "", titlesColoredSlider, 1, "'farbiger Toggle' changed");
  Setting showTopBar(SWITCH, "", titlesShowTopBar, 1, "'showTopBar' changed");
}

namespace developper {
  Setting calibrateMin(ACTIVATOR, "", titlesCalibrateMin, 1);
  Setting calibrateMax(ACTIVATOR, "", titlesCalibrateMax, 2);
  Setting autoCalibrationMin(ACTIVATOR, "", titlesAutoMinCali, 3);
  Setting deviceInfo(ACTIVATOR, "", {"Device Info"}, 11);
  Setting screenShot(ACTIVATOR, "", {"Screenshot"}, 13);
  Setting isMappingActive(SWITCH, "", {"Mapping Active"}, 0);
  Setting triggerCalibrateGui(ACTIVATOR, "", {"CalibrateGui"}, 14);
}

namespace report {
  Setting sensorError(ACTIVATOR, "", {"Sensor Fehler"}, 5);
  Setting sensorWrong(ACTIVATOR, "", {"Komische Werte"}, 6);
  Setting graphicalBug(ACTIVATOR, "", {"Graphische Fehler"}, 7);
  Setting acousticBug(ACTIVATOR, "", {"Ton Problem"}, 8);
  Setting helpShortterm(ACTIVATOR, "", {"Hilfe schnell"}, 9);
  Setting developerEmail(ACTIVATOR, "", {"Entwickler-Email"}, 10);
}
