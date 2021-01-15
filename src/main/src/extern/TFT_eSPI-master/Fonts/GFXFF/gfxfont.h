// Adopted by Bodmer to support TFT_eSPI library.

// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#ifndef _GFXFONT_H_
#define _GFXFONT_H_

#ifdef LOAD_GFXFF

typedef struct { // Data stored PER GLYPH
	uint32_t bitmapOffset;     // Pointer into GFXfont->bitmap
	uint8_t  width, height;    // Bitmap dimensions in pixels
	uint8_t  xAdvance;         // Distance to advance cursor (x axis)
	int8_t   xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;

typedef struct { // Data stored for FONT AS A WHOLE:
	uint8_t  *bitmap;      // Glyph bitmaps, concatenated
	GFXglyph *glyph;       // Glyph array
	uint16_t  first, last; // ASCII extents
	uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;

  // Original Adafruit_GFX "Free Fonts"
  #include "TomThumb.h"  // TT1

  #include "FreeMono9pt7b.h"  // FF1 or FM9
  #include "FreeMono12pt7b.h" // FF2 or FM12
  #include "FreeMono18pt7b.h" // FF3 or FM18
  #include "FreeMono24pt7b.h" // FF4 or FM24

  #include "FreeMonoOblique9pt7b.h"  // FF5 or FMO9
  #include "FreeMonoOblique12pt7b.h" // FF6 or FMO12
  #include "FreeMonoOblique18pt7b.h" // FF7 or FMO18
  #include "FreeMonoOblique24pt7b.h" // FF8 or FMO24

  #include "FreeMonoBold9pt7b.h"  // FF9  or FMB9
  #include "FreeMonoBold12pt7b.h" // FF10 or FMB12
  #include "FreeMonoBold18pt7b.h" // FF11 or FMB18
  #include "FreeMonoBold24pt7b.h" // FF12 or FMB24

  #include "FreeMonoBoldOblique9pt7b.h"  // FF13 or FMBO9
  #include "FreeMonoBoldOblique12pt7b.h" // FF14 or FMBO12
  #include "FreeMonoBoldOblique18pt7b.h" // FF15 or FMBO18
  #include "FreeMonoBoldOblique24pt7b.h" // FF16 or FMBO24

  // Sans serif fonts
  #include "FreeSans9pt7b.h"  // FF17 or FSS9
  #include "FreeSans12pt7b.h" // FF18 or FSS12
  #include "FreeSans18pt7b.h" // FF19 or FSS18
  #include "FreeSans24pt7b.h" // FF20 or FSS24

  #include "FreeSansOblique9pt7b.h"  // FF21 or FSSO9
  #include "FreeSansOblique12pt7b.h" // FF22 or FSSO12
  #include "FreeSansOblique18pt7b.h" // FF23 or FSSO18
  #include "FreeSansOblique24pt7b.h" // FF24 or FSSO24

  #include "FreeSansBold9pt7b.h"  // FF25 or FSSB9
  #include "FreeSansBold12pt7b.h" // FF26 or FSSB12
  #include "FreeSansBold18pt7b.h" // FF27 or FSSB18
  #include "FreeSansBold24pt7b.h" // FF28 or FSSB24

  #include "FreeSansBoldOblique9pt7b.h"  // FF29 or FSSBO9
  #include "FreeSansBoldOblique12pt7b.h" // FF30 or FSSBO12
  #include "FreeSansBoldOblique18pt7b.h" // FF31 or FSSBO18
  #include "FreeSansBoldOblique24pt7b.h" // FF32 or FSSBO24

  // Serif fonts
  #include "FreeSerif9pt7b.h"  // FF33 or FS9
  #include "FreeSerif12pt7b.h" // FF34 or FS12
  #include "FreeSerif18pt7b.h" // FF35 or FS18
  #include "FreeSerif24pt7b.h" // FF36 or FS24

  #include "FreeSerifItalic9pt7b.h"  // FF37 or FSI9
  #include "FreeSerifItalic12pt7b.h" // FF38 or FSI12
  #include "FreeSerifItalic18pt7b.h" // FF39 or FSI18
  #include "FreeSerifItalic24pt7b.h" // FF40 or FSI24

  #include "FreeSerifBold9pt7b.h"  // FF41 or FSB9
  #include "FreeSerifBold12pt7b.h" // FF42 or FSB12
  #include "FreeSerifBold18pt7b.h" // FF43 or FSB18
  #include "FreeSerifBold24pt7b.h" // FF44 or FSB24

  #include "FreeSerifBoldItalic9pt7b.h"  // FF45 or FSBI9
  #include "FreeSerifBoldItalic12pt7b.h" // FF46 or FSBI12
  #include "FreeSerifBoldItalic18pt7b.h" // FF47 or FSBI18
  #include "FreeSerifBoldItalic24pt7b.h" // FF48 or FSBI24

#endif // LOAD_GFXFF

#endif // _GFXFONT_H_