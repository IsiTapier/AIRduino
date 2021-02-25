/*
  Settings.cpp - Settings-Library for Librarys.
*/

#include "Arduino.h"
#include "Settings.h"
#include "../Util.h"

namespace general {
  Setting empty = Setting(EMPTY);  

  Setting version = Setting(SWITCH, "version", {"Version", "version", "version", "versi","versi"+oa+"n"}, 0, "'Version' changed", 0, 1, {{"Version 1", "version 1", "version 1", "versi 1", "versi"+oa+"n 1"}, {"Version 2", "version 2", "version 2", "versi 2", "versi"+oa+"n 2"}}, false);
  Setting theme = Setting(SELECT, "theme", {"Thema", "theme", "th"+eg+"me", "tema", "tema"}, 1, "'Theme' changed", 0, 1, {{"heller Modus", "light mode", "mode clair", "mode cahya", "modo de luz"}, {"dunkel Modus", "dark mode", "mode noir", "mode peteng", "modo oscuro"}});
  Setting sound = Setting(SWITCH, "sound", {"Ton", "sound", "son", "swara", "sonido"}, 1, "'Piezo' changed");
  Setting blink = Setting(SWITCH, "blink", {"Blinken", "blink", "clignoter", "lampu kilat", "destello"}, 1, "'Blink' changed");
  Setting data = Setting(SWITCH, "", {"Daten", "data", "donn"+ea+"es", "data", "datos"}, 1, "Data gettering changed", 0, 1, {{"generierte Daten", "generated data", "donn"+ea+"es g"+ea+"n"+ea+"r"+ea+"es", "data sing digawe", "datos generados"}, {"Sensor Daten", "sensor data", "donn"+ea+"es du capteur", "data sensor", "datos del sensor"}}); //TODO: connect to database
  Setting calibrateTouch = Setting(ACTIVATOR, "", {"Touch kalibrieren", "calibrate Touch", "calibrage tactile", "tutul kalibrasi", "calibrar el tacto"});

  Setting language = Setting(MULTISELECT, "language", {"Sprache", "language", "langue", "basa", "idioma"}, 0, "'Status' changed", 0, 5, {{"deutsch", "german", "allemand", "jerman", "alem"+aa+"n"}, {"englisch", "english", "anglais"}, {"franz"+oe+"sisch", "french", "fran"+cC+"aise", "prancis", "franc"+ea+"s"}, {"java", "java", "javanais", "wong jawa", "javan"+ea+"s"}, {"spanisch", "spanish", "espagnol", "spanyol","espa"+nw+"ol"}, {"spezial", "special", "special", "khusus", "especial", "ESPecial"}});
  Setting kernMode = Setting(SWITCH, "", {"Kern Modus", "Kern mode", "mode de Kern", "mode Kern", "modo de Kern"}, 0, "'Kern Modus' changed");
  Setting graph_speed = Setting(NUMBER, "graph_speed", {"Graph Dauer", "Graph Speed", "vitesse de graphe", "kacepetan grafik", "velocidad gr"+aa+"fica"}, 5, "'Graph Speed' changed", 1, 9);
  Setting segments = Setting(NUMBER, "segments", {"Menu Felder", "menu inputs", "entr"+ea+"es de menu", "kothak menu", "entradas de men"+ua}, 6, "'Segments' changed", 1, 6);
  Setting blink_thickness = Setting(NUMBER, "blink_thickness", {"Blink Dicke", "blink thickness", "grosseur de clignoter", "ketebalan kedhip", "espesor de parpadeo"}, 2, "'Blink Boarder Thickness' changed", 1, 9);
  Setting ventilating_timeout = Setting(NUMBER, "ventilating_timeout", {"Time-out", "timeout", "temps mort", "wektu entek", "se acab"+oa+" el tiempo"}, 5, "'ventilatingTimeout' changed", 1, 9);

  Setting debugPriority = Setting(MULTISELECT, "", {"Debug", "debug", "d"+ea+"boguer", "debug", "depurar"}, 0, "'Debug' changed", 0, 6, {{"Keinen", "none", "rien"}, {"Fehler", "error", "fautes"}, {"Warnungen", "warnings", "alerte"}, {"Wichtig", "important", "important"}, {"Debug", "debug", "d"+ea+"boguer"}, {"Info", "info", "info"}, {"Spam", "spam", "pourriel"}});
  Setting debugSetup = Setting(SWITCH, "", {"Debug Setup", "debug setup", "d"+ea+"boguer installation", "persiyapan debug", "depurar de configuracti"+oa+"n"}, 0, "'Debug Setup' changed");
  Setting debugSensor = Setting(SWITCH, "", {"Debug Sensor", "debug sensor", "d"+ea+"boguer capteur", "sensor debug", "depurar de sensor"}, 0, "'Debug Sensor' changed");
  Setting debugDisplay = Setting(SWITCH, "", {"Debug Display", "debug display", "d"+ea+"boguer affichage", "tampilan debug", "depurar de mostrar"}, 0, "'Debug Display' changed");
  Setting debugMenu = Setting(SWITCH, "", {"Debug Menu", "debug menu", "d"+ea+"boguer menu", "menu debug", "depurar de men"+ua}, 0, "'Debug Menu' changed");
  Setting debugTouch = Setting(SWITCH, "", {"Debug Touch", "debug touch", "d"+ea+"boguer toucher", "tutul debug", "depurar de tocar"}, 0, "'Debug Touch' changed");

  Setting mode = Setting(MULTISELECT, "", {"Modus", "mode", "mode", "pantun", "moda"}, LOADINGSCREEN, "'Mode' changed", 0, 3);
  Setting maintenance_mode = Setting(MULTISELECT, "maintenance_mode", {"Wartungsmodus", "maintance mode", "mode de maintenance", "mode pangop"+ag+"nan", "mode de mantenimiento"}, 0, "'Wartungsmodus' changed", 0, 3);

}

namespace colorModes {
  Setting c_design = Setting(SWITCH, "c_design", {"farbiges Design", "colored design", "design color"+ea, "desain warna", "dise"+nw+"o coloreado"}, 0, "'farbiges Disign' changed");
  Setting c_chart = Setting(SWITCH, "c_chart", {"farbiger Graph", "colored chart", "graphe color"+ea, "denah warna", "carta de colores"}, 1, "'farbiger Graph' changed");
  Setting c_bar = Setting(SWITCH, "c_bar", {"farbige Leiste", "colored bar", "barre color"+ea+"e", "bar warna", "baara de color"}, 1, "'farbige bar' changed");
  Setting c_state = Setting(SWITCH, "c_state", {"farbiger Status", "colored state", ea+"tat color"+ea, "negara warna", "estado de color"}, 0, "'farbiger Status' changed");
  Setting c_time = Setting(SWITCH, "c_time", {"farbige Zeit", "colored time", "temp color"+ea, "wektu warna", "tiempo de color"}, 1, "'farbige Zeit' changed");
  Setting c_value = Setting(SWITCH, "c_value", {"farbiger Wert", "colored value", "valeur color"+ea+"e", "regane warna", "valor de color"}, 1, "'farbiger Wert' changed");

  Setting c_slider = Setting(SWITCH, "c_slider", {"farbiger Toggle", "colord slider", "glissi"+eg+"re color"+ea, "slider warna", "control deslizante de color"}, 1, "'farbiger Toggle' changed");
}

/*namespace colors {
  Setting* graphColor = Setting(COLOR "Graphfarbe", 0, true, "'Graphfarbe' changed");
  Setting* valueNormal = Setting(COLOR, "Farbe 'Gut'", 0, true, "'' changed");
  Setting* valueRisk = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* valueWarning = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* valueAlarm = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* barColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");

  Setting* barSectionColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* DataboxColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* defaultTextColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* ChartBorderColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* SliderDotColorOn = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* SliderDotColorOn = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");

  Setting* SliderColorStrip = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
  Setting* SliderBackgroundColor = Setting(COLOR, "farbiger Toggle", 0, true, "'' changed");
} */
