// Default color definitions
#define BLACK       0x0000      /*   0,   0,   0 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define BLUE        0x001F      /*   0,   0, 255 */
#define GREEN       0x07E0      /*   0, 255,   0 */
#define CYAN        0x07FF      /*   0, 255, 255 */
#define RED         0xF800      /* 255,   0,   0 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define YELLOW      0xFFE0      /* 255, 255,   0 */
#define WHITE       0xFFFF      /* 255, 255, 255 */
#define ORANGE      0xFDA0      /* 255, 180,   0 */
#define GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
#define BROWN       0x9A60      /* 150,  75,   0 */
#define GOLD        0xFEA0      /* 255, 215,   0 */
#define SILVER      0xC618      /* 192, 192, 192 */
#define SKYBLUE     0x867D      /* 135, 206, 235 */
#define VIOLET      0x915C      /* 180,  46, 226 */
#define GREY        0x8C51
#define LIME        0x87F4
#define LIGHT_BLUE  0x76BF
#define TURKISE     0x3FFA



#define BLACK_MODE general::theme.getValue()

#define GRAPH_COLOR (BLACK_MODE) ? WHITE : BLACK //WHITE//MAGENTA
#define BACKGROUND_COLOR (BLACK_MODE) ? BACKGROUND_COLOR_BLACK : BACKGROUND_COLOR_LIGHT
#define DATABOX_BACKGROUND_COLOR (BLACK_MODE) ? 0x2104 : 0xC618//Fast schwarz
#define TIME_COLOR_CRITICAL RED
#define TIME_COLOR_NORMAL TEXT_COLOR//WHITE
#define PPM_COLOR_N (BLACK_MODE) ? GREEN : 0x3DE0//Normal
#define PPM_COLOR_R (BLACK_MODE) ? ORANGE : 0xEBC0//0xFE60 //Risk
#define PPM_COLOR_W (BLACK_MODE) ? RED : 0x8800//Warning
#define PPM_COLOR_A RED//MAROON//0xFA27 //Alarm
#define BAR_COLOR TEXT_COLOR// WHITE
#define BAR_SECTIONS_COLOR TEXT_COLOR // WHITE
#define DATABOX_BAR_COLOR TEXT_COLOR //WHITE
#define DEFAULT_COLOR TEXT_COLOR //WHITE
#define CHART_BORDER_COLOR TEXT_COLOR
#define TEXT_COLOR (BLACK_MODE) ? TEXT_COLOR_BLACK : TEXT_COLOR_LIGHT

#define CHART_BACKGROUND_COLOR BACKGROUND_COLOR
#define BACKGROUND_COLOR_LIGHT WHITE
#define BACKGROUND_COLOR_BLACK BLACK
#define TEXT_COLOR_LIGHT BLACK
#define TEXT_COLOR_BLACK WHITE

#define SLIDER_COLOR_ON GREEN
#define SLIDER_COLOR_ON_STRIP DARKGREEN
#define SLIDER_COLOR_OFF RED
#define SLIDER_COLOR_BACKGROUND GREY
