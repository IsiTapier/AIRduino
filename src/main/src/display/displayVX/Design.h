//general
#define ROTATION 3
#define DISPLAY_BRIGHTNESS 0.7
#define LOADING_SCREEN_TIME 1

//settings
#define COLOR_MODE false
#define COLORED_BAR true
#define COLORED_STATE true
#define COLORED_TIME true
#define COLORED_PPM true
#define COLORED_CHART true

#define DISPLAYED_PPM_LOWEST  400
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
//Status Info
#define STATUS_LENGTH              round(DISPLAY_LENGTH * 0.8)
#define STATUS_HEIGHT              round(DISPLAY_HEIGHT * 0.13)
#define STATUS_MARGIN_TOP          DISPLAY_MARGIN_TOP
#define STATUS_MARGIN_BOTTOM       DISPLAY_MARGIN_BOTTOM
#define STATUS_SIZE                SIZE(STATUS_LENGTH, STATUS_HEIGHT, state.getTitle().length())
#define LAST_STATUS_SIZE           SIZE(STATUS_LENGTH, STATUS_HEIGHT, lastState.getTitle().length())
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
#define SS  STATUS_SIZE
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


#define BAR_SECTIONS_THICKNESS 2

//loadingScreen
#define LOADING_SCREEN_TITLE_SIZE 12
#define LOADING_SCREEN_SUB_SIZE 8
#define LOADING_SCREEN_DOTS_COLOR WHITE
#define LOADING_SCREEN_DOT_SIZE 10
