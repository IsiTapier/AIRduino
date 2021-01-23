//SONDER ZEICHEN
#define AE String(char(142))
#define ae String(char(132))
#define OE String(char(153))
#define oe String(char(148))
#define UE String(char(154))
#define ue String(char(129))
//#define ss String(char(225))
#define ag String(char(138))
#define aa String(char(160))
#define ac String(char(131))
#define eg String(char(138))
#define ea String(char(130))
#define ec String(char(136))
#define ep String(char(137))
#define og String(char(149))
#define oa String(char(162))
#define oc String(char(147))
#define ig String(char(141))
#define ia String(char(161))
#define ic String(char(140))
#define ip String(char(139))
#define ug String(char(151))
#define ua String(char(163))
#define uc String(char(150))
#define cc String(char(135))
#define CC String(char(128))


//general
#define ROTATION 3
#define DISPLAY_BRIGHTNESS 0.7
#define LOADING_SCREEN_TIME 1

//settings
#define STAGE_TIME 1*800
#define VENTILATING_TIMEOUT 5*1000

#define AVERAGING_MEASUREMENTS 10000
#define AVERAGING_GRADIENT 10
#define ALPHA_MEASUREMENTS 0.7
#define ALPHA_LOWEST 0.5

#define MAX_INCREASE 1.5
#define MAX_DECREASE 0.9
#define MAX_INCREASE_LOWEST 50

#define TOUCH_RESISTANCE 10000000

#define MINPRESSURE -1
#define MAXPRESSURE 0

#define TS_MINX -2300
#define TS_MINY -2400
#define TS_MAXX 740
#define TS_MAXY 540

#define TOUCH_INACCURACY 5
#define TOUCH_INACCURACY_X TOUCH_INACCURACY
#define TOUCH_INACCURACY_Y TOUCH_INACCURACY

//DisplayV1
#define AVERAGING_GRAPH 9
#define ALPHA_GRAPH 0.7

//DisplayV2

//settings
#define COLOR_MODE colorModes::variousColors.getValue()
#define COLORED_BAR colorModes::coloredBar.getValue()
#define COLORED_STATE colorModes::coloredState.getValue()
#define COLORED_TIME colorModes::coloredTime.getValue()
#define COLORED_PPM colorModes::coloredValue.getValue()
#define COLORED_CHART colorModes::coloredChart.getValue()


#define DISPLAYED_PPM_LOWEST 400
#define DISPLAYED_PPM_HIGHEST 1000


//display
#define DISPLAY_LENGTH             320
#define DISPLAY_HEIGHT             240
#define DISPLAY_MARGIN             0
#define DISPLAY_MARGIN_HORIZONTAL  (DISPLAY_MARGIN + round(DISPLAY_LENGTH * 0.025))
#define DISPLAY_MARGIN_VERTICAL    (DISPLAY_MARGIN + round(DISPLAY_HEIGHT * 0.033))
#define DISPLAY_MARGIN_LEFT        DISPLAY_MARGIN_HORIZONTAL
#define DISPLAY_MARGIN_RIGHT       DISPLAY_MARGIN_HORIZONTAL
#define DISPLAY_MARGIN_TOP         DISPLAY_MARGIN_VERTICAL
#define DISPLAY_MARGIN_BOTTOM      DISPLAY_MARGIN_VERTICAL
//font
#define LETTER_LENGTH              6
#define LETTER_HEIGHT              8

//TOOUCH CALIBRATION
#define TOUCH_CALIBRATION_BOX_MARGIN 5
#define TOUCH_CALIBRATION_BOX_SIZE 10
#define TOUCH_CALIBRATION_BOX_COLOR RED
#define TOUCH_CALIBRATION_BOX_CALIBRATION_COLOR BLUE
#define TOUCH_CALIBRATION_BOX_CHANGE (TOUCH_CALIBRATION_BOX_SIZE + TOUCH_CALIBRATION_BOX_MARGIN)
#define TOUCH_CALIBRATION_BOX_CHANGE_X (DISPLAY_LENGTH - TOUCH_CALIBRATION_BOX_CHANGE)
#define TOUCH_CALIBRATION_BOX_CHANGE_Y (DISPLAY_HEIGHT - TOUCH_CALIBRATION_BOX_CHANGE)
#define TOUCH_CALIBRATION_HEADER_SIZE SIZE(DISPLAY_LENGTH, round(DISPLAY_HEIGHT*0.3), 11)
#define TOUCH_CALIBRATION_TEXT_SIZE SIZE(DISPLAY_LENGTH, round(DISPLAY_HEIGHT*0.2), 14)

//MENU general
#define MENU_TITLE_SIZE SIZE(STATUS_LENGTH, STATUS_HEIGHT, 4)
#define MENU_SECTIONS 6
#define MENU_HEIGHT (DISPLAY_HEIGHT - STATUS_END_HEIGHT)
#define MENU_SECTION_LENGTH (DISPLAY_LENGTH - MENU_SIDE_LENGTH -1)
#define MENU_SECTION_HEIGHT floor(MENU_HEIGHT/MENU_SECTIONS)
#define MENU_MARGIN_LEFT (DISPLAY_MARGIN_LEFT*2)
#define MENU_MARGIN_RIGHT (DISPLAY_MARGIN_RIGHT*2)
#define MENU_MARGIN_VERTICAL (DISPLAY_MARGIN_VERTICAL)
#define MENU_SECTION_SIZE SIZE((MENU_SECTION_LENGTH - MENU_SLIDER_LENGTH - 2*MENU_MARGIN_RIGHT), MENU_SECTION_TEXT_HEIGHT, text.length())
#define MENU_SECTION_OLD_SIZE SIZE((MENU_SECTION_LENGTH - MENU_SIDE_LENGTH - 2*MENU_MARGIN_RIGHT), MENU_SECTION_TEXT_HEIGHT, oldText.length())

#define MENU_SECTION_Y(section) (section*MENU_SECTION_HEIGHT + STATUS_END_HEIGHT - MENU_SECTION_BORDER_SHIFT)
#define MENU_SECTION_END_Y(section) (MENU_SECTION_Y(section) + MENU_SECTION_HEIGHT)
#define MENU_SECTION_BORDER_THICKNESS 2
#define MENU_SECTION_BORDER_SHIFT floor(MENU_SECTION_BORDER_THICKNESS/2)
#define MENU_SECTION_TEXT_Y(section) (MENU_SECTION_Y(section) + round((MENU_SECTION_HEIGHT + MENU_SECTION_SIZE)/2))
#define MENU_SECTION_TEXT_HEIGHT (MENU_SECTION_HEIGHT - 2*MENU_MARGIN_VERTICAL)
#define MENU_SECTION_HALF_HEIGHT round(MENU_SECTION_HEIGHT/2)
#define MENU_SECTION_CENTER(section) (MENU_SECTION_HALF_HEIGHT + MENU_SECTION_Y(section))

//MENU SIDE
#define MENU_SIDE_LENGTH round(DISPLAY_LENGTH*0.15)
#define MENU_SIDE_HEIGHT (DISPLAY_HEIGHT - STATUS_END_HEIGHT)
#define MENU_SIDE_X (DISPLAY_LENGTH - MENU_SIDE_LENGTH)
#define MENU_SIDE_Y (STATUS_END_HEIGHT)
#define MENU_SIDE_THICKNESS round(DISPLAY_LENGTH*0.01)
#define MENU_SIDE_CENTER_X (MENU_SIDE_X + round((MENU_SIDE_LENGTH - MENU_SIDE_THICKNESS)/2) + 2*MENU_SIDE_THICKNESS)
#define MENU_SIDE_CENTER_Y (MENU_SIDE_Y + round(MENU_SIDE_HEIGHT/2))
#define MENU_SIDE_BAR_X (MENU_SIDE_X + DISPLAY_MARGIN_LEFT + MENU_SIDE_THICKNESS)
#define MENU_SIDE_BAR_Y (MENU_SIDE_CENTER_Y - round(MENU_SIDE_BAR_THICKNESS/2))
#define MENU_SIDE_BAR_THICKNESS 4
#define MENU_SIDE_TEXT_LENGTH (MENU_SIDE_LENGTH - DISPLAY_MARGIN_LEFT - DISPLAY_MARGIN_RIGHT - MENU_SIDE_THICKNESS)
#define MENU_SIDE_SIZE SIZE(MENU_SIDE_TEXT_LENGTH, 0, String(_pages.size()).length())

//MENU ICONS
#define MENU_ICON_SIZE 32
#define MENU_ICON_LENGTH MENU_ICON_SIZE
#define MENU_ICON_HEIGHT MENU_ICON_SIZE
#define MENU_ICON_MARGIN round(DISPLAY_LENGTH*0.02)
#define MENU_ICON_MARGIN_HORIZONTAL MENU_ICON_MARGIN
#define MENU_ICON_MARGIN_VERTICAL MENU_ICON_MARGIN
#define MENU_ICON_MARGIN_LEFT MENU_ICON_MARGIN_HORIZONTAL
#define MENU_ICON_MARGIN_RIGHT MENU_ICON_MARGIN_HORIZONTAL
#define MENU_ICON_MARGIN_TOP MENU_ICON_MARGIN_VERTICAL
#define MENU_ICON_MARGIN_BOTTOM MENU_ICON_MARGIN_VERTICAL
#define MENU_ARROW_BACK_START_X (MENU_ARROW_BACK_END_X - MENU_ICON_LENGTH)
#define MENU_ARROW_BACK_START_Y (MENU_ICON_MARGIN_TOP)
#define MENU_ARROW_BACK_END_X (DISPLAY_LENGTH - MENU_ICON_MARGIN_RIGHT)
#define MENU_ARROW_BACK_END_Y (MENU_ARROW_BACK_START_Y + MENU_ICON_HEIGHT)
#define MENU_ARROW_RESET_START_X (MENU_ICON_MARGIN_LEFT)
#define MENU_ARROW_RESET_START_Y (MENU_ICON_MARGIN_TOP)
#define MENU_ARROW_RESET_END_X (MENU_ARROW_RESET_START_X + MENU_ICON_LENGTH)
#define MENU_ARROW_RESET_END_Y (MENU_ARROW_RESET_START_Y + MENU_ICON_HEIGHT)
#define MENU_ARROW_LEFT_START_X (MENU_ARROW_RESET_END_X + MENU_ICON_MARGIN_LEFT + MENU_ICON_MARGIN_RIGHT)
#define MENU_ARROW_LEFT_START_Y (MENU_ICON_MARGIN_TOP)
#define MENU_ARROW_LEFT_END_X (MENU_ARROW_LEFT_START_X + MENU_ICON_LENGTH)
#define MENU_ARROW_LEFT_END_Y (MENU_ARROW_LEFT_START_Y + MENU_ICON_HEIGHT)
#define MENU_ARROW_RIGHT_START_X (MENU_ARROW_RIGHT_END_X - MENU_ICON_LENGTH)
#define MENU_ARROW_RIGHT_START_Y (MENU_ICON_MARGIN_TOP)
#define MENU_ARROW_RIGHT_END_X (MENU_ARROW_BACK_START_X - MENU_ICON_MARGIN_RIGHT - MENU_ICON_MARGIN_LEFT)
#define MENU_ARROW_RIGHT_END_Y (MENU_ARROW_RIGHT_START_Y + MENU_ICON_HEIGHT)
#define MENU_ARROW_UP_START_X (MENU_SIDE_X + 2*MENU_ICON_MARGIN_LEFT)
#define MENU_ARROW_UP_START_Y (STATUS_END_HEIGHT + MENU_ICON_MARGIN_TOP)
#define MENU_ARROW_UP_END_X (MENU_ARROW_UP_START_X + MENU_ICON_LENGTH)
#define MENU_ARROW_UP_END_Y (MENU_ARROW_UP_START_Y + MENU_ICON_HEIGHT)
#define MENU_ARROW_DOWN_START_X (MENU_SIDE_X + 2*MENU_ICON_MARGIN_LEFT)
#define MENU_ARROW_DOWN_START_Y (MENU_ARROW_DOWN_END_Y - MENU_ICON_HEIGHT)
#define MENU_ARROW_DOWN_END_X (MENU_ARROW_DOWN_START_X + MENU_ICON_LENGTH)
#define MENU_ARROW_DOWN_END_Y (DISPLAY_HEIGHT - MENU_ICON_MARGIN_BOTTOM)
#define MENU_INPUT_ICON_SIZE 24
#define MENU_INPUT_ICON_LENGTH MENU_INPUT_ICON_SIZE
#define MENU_INPUT_ICON_HEIGHT MENU_INPUT_ICON_SIZE
#define MENU_INPUT_ICON_START_Y(section) round((MENU_SECTION_HEIGHT - MENU_INPUT_ICON_HEIGHT)/2 + MENU_SECTION_Y(section))
#define MENU_INPUT_ICON_END_Y(section) (MENU_INPUT_ICON_START_Y(section) + MENU_INPUT_ICON_HEIGHT)

//NUMBER
#define MENU_NUMBER_PLUS_START_X (MENU_NUMBER_PLUS_END_X - MENU_INPUT_ICON_LENGTH)
#define MENU_NUMBER_PLUS_END_X   (MENU_SIDE_X - MENU_MARGIN_RIGHT)
#define MENU_NUMBER_MINUS_START_X (MENU_NUMBER_MINUS_END_X - MENU_INPUT_ICON_LENGTH)
#define MENU_NUMBER_MINUS_END_X   (MENU_NUMBER_NUMBER_START_X - MENU_MARGIN_LEFT)
#define MENU_NUMBER_NUMBER_START_X (MENU_NUMBER_NUMBER_END_X - MENU_NUMBER_NUMBER_LENGTH)
#define MENU_NUMBER_NUMBER_END_X   (MENU_NUMBER_PLUS_START_X - MENU_MARGIN_RIGHT)
#define MENU_NUMBER_NUMBER_LENGTH (MENU_NUMBER_NUMBER_SIZE*(LETTER_LENGTH-1))
#define MENU_NUMBER_NUMBER_SIZE SIZE(0, MENU_SECTION_TEXT_HEIGHT, 1)
#define MENU_NUMBER_TITLE_LENGTH (MENU_NUMBER_MINUS_START_X - 2*MENU_MARGIN_LEFT)
#define MENU_NUMBER_TITLE_SIZE SIZE(MENU_NUMBER_TITLE_LENGTH, MENU_SECTION_TEXT_HEIGHT, text.length())

//SELECT
#define MENU_SELECT_LENGTH round(MENU_SECTION_LENGTH/2)
#define MENU_SELECT_SIZE SIZE(MENU_SELECT_LENGTH, MENU_SECTION_TEXT_HEIGHT, text.length())

//SLIDER
#define MENU_SLIDER_RADIUS floor(MENU_SECTION_HALF_HEIGHT/2)
#define MENU_SLIDER_LENGTH (MENU_SLIDER_HEIGHT*2)
#define MENU_SLIDER_HEIGHT (((int)MENU_SECTION_HALF_HEIGHT%2 == 0) ? (MENU_SECTION_HALF_HEIGHT+1) : MENU_SECTION_HALF_HEIGHT)
#define MENU_SLIDER_START_X (MENU_SLIDER_END_X - MENU_SLIDER_LENGTH)
#define MENU_SLIDER_END_X (MENU_SIDE_X - MENU_MARGIN_RIGHT)
#define MENU_SLIDER_START_Y(section) (MENU_SECTION_CENTER(section) - MENU_SLIDER_RADIUS)
#define MENU_SLIDER_END_Y(section) (MENU_SECTION_CENTER(section) + MENU_SLIDER_RADIUS)
#define MENU_SLIDER_DOT_RADIUS round(MENU_SECTION_HEIGHT/3)
#define MENU_SLIDER_DOT_Y(section) MENU_SECTION_CENTER(section)
#define MENU_SLIDER_DOT_START_X (MENU_SLIDER_START_X + MENU_SLIDER_RADIUS)
#define MENU_SLIDER_DOT_END_X (MENU_SLIDER_END_X - MENU_SLIDER_RADIUS)
#define MENU_SLIDER_DOT_X(value) ((value) ? MENU_SLIDER_DOT_END_X : MENU_SLIDER_DOT_START_X)
#define MENU_SLIDER_STRIP_COLOR(value, mode) (!mode ? SLIDER_COLOR_BACKGROUND : value ? SLIDER_COLOR_ON_STRIP : SLIDER_COLOR_BACKGROUND)
#define MENU_SLIDER_DOT_COLOR(value, mode) (!mode ? TEXT_COLOR : value ? SLIDER_COLOR_ON : SLIDER_COLOR_OFF)

//Status Info
#define STATUS_LENGTH              round(DISPLAY_LENGTH * 0.8)
#define STATUS_HEIGHT              round(DISPLAY_HEIGHT * 0.13)
#define STATUS_MARGIN_TOP          DISPLAY_MARGIN_TOP
#define STATUS_MARGIN_BOTTOM       DISPLAY_MARGIN_BOTTOM
#define STATUS_SIZE                SIZE(STATUS_LENGTH, STATUS_HEIGHT, state.getTitle().length())
#define LAST_STATUS_SIZE           SIZE(STATUS_LENGTH, STATUS_HEIGHT, lastState.getTitle().length())
#define TOP_BAR_HEIGHT             (STATUS_END_HEIGHT - TOP_BAR_THICKNESS)
#define TOP_BAR_THICKNESS          5
#define STATUS_END_HEIGHT          (STATUS_MARGIN_TOP + STATUS_HEIGHT + STATUS_MARGIN_BOTTOM)
//PPM
#define PPM_MARGIN_LEFT            DISPLAY_MARGIN_LEFT
#define PPM_MARGIN_BOTTOM          DISPLAY_MARGIN_BOTTOM
#define PPM_MARGIN_RIGHT           DISPLAY_MARGIN_RIGHT
#define PPM_LENGTH                 round(DISPLAY_LENGTH * 0.5)
#define PPM_HEIGHT                 round(DISPLAY_HEIGHT * 0.2)
#define PPM_Y                      round(DISPLAY_HEIGHT - (DATABOX_HEIGHT - PPM_SIZE*(LETTER_HEIGHT + 1))/2)
#define PPM_SIZE                   SIZE(0, PPM_HEIGHT, 0)
#define PPM_STRING_X               (PPM_MARGIN_LEFT + (3*LETTER_LENGTH - 1)*PPM_SIZE + PPM_MARGIN_RIGHT)
#define PPM_STRING_Y               (PPM_Y - PPM_MARGIN_BOTTOM)
#define PPM_STRING_SIZE            SIZE((PPM_LENGTH - PPM_STRING_X), 0, 3)
//Timer
#define TIMER_MARGIN_RIGHT         DISPLAY_MARGIN_RIGHT
#define TIMER_MARGIN_BOTTOM        DISPLAY_MARGIN_BOTTOM
#define TIMER_LENGTH               round(DISPLAY_LENGTH * 0.4)
#define TIMER_HEIGHT               round(DISPLAY_HEIGHT * 0.2)
#define TIMER_X                    (DISPLAY_LENGTH - TIMER_MARGIN_RIGHT)
#define TIMER_Y                    round(DISPLAY_HEIGHT - (DATABOX_HEIGHT - TIMER_SIZE*(LETTER_HEIGHT + 1))/2)
#define TIMER_SIZE                 SIZE(TIMER_LENGTH, TIMER_HEIGHT, 5)
//Databox
#define DATABOX_HEIGHT             (round(DISPLAY_HEIGHT * 0.3) - DATABOX_BAR_THICKNESS)
#define DATABOX_Y                  (DISPLAY_HEIGHT - DATABOX_HEIGHT)
#define DATABOX_BAR_Y              (DATABOX_Y - DATABOX_BAR_THICKNESS)
#define DATABOX_BAR_THICKNESS      round(DISPLAY_HEIGHT * 0.033)
#define DATABOX_MARGIN_TOP         DISPLAY_MARGIN_TOP

//V2
//Bar
#define BAR_BORDER_LENGTH          round(DISPLAY_LENGTH * 0.8)
#define BAR_BORDER_HEIGHT          round(DISPLAY_HEIGHT * 0.4)
#define BAR_MARGIN_LEFT            round(DISPLAY_LENGTH * 0.1)
#define BAR_MARGIN_RIGHT           round(DISPLAY_LENGTH * 0.1)
#define BAR_MARGIN_TOP             DISPLAY_MARGIN_TOP
#define BAR_MARGIN_BOTTOM          DISPLAY_MARGIN_BOTTOM
#define BAR_BORDER_X               BAR_MARGIN_LEFT
#define BAR_BORDER_Y               (STATUS_MARGIN_TOP + STATUS_HEIGHT + STATUS_MARGIN_BOTTOM + BAR_MARGIN_TOP)
#define BAR_BORDER_THICKNESS       round(DISPLAY_LENGTH * 0.01)
#define BAR_HEIGHT                 (BAR_BORDER_HEIGHT - 2*BAR_BORDER_THICKNESS)
#define BAR_LENGTH                 (BAR_BORDER_LENGTH - 2*BAR_BORDER_THICKNESS)
#define BAR_START_X                (BAR_BORDER_X + BAR_BORDER_THICKNESS)
#define BAR_Y                      (BAR_BORDER_Y + BAR_BORDER_THICKNESS)
#define BAR_END_X                  (BAR_LENGTH + BAR_START_X)
#define FIRST_SECTION_X            map(LIMIT_GOOD, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, BAR_START_X, BAR_END_X)
#define SECOND_SECTION_X           map(LIMIT_MEDIUM, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, BAR_START_X, BAR_END_X)
#define BAR_SECTIONS_LENGTH        round(DISPLAY_LENGTH * 0.02)
#define BAR_SECTIONS_HEIGHT        (BAR_SECTIONS_Y_MARGIN*2 + BAR_HEIGHT)
#define BAR_SECTIONS_X_MARGIN      floor(BAR_SECTIONS_LENGTH/2)
#define BAR_SECTIONS_Y_MARGIN      round(DISPLAY_HEIGHT * 0.06)
#define BAR_SECTIONS_Y             (BAR_Y - BAR_SECTIONS_Y_MARGIN)
#define FIRST_SECTION_START_X      (FIRST_SECTION_X - BAR_SECTIONS_X_MARGIN)
#define SECOND_SECTION_START_X     (SECOND_SECTION_X - BAR_SECTIONS_X_MARGIN)
#define BAR_SECTIONS_STRIPE_DISTANCE 5

#define GRAPH_HEIGHT               round(DISPLAY_HEIGHT * 0.5)
#define GRAPH_START_Y              (GRAPH_END_Y + GRAPH_HEIGHT)
#define GRAPH_END_Y                STATUS_END_HEIGHT
#define FIRST_SECTION_Y            map(LIMIT_GOOD, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, GRAPH_START_Y, GRAPH_END_Y) //GRAPH_START_Y - map(LIMIT_GOOD, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, 0, GRAPH_HEIGHT)
#define SECOND_SECTION_Y           map(LIMIT_MEDIUM, DISPLAYED_PPM_LOWEST, DISPLAYED_PPM_HIGHEST, GRAPH_START_Y, GRAPH_END_Y)
#define GRAPH_SECTIONS_STRIPE_DISTANCE 5

//SHORTS

//general
#define ROT ROTATION
#define DBR DISPLAY_BRIGHTNESS
#define LST LOADING_SCREEN_TIME
//display
#define DL  DISPLAY_LENGTH
#define DW  DISPLAY_HEIGHT
#define DHM DISPLAY_MARGIN_HORIZONTAL
#define DVM DISPLAY_MARGIN_VERTICAL
#define DLM DISPLAY_MARGIN_LEFT
#define DRM DISPLAY_MARGIN_RIGHT
#define DTM DISPLAY_MARGIN_TOP
#define DBM DISPLAY_MARGIN_BOTTOM
//font
#define LH  LETTER_HEIGHT
#define LW  LETTER_LENGTH
//Status Info
#define SH  STATUS_HEIGHT
#define SW  STATUS_LENGTH
#define STM STATUS_MARGIN_TOP
//#define SS  STATUS_SIZE
//PPM
#define PLM PPM_MARGIN_LEFT
#define PBM PPM_MARGIN_BOTTOM
#define PRM PPM_MARGIN_RIGHT
#define PH  PPM_HEIGHT
#define PW  PPM_LENGTH
#define PY  PPM_Y
#define PS  PPM_SIZE
#define PSX PPM_STRING_X
#define PSS PPM_STRING_SIZE


//MACROS
#define SIZE(width, height, letters) (                        \
  (width <= 0 && height <= 0 || height <= 0 && letters <= 0) ? 1 :           \
  (width <= 0) ?  GETSIZE(DISPLAY_LENGTH, height, 1) :  \
  (height <= 0) ? GETSIZE(width, DISPLAY_HEIGHT, letters) :   \
                  GETSIZE(width, height, letters))

#define GETSIZE(width, height, letters) max((int) min(floor(height / LETTER_HEIGHT), floor(width / (LETTER_LENGTH*letters - 1))), 1)


//loadingScreen
#define LOADING_SCREEN_TITLE_SIZE 12
#define LOADING_SCREEN_SUB_SIZE 8
#define LOADING_SCREEN_DOTS_COLOR WHITE
#define LOADING_SCREEN_DOT_SIZE 10
#define DD_MARGIN 2
