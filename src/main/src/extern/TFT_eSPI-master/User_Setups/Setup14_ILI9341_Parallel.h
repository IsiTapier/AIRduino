// See SetupX_Template.h for all options available

#define TFT_PARALLEL_8_BIT


#define ILI9341_DRIVER


// ESP32 pins used for the parallel interface TFT

#define TFT_RST  25//32  // Reset pin
#define TFT_CS   26//33  // Chip select control pin
#define TFT_DC   27//25  // Data Command control pin - must use a pin in the range 0-31

#define TFT_WR   14//26  // Write strobe control pin - must use a pin in the range 0-31
#define TFT_RD   12//27

#define TFT_BL   13//14// 32            // LED back-light control pin

#define TFT_D2   19
#define TFT_D3   18
#define TFT_D4   5
#define TFT_D5   17
#define TFT_D6   16
#define TFT_D7   4
#define TFT_D0   2  // Must use pins in the range 0-31 for the data bus
#define TFT_D1   15  // so a single register write sets/clears all bits

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT
