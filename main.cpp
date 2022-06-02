/* PROJECT:  MPRO Laboraufgabe 3
 *
 * DEVELOPERS: Jannik Kreucher
 *             Nick Diendorf
 *             Benedikt Wendling
 */
#include "mbed.h"
#include "mlme.h"
#include <chrono>
using namespace std::chrono;

typedef unsigned char byte;

#define SEGMENT_FIGURE_1 0x06
#define SEGMENT_FIGURE_2 0x5B
#define SEGMENT_FIGURE_3 0x4F
#define SEGMENT_FIGURE_4 0x66
#define SEGMENT_FIGURE_5 0x6D
#define SEGMENT_FIGURE_6 0x7D

#define SW_1 PA_9
#define SW_2 PC_7
#define SW_3 PB_6
#define SW_4 PA_7
#define SW_5 PA_6

#define SEGMENT_A PB_0
#define SEGMENT_B PC_1
#define SEGMENT_C PA_10
#define SEGMENT_D PB_3
#define SEGMENT_E PB_5
#define SEGMENT_F PB_4
#define SEGMENT_G PB_10
#define SEGMENT_DOT PA_8

#define MANIPULATE_CYCLE_LIMIT 300

DigitalIn bSW1(SW_1);
DigitalIn bSW2(SW_2);
DigitalIn bSW3(SW_3);
DigitalIn bSW5(SW_5);

BusOut bSegmentDisplay(SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E,
                       SEGMENT_F, SEGMENT_G);
DigitalOut bSegmentDot(SEGMENT_DOT);

microseconds CYCLE_TIME = 5ms;

enum { enIdle = 0, enDice, enFade };

byte bDiceState = enIdle;

byte bCurrentFigureIndex = 0;
byte bCurrentFigurePatternIndex = 0;

byte bCurrentBlinkTimeIndex = 0;
byte bCurrentBlinkTimePatternIndex = 0;
byte bCurrentBlinkTimeCycleCount = 0;
byte bBlinkTimeLimit = 1;

byte bCurrentPatternCycleCount = 0;
byte bPatternCycleMultiple = 10; // every 10 times 5ms

byte bManipulateOne = 0;
byte bManipulateSix = 0;

byte bDiagnoseMode = 0;

int bManipulateCycleCount = 0;

const byte aabDicePattern[4][6] = {
    {SEGMENT_FIGURE_1, SEGMENT_FIGURE_2, SEGMENT_FIGURE_3, SEGMENT_FIGURE_4,
     SEGMENT_FIGURE_5, SEGMENT_FIGURE_6},
    {SEGMENT_FIGURE_1, SEGMENT_FIGURE_6, SEGMENT_FIGURE_2, SEGMENT_FIGURE_5,
     SEGMENT_FIGURE_3, SEGMENT_FIGURE_4},
    {SEGMENT_FIGURE_1, SEGMENT_FIGURE_2, SEGMENT_FIGURE_5, SEGMENT_FIGURE_6,
     SEGMENT_FIGURE_3, SEGMENT_FIGURE_4},
    {SEGMENT_FIGURE_2, SEGMENT_FIGURE_4, SEGMENT_FIGURE_1, SEGMENT_FIGURE_6,
     SEGMENT_FIGURE_3, SEGMENT_FIGURE_5}};

const byte abManipulateOnePattern[6] = {SEGMENT_FIGURE_1, SEGMENT_FIGURE_2,
                                        SEGMENT_FIGURE_1, SEGMENT_FIGURE_3,
                                        SEGMENT_FIGURE_1, SEGMENT_FIGURE_4};
const byte abManipulateSixPattern[6] = {SEGMENT_FIGURE_6, SEGMENT_FIGURE_2,
                                        SEGMENT_FIGURE_6, SEGMENT_FIGURE_3,
                                        SEGMENT_FIGURE_6, SEGMENT_FIGURE_4};

const unsigned char aabBlinkTimeCyclePattern[4][8] = {
    {10, 20, 30, 40, 50, 60, 70, 80},
    {20, 25, 30, 35, 40, 45, 50, 55},
    {20, 40, 60, 80, 100, 120, 140, 160},
    {10, 15, 20, 30, 45, 60, 80, 100}};

Ticker stSysTick;

volatile char bStandby;

void nextFigurePattern(void) {
  bCurrentFigurePatternIndex++;

  if (bCurrentFigurePatternIndex > 3) {
    bCurrentFigurePatternIndex = 0;
  }
}

void nextFigure(void) {
  bCurrentFigureIndex++;

  if (bCurrentFigureIndex > 5) {
    bCurrentFigureIndex = 0;
  }
}

void nextBlinkTimePattern(void) {
  bCurrentBlinkTimePatternIndex++;

  if (bCurrentBlinkTimePatternIndex > 3) {
    bCurrentBlinkTimePatternIndex = 0;
  }
}

void nextBlinkTimeLimit(void) {
  bBlinkTimeLimit++;

  if (bBlinkTimeLimit > 7) {
    bBlinkTimeLimit = 1;
  }
}

void setManipulation(byte manipulateOne, byte manipulateSix) {
  bManipulateOne = manipulateOne;
  bManipulateSix = manipulateSix && !manipulateOne;

  if (bDiagnoseMode) {
    bSegmentDot = !(manipulateOne || manipulateSix);
  }

  bManipulateCycleCount = 0;
}

void updateDisplay(void) {
  if (bManipulateOne) {
    bSegmentDisplay = ~(abManipulateOnePattern[bCurrentFigureIndex]);
  }
  if (bManipulateSix) {
    bSegmentDisplay = ~(abManipulateSixPattern[bCurrentFigureIndex]);
  }
  if (!bManipulateOne && !bManipulateSix) {
    bSegmentDisplay =
        ~(aabDicePattern[bCurrentFigurePatternIndex][bCurrentFigureIndex]);
  }
}

void vTimer(void) { bStandby = 1; }

void vTaste(void) {
  if (bSW1) {
    if (bDiceState == enIdle) {
      bDiceState = enDice;
    }
  } else {
    if (bDiceState == enDice) {
      bDiceState = enFade;
    }

    // Manipulation
    if (bDiceState == enIdle) {
      // Manipulation
      if (bSW2) {
        setManipulation(0, 1);
      }

      if (bSW5) {
        setManipulation(1, 0);
      }

      bManipulateCycleCount++;

      if (bManipulateCycleCount >= MANIPULATE_CYCLE_LIMIT) {
        setManipulation(0, 0);
      }
    }
  }
}

void vBlink(void) {
  switch (bDiceState) {
  case enIdle:

    bCurrentPatternCycleCount = 0;
    bCurrentBlinkTimeIndex = 0;
    bCurrentBlinkTimeCycleCount = 0;

    break;
  case enDice:

    if (bCurrentPatternCycleCount >= bPatternCycleMultiple) {
      updateDisplay();
      bCurrentPatternCycleCount = 0;
    } else {
      bCurrentPatternCycleCount++;
    }

    break;
  case enFade:

    if (bCurrentBlinkTimeCycleCount >=
        aabBlinkTimeCyclePattern[bCurrentBlinkTimePatternIndex]
                                [bCurrentBlinkTimeIndex]) {
      updateDisplay();
      bCurrentBlinkTimeIndex++;
      bCurrentBlinkTimeCycleCount = 0;
    } else {
      bCurrentBlinkTimeCycleCount++;
    }

    if (bCurrentBlinkTimeIndex > bBlinkTimeLimit) {
      bDiceState = enIdle;
    }

    break;
  }
}

// main() runs in its own thread in the OS
int main() {
  // Check diagnose mode
  if (bSW3) {
    bDiagnoseMode = 1;
  }

  // Set segment display dot default to off
  bSegmentDot = 1;

  // Show first figure on segment display on start
  updateDisplay();

  stSysTick.attach(&vTimer, CYCLE_TIME);

  while (true) {
    vTaste();
    vBlink();

    // Next pseudo random figure
    nextFigure();

    // If idle then pseudo random pattern
    if (bDiceState == enIdle) {
      nextFigurePattern();
      nextBlinkTimePattern();

      // Pseudo random blink time limi
      nextBlinkTimeLimit();
    }

    while (bStandby == 0)
      ;
    bStandby = 0;
  }
}