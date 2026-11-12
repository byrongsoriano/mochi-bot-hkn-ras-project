#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "bitmaps.h" // Contains bitmap bytes AND frame pointer arrays

// ============================================================
// PIN DEFINITIONS (Seeed Studio XIAO ESP32-C3)
// ============================================================
#define PIN_SDA     D4
#define PIN_SCL     D5
#define TOUCH_PIN   D2
#define SPEAKER_PIN D1

// ============================================================
// DISPLAY INITIALIZATION
// ============================================================
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, PIN_SCL, PIN_SDA);

// ============================================================
// STATE & TIMING
// ============================================================
int normalFrame = 0;
int happyFrame = 0;
int surprisedFrame = 0;
int loveFrame = 0;

uint8_t reactionType = 0;   // Next reaction
uint8_t activeReaction = 0; // Current reaction

const uint8_t TOUCH_ACTIVE_STATE = HIGH;
bool wasTouched = false;
unsigned long lastTouchChange = 0;
const unsigned long TOUCH_DEBOUNCE_MS = 50;

unsigned long lastFrameTime = 0;
const unsigned long FRAME_INTERVAL_MS = 40;

unsigned long lastTouchTime = 0;
const unsigned long IDLE_TIMEOUT = 5000; // 5 seconds

const char idleText[] = "HKN x RAS";
const size_t textLen = sizeof(idleText) - 1;
unsigned long lastCharTime = 0;
const unsigned long CHAR_INTERVAL = 250;
size_t charIndex = 0;

// ============================================================
// SOUND FUNCTIONS
// ============================================================
void setupBuzzer() {
  ledcAttach(SPEAKER_PIN, 2000, 8);
  ledcWriteTone(SPEAKER_PIN, 0);
}

void playTone(int freq, int duration) {
  ledcWriteTone(SPEAKER_PIN, freq);
  delay(duration);
  ledcWriteTone(SPEAKER_PIN, 0);
}

void happySound() {
  playTone(988, 120);
  playTone(1318, 120);
  playTone(1760, 150);
}

void surprisedSound() {
  playTone(784, 90);
  playTone(1175, 90);
  playTone(1568, 140);
}

void loveSound() {
  playTone(659, 100);
  playTone(784, 100);
  playTone(988, 100);
  playTone(1318, 180);
}

// ============================================================
// ANIMATION DRAWING FUNCTIONS
// ============================================================
void drawFrame(const unsigned char* frame) {
  u8g2.clearBuffer();
  u8g2.drawXBMP(0, 0, 128, 64, frame);
  u8g2.sendBuffer();
}

void playAnimation(const unsigned char** frames, int frameCount, int &currentFrame) {
  unsigned long now = millis();
  if (now - lastFrameTime < FRAME_INTERVAL_MS) return;

  lastFrameTime = now;
  drawFrame(frames[currentFrame]);

  currentFrame++;
  if (currentFrame >= frameCount) {
    currentFrame = 0;
  }
}

void drawIdleAnimation() {
  if (millis() - lastCharTime >= CHAR_INTERVAL) {
    lastCharTime = millis();
    if (charIndex < textLen) {
      charIndex++;
    }
  }

  char currentText[16] = {0};
  strncpy(currentText, idleText, charIndex);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  
  int strWidth = u8g2.getStrWidth(currentText);
  int x = (128 - strWidth) / 2;
  int y = 40; 

  u8g2.drawStr(x, y, currentText);
  u8g2.sendBuffer();
}

// ============================================================
// SETUP & LOOP
// ============================================================
void setup(void) {
  pinMode(TOUCH_PIN, INPUT_PULLDOWN);
  Wire.begin(PIN_SDA, PIN_SCL);
  u8g2.begin();
  setupBuzzer();

  lastTouchTime = millis();
  drawFrame(normalFrames[0]);
}

void loop() {
  bool rawTouched = (digitalRead(TOUCH_PIN) == TOUCH_ACTIVE_STATE);
  static bool lastRawTouched = false;

  if (rawTouched != lastRawTouched) {
    lastTouchChange = millis();
    lastRawTouched = rawTouched;
  }

  // Debounce logic
  if ((millis() - lastTouchChange) >= TOUCH_DEBOUNCE_MS) {
    if (rawTouched != wasTouched) {
      wasTouched = rawTouched;

      if (wasTouched) {
        lastTouchTime = millis();
        charIndex = 0;
        activeReaction = reactionType;

        if (activeReaction == 0) {
          happyFrame = 0;
          drawFrame(happyFrames[happyFrame]);
          happySound();
        } else if (activeReaction == 1) {
          surprisedFrame = 0;
          drawFrame(surprisedFrames[surprisedFrame]);
          surprisedSound();
        } else {
          loveFrame = 0;
          drawFrame(loveFrames[loveFrame]);
          loveSound();
        }

        reactionType = (reactionType + 1) % 3;
      }
    }
  }

  // Animation rendering state machine
  if (wasTouched) {
    if (activeReaction == 0) {
      playAnimation(happyFrames, HAPPY_FRAME_COUNT, happyFrame);
    } else if (activeReaction == 1) {
      playAnimation(surprisedFrames, SURPRISED_FRAME_COUNT, surprisedFrame);
    } else {
      playAnimation(loveFrames, LOVE_FRAME_COUNT, loveFrame);
    }
  } else {
    // Return to idle typewriter mode if untouched for 5 seconds
    if (millis() - lastTouchTime >= IDLE_TIMEOUT) {
      drawIdleAnimation();
    } else {
      playAnimation(normalFrames, NORMAL_FRAME_COUNT, normalFrame);
    }
  }
}