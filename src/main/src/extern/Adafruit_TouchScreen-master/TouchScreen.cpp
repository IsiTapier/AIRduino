// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// Code under MIT License

#include "Arduino.h"
#include "pins_arduino.h"

#ifdef __AVR
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif
#include "TouchScreen.h"

// increase or decrease the touchscreen oversampling. This is a little different
// than you make think: 1 is no oversampling, whatever data we get is
// immediately returned 2 is double-sampling and we only return valid data if
// both points are the same 3+ uses insert sort to get the median value. We
// found 2 is precise yet not too slow so we suggest sticking with it!

#define NUMSAMPLES 2

TSPoint::TSPoint(void) { x = y = z = 0; }

TSPoint::TSPoint(int16_t x0, int16_t y0, int16_t z0) {
  x = x0;
  y = y0;
  z = z0;
  xc = 0;
  yc = 0;
  xmin = 0;
  xmax = 0;
  ymin = 0;
  ymax = 0;
}

TSPoint::TSPoint(int16_t x0, int16_t y0, int16_t z0, TouchScreen *ts) {
  x = x0;
  y = y0;
  z = z0;
  xc = 0;
  yc = 0;
  xmin = ts->xmin;
  xmax = ts->xmax;
  ymin = ts->ymin;
  ymax = ts->ymax;
}

void TSPoint::calibrate() {
  xc = map(x, xmin, xmax, TOUCH_CALIBRATION_BOX_MARGIN + TOUCH_CALIBRATION_BOX_SIZE/2, DISPLAY_LENGTH - TOUCH_CALIBRATION_BOX_MARGIN - TOUCH_CALIBRATION_BOX_SIZE/2);
  yc = map(y, ymin, ymax, TOUCH_CALIBRATION_BOX_MARGIN + TOUCH_CALIBRATION_BOX_SIZE/2, DISPLAY_HEIGHT - TOUCH_CALIBRATION_BOX_MARGIN - TOUCH_CALIBRATION_BOX_SIZE/2);
}

void TSPoint::print() {
  Serial.print("point: ");
  Serial.print("\traw: ");
  Serial.print(" X = "); Serial.print(x);
  Serial.print(" Y = "); Serial.print(y);
  Serial.print("\tcalibrated: ");
  Serial.print(" X = "); Serial.print(xc);
  Serial.print(" Y = "); Serial.print(yc);
  Serial.print(" Pressure = "); Serial.println(z);
}

boolean TSPoint::isTouching(int startx, int endx, int starty, int endy) {
  if (z < MINPRESSURE || z > MAXPRESSURE) {
    if((xc + TOUCH_INACCURACY >= startx && xc - TOUCH_INACCURACY <= endx || xc == 0) && (yc + TOUCH_INACCURACY >= starty && yc - TOUCH_INACCURACY <= endy || yc == 0)) {
      return(true);
    }
  }
  return(false);
}

/**
 * @brief Check if the current point is **not** equivalent to another point
 *
 * @param p1 The other point being checked for equivalence
 * @return `true` : the two points are equivalent
 * `false`: the two points are **not** equivalent
 */
bool TSPoint::operator==(TSPoint p1) {
  return ((p1.x == x) && (p1.y == y) && (p1.z == z));
}
/**
 * @brief Check if the current point is **not** equivalent to another point
 *
 * @param p1 The other point being checked for equivalence

 * @return `true` :the two points are **not** equivalent
 * `false`: the two points are equivalent
 */
bool TSPoint::operator!=(TSPoint p1) {
  return ((p1.x != x) || (p1.y != y) || (p1.z != z));
}

#if (NUMSAMPLES > 2)
static void insert_sort(int array[], uint8_t size) {
  uint8_t j;
  int save;

  for (int i = 1; i < size; i++) {
    save = array[i];
    for (j = i; j >= 1 && save < array[j - 1]; j--)
      array[j] = array[j - 1];
    array[j] = save;
  }
}
#endif
/**
 * @brief Measure the X, Y, and pressure and return a TSPoint with the
 * measurements
 *
 * @return TSPoint The measured X, Y, and Z/pressure values
 */
TSPoint TouchScreen::getPoint(void) {

  int x, y, z;
  int samples[NUMSAMPLES];
  uint8_t i, valid;

  valid = 1;

  pinMode(_yp, INPUT);
  pinMode(_ym, INPUT);
  pinMode(_xp, OUTPUT);
  pinMode(_xm, OUTPUT);

#if defined(USE_FAST_PINIO)
  *xp_port |= xp_pin;
  *xm_port &= ~xm_pin;
#else
  digitalWrite(_xp, HIGH);
  digitalWrite(_xm, LOW);
#endif

#ifdef __arm__
  delayMicroseconds(20); // Fast ARM chips need to allow voltages to settle
#endif

  for (i = 0; i < NUMSAMPLES; i++) {
    samples[i] = analogRead(_yp);
  }

#if NUMSAMPLES > 2
  insert_sort(samples, NUMSAMPLES);
#endif
#if NUMSAMPLES == 2
  // Allow small amount of measurement noise, because capacitive
  // coupling to a TFT display's signals can induce some noise.
  if (samples[0] - samples[1] < -4 || samples[0] - samples[1] > 4) {
    valid = 0;
  } else {
    samples[1] = (samples[0] + samples[1]) >> 1; // average 2 samples
  }
#endif

  x = (1023 - samples[NUMSAMPLES / 2]);

  pinMode(_xp, INPUT);
  pinMode(_xm, INPUT);
  pinMode(_yp, OUTPUT);
  pinMode(_ym, OUTPUT);

#if defined(USE_FAST_PINIO)
  *ym_port &= ~ym_pin;
  *yp_port |= yp_pin;
#else
  digitalWrite(_ym, LOW);
  digitalWrite(_yp, HIGH);
#endif

#ifdef __arm__
  delayMicroseconds(20); // Fast ARM chips need to allow voltages to settle
#endif

  for (i = 0; i < NUMSAMPLES; i++) {
    samples[i] = analogRead(_xm);
  }

#if NUMSAMPLES > 2
  insert_sort(samples, NUMSAMPLES);
#endif
#if NUMSAMPLES == 2
  // Allow small amount of measurement noise, because capacitive
  // coupling to a TFT display's signals can induce some noise.
  if (samples[0] - samples[1] < -4 || samples[0] - samples[1] > 4) {
    valid = 0;
  } else {
    samples[1] = (samples[0] + samples[1]) >> 1; // average 2 samples
  }
#endif

  y = (1023 - samples[NUMSAMPLES / 2]);

  // Set X+ to ground
  // Set Y- to VCC
  // Hi-Z X- and Y+
  pinMode(_xp, OUTPUT);
  pinMode(_yp, INPUT);

#if defined(USE_FAST_PINIO)
  *xp_port &= ~xp_pin;
  *ym_port |= ym_pin;
#else
  digitalWrite(_xp, LOW);
  digitalWrite(_ym, HIGH);
#endif

  int z1 = analogRead(_xm);
  int z2 = analogRead(_yp);

  if (_rxplate != 0) {
    // now read the x
    float rtouch;
    rtouch = z2;
    rtouch /= z1;
    rtouch -= 1;
    rtouch *= x;
    rtouch *= _rxplate;
    rtouch /= 1024;

    z = rtouch;
  } else {
    z = (1023 - (z2 - z1));
  }

  if (!valid) {
    z = 0;
  }

  pinMode(_xp, OUTPUT);
  pinMode(_xm, OUTPUT);
  pinMode(_yp, OUTPUT);
  pinMode(_ym, OUTPUT);
  return TSPoint(x, y, z, this);
}

TouchScreen::TouchScreen() {}

TouchScreen::TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rxplate) {
  _yp = yp;
  _xm = xm;
  _ym = ym;
  _xp = xp;
  _rxplate = rxplate;
  xmin = 0;
  xmax = 0;
  ymin = 0;
  ymax = 0;
#if defined(USE_FAST_PINIO)
  xp_port = portOutputRegister(digitalPinToPort(_xp));
  yp_port = portOutputRegister(digitalPinToPort(_yp));
  xm_port = portOutputRegister(digitalPinToPort(_xm));
  ym_port = portOutputRegister(digitalPinToPort(_ym));

  xp_pin = digitalPinToBitMask(_xp);
  yp_pin = digitalPinToBitMask(_yp);
  xm_pin = digitalPinToBitMask(_xm);
  ym_pin = digitalPinToBitMask(_ym);
#endif
  pressureThreshhold = 10;
}

TouchScreen TouchScreen::operator=(TouchScreen touchscreen)  {
  pressureThreshhold = touchscreen.pressureThreshhold;
  xmin = touchscreen.xmin;
  xmax = touchscreen.xmax;
  ymin = touchscreen.ymin;
  ymax = touchscreen.ymax;
  _yp = touchscreen._yp;
  _ym = touchscreen._ym;
  _xm = touchscreen._xm;
  _xp = touchscreen._xp;
  _rxplate = touchscreen._rxplate;
  return(*this);
}

boolean TouchScreen::isTouching(void) {
  return(getPoint().isTouching());
}

void TouchScreen::calibration(void) {
  display.setRotation(ROTATION);
  //get upper left corner
  display.fillRect(TOUCH_CALIBRATION_BOX_MARGIN, TOUCH_CALIBRATION_BOX_MARGIN, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_CALIBRATION_COLOR);
  display.fillRect(TOUCH_CALIBRATION_BOX_CHANGE_X, TOUCH_CALIBRATION_BOX_MARGIN, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_COLOR);
  display.fillRect(TOUCH_CALIBRATION_BOX_MARGIN, TOUCH_CALIBRATION_BOX_CHANGE_Y, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_COLOR);
  display.fillRect(TOUCH_CALIBRATION_BOX_CHANGE_X, TOUCH_CALIBRATION_BOX_CHANGE_Y, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_COLOR);
  TSPoint topleft = getPoint();
  while(topleft.isTouching()) {
    delay(10);
    yield();
    topleft = getPoint();
  }
  delay(1000);
  while(!topleft.isTouching()) {
    delay(100);
    topleft = getPoint();
  }

  //get upper right corner
  display.fillRect(TOUCH_CALIBRATION_BOX_MARGIN, TOUCH_CALIBRATION_BOX_MARGIN, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_COLOR);
  display.fillRect(TOUCH_CALIBRATION_BOX_CHANGE_X, TOUCH_CALIBRATION_BOX_MARGIN, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_CALIBRATION_COLOR);
  TSPoint topright = topleft;
  while(topright.isTouching()) {
    delay(10);
    yield();
    topright = getPoint();
  }
  delay(1000);
  while(!topright.isTouching()) {
    delay(100);
    yield();
    topright = getPoint();
  }

  //get lower left corner
  display.fillRect(TOUCH_CALIBRATION_BOX_CHANGE_X, TOUCH_CALIBRATION_BOX_MARGIN, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_COLOR);
  display.fillRect(TOUCH_CALIBRATION_BOX_MARGIN, TOUCH_CALIBRATION_BOX_CHANGE_Y, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_CALIBRATION_COLOR);
  TSPoint bottomleft = topright;
  while(bottomleft.isTouching()) {
    delay(10);
    yield();
    bottomleft = getPoint();
  }
  delay(1000);
  while(!bottomleft.isTouching()) {
    delay(100);
    yield();
    bottomleft = getPoint();
  }

  //get lower right corner
  display.fillRect(TOUCH_CALIBRATION_BOX_MARGIN, TOUCH_CALIBRATION_BOX_CHANGE_Y, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_COLOR);
  display.fillRect(TOUCH_CALIBRATION_BOX_CHANGE_X, TOUCH_CALIBRATION_BOX_CHANGE_Y, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_CALIBRATION_COLOR);
  TSPoint bottomright = bottomleft;
  while(bottomright.isTouching()) {
    delay(10);
    yield();
    bottomright = getPoint();
  }
  delay(1000);
  while(!bottomright.isTouching()) {
    delay(100);
    yield();
    bottomright = getPoint();
  }

  display.fillRect(TOUCH_CALIBRATION_BOX_CHANGE_X, TOUCH_CALIBRATION_BOX_CHANGE_Y, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_SIZE, TOUCH_CALIBRATION_BOX_COLOR);

  this->xmin = (topleft.x + bottomleft.x)/2;
  this->xmax = (topright.x + bottomright.x)/2;
  this->ymin = (topleft.y + topright.y)/2;
  this->ymax = (bottomleft.y + bottomright.y)/2;
  Serial.println("the calibration values are:");
  Serial.print("x:"); Serial.print("\t min: "); Serial.print(xmin); Serial.print(" max: "); Serial.println(xmax);
  Serial.print("y:"); Serial.print("\t min: "); Serial.print(ymin); Serial.print(" max: "); Serial.println(ymax);
}



/**
 * @brief Read the touch event's X value
 *
 * @return int the X measurement
 */
int TouchScreen::readTouchX(void) {
  pinMode(_yp, INPUT);
  pinMode(_ym, INPUT);
  digitalWrite(_yp, LOW);
  digitalWrite(_ym, LOW);

  pinMode(_xp, OUTPUT);
  digitalWrite(_xp, HIGH);
  pinMode(_xm, OUTPUT);
  digitalWrite(_xm, LOW);

  pinMode(_yp, OUTPUT);
  pinMode(_ym, OUTPUT);

  return (1023 - analogRead(_yp));
}
/**
 * @brief Read the touch event's Y value
 *
 * @return int the Y measurement
 */
int TouchScreen::readTouchY(void) {
  pinMode(_xp, INPUT);
  pinMode(_xm, INPUT);
  digitalWrite(_xp, LOW);
  digitalWrite(_xm, LOW);

  pinMode(_yp, OUTPUT);
  digitalWrite(_yp, HIGH);
  pinMode(_ym, OUTPUT);
  digitalWrite(_ym, LOW);

  pinMode(_xp, OUTPUT);
  pinMode(_xm, OUTPUT);

  return (1023 - analogRead(_xm));
}
/**
 * @brief Read the touch event's Z/pressure value
 *
 * @return int the Z measurement
 */
uint16_t TouchScreen::pressure(void) {
  // Set X+ to ground
  pinMode(_xp, OUTPUT);
  digitalWrite(_xp, LOW);

  // Set Y- to VCC
  pinMode(_ym, OUTPUT);
  digitalWrite(_ym, HIGH);

  // Hi-Z X- and Y+
  digitalWrite(_xm, LOW);
  pinMode(_xm, INPUT);
  digitalWrite(_yp, LOW);
  pinMode(_yp, INPUT);

  int z1 = analogRead(_xm);
  int z2 = analogRead(_yp);

  pinMode(_xm, OUTPUT);
  pinMode(_yp, OUTPUT);

  if (_rxplate != 0) {
    // now read the x
    float rtouch;
    rtouch = z2;
    rtouch /= z1;
    rtouch -= 1;
    rtouch *= readTouchX();
    rtouch *= _rxplate;
    rtouch /= 1024;

    return rtouch;
  } else {
    return (1023 - (z2 - z1));
  }
}
