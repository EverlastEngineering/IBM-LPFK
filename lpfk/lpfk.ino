// #define PATTERN_DESIGN_MODE // Comment out for normal HID operation

/*
 * IBM LPFK Arduino Controller
 *
 * Controls the IBM LPFK (Lighted Programmable Function Keyboard) via Arduino Leonardo.
 * - 32 keys, each with an LED (indexed 0-31, top-left to bottom-right)
 * - Supports USB HID keyboard emulation and per-key LED control
 * - Patterns (letters, shapes) are defined as macros and can be created interactively
 * - DaVinci mode has custom key mappings for video editing shortcuts
 * - Other modes are for visual effects only
 *
 * Pattern Design Mode:
 *   If PATTERN_DESIGN_MODE is defined, disables HID and enables Serial for pattern creation.
 *   Use the Arduino Serial Monitor to send commands and receive LED array dumps.
 *   When undefined, normal HID operation resumes.
 *
 * Connections:
 *   - softSerial (SoftwareSerialParity) is used for communication with the LPFK hardware.
 *     RX: Arduino pin 8
 *     TX: Arduino pin 9
 *   - Serial (or Keyboard) is over USB (Leonardo's native USB port).
 *     Serial: for pattern design and debugging
 *     Keyboard: for HID keyboard emulation
 *
 * Workflow for creating new patterns:
 *   1. Define PATTERN_DESIGN_MODE at the top of the sketch.
 *   2. Use serial commands to light up desired LEDs.
 *   3. Call dumpLightArrays [name] to print the current LED array.
 *   4. Copy the output and add as a macro (e.g., #define letterX ...).
 *   5. Undefine PATTERN_DESIGN_MODE for normal HID operation.
 *
 * // LPFK LED layout mapping (physical):
// Row 0: columns 1-4 (LEDs 0-3)
// Row 1: columns 0-5 (LEDs 4-9)
// Row 2: columns 0-5 (LEDs 10-15)
// Row 3: columns 0-5 (LEDs 16-21)
// Row 4: columns 0-5 (LEDs 22-27)
// Row 5: columns 1-4 (LEDs 28-31)
// Only these 32 positions are valid; other bits are 'missing'.

 * See README.md for more details
 */

// Animation frame structure
struct AnimationFrame
{
  byte pattern[4];
  int duration;
};


bool selectDeselectToggle = false;
int armTriggerCount = 0;
bool isArmed = false;

#define FRAME(pause, pat) {{pat}, pause}

// Example LED array dumps for reference
// LightArray: 0x27, 0x89, 0xA2, 0x74  // "D"
// LightArray: 0x0 0x70 0x92 0xE4      // "a"
// LightArray: 0x10 0x45 0x91 0x22
// LightArray: 0x40 0x0 0x4 0x21
// LightArray: 0x0 0x28 0x96 0x94
// LightArray: 0xC0 0x48 0x82 0x64
// LightArray: 0x40 0x0 0x4 0x21

// Off
#define allOff 0x0, 0x0, 0x0, 0x0

// LED pattern macros: each defines a 4-byte array for a letter or shape
#define letterD 0x27, 0x89, 0xA2, 0x74 // "D"
#define lettera 0x0, 0x70, 0x92, 0xE4  // "a"
#define letterV 0x29, 0x49, 0x12, 0x45 // "V"
#define letteri 0x40, 0x0, 0x4, 0x21   // "i"
#define lettern 0x0, 0x28, 0x96, 0x94  // "n"
#define letterc 0xC0, 0x48, 0x82, 0x64 // "c"
#define letterR 0x27, 0x49, 0x8E, 0x92 // "R"
#define lettere 0xC0, 0x48, 0x9E, 0x60 // "e"
#define letters 0xC0, 0x8, 0xC, 0x64   // "s"
#define lettero 0xC0, 0x48, 0x92, 0x64 // "o"
#define letterl 0x42, 0x10, 0x4, 0x21  // "l"
#define letterv 0x0, 0x88, 0x22, 0x45  // "v"

#define crosshatch1 0xAA, 0x56, 0x6A, 0x55 // crosshatch pattern
#define crosshatch2 0x55, 0xA9, 0x95, 0xAA // crosshatch pattern

#define letterL 0x21, 0x8, 0x82, 0xF0  // "L"
#define letterg 0xC0, 0x49, 0x9C, 0x64 // "g"
#define letterP 0x27, 0x49, 0x8E, 0x10 // "P"
#define letterr 0x0, 0x38, 0x92, 0x10  // "r"

#define letterE 0x2F, 0x78, 0x82, 0xF0 // "E"
#define lettert 0x84, 0x70, 0x8, 0x42  // "t"

#define letterC 0x26, 0x9, 0x82, 0x64  // "C"
#define letterd 0x8, 0x71, 0x92, 0xE4  // "d"
#define letterS 0x26, 0x19, 0x98, 0x64 // "S"

#define letterK 0xA9, 0x18, 0x86, 0x92
#define number4 0x29, 0xF9, 0x10, 0x84
#define number0 0x2E, 0x8A, 0xA2, 0xE8
#define letterL 0x21, 0x8, 0x82, 0xF0
#define letterr 0xA0, 0x99, 0x82, 0x10

// Laser patterns
#define laser1 0x0, 0x0, 0x1, 0x0
#define laser2 0x0, 0x0, 0x3, 0x0
#define laser3 0x0, 0x0, 0x7, 0x0
#define laser4 0x0, 0x0, 0xF, 0x0
#define laser5 0x0, 0x0, 0x1F, 0x0
#define laser6 0x0, 0x0, 0x3F, 0x0
#define laserhit1 0x0, 0x80, 0x3F, 0x8
#define laserhit2 0x0, 0x1, 0x3F, 0x80

#define laserb1 0x0, 0x4, 0x0, 0x0
#define laserb2 0x20, 0x4, 0x0, 0x0
#define laserb3 0x22, 0x4, 0x0, 0x0
#define laserb4 0x22, 0x0, 0x0, 0x0
#define laserb5 0x2, 0x0, 0x0, 0x0
#define laserc1 0x0, 0x0, 0x0, 0x20
#define laserc2 0x0, 0x0, 0x0, 0x21
#define laserc3 0x0, 0x0, 0x8, 0x21
#define laserc4 0x0, 0x20, 0x8, 0x21
#define laserc5 0x0, 0x21, 0x8, 0x21
#define laserc6 0x8, 0x21, 0x8, 0x21
#define laserc7 0x8, 0x21, 0x8, 0x1
#define laserc8 0x8, 0x21, 0x8, 0x0
#define laserc9 0x8, 0x21, 0x0, 0x0
#define laserc10 0x8, 0x1, 0x0, 0x0
#define laserc11 0x8, 0x0, 0x0, 0x0
#define laserc11 0x8, 0x0, 0x0, 0x0

#define lasere1 0x0, 0x80, 0x0, 0x0
#define lasere2 0x0, 0xC0, 0x0, 0x0
#define lasere3 0x0, 0xE0, 0x0, 0x0
#define lasere4 0x0, 0xF0, 0x0, 0x0
#define lasere5 0x0, 0xF8, 0x0, 0x0
#define lasere6 0x0, 0xFC, 0x0, 0x0
#define lasere7 0x10, 0xFC, 0x1, 0x0
#define lasere8 0x1, 0xFC, 0x80, 0x0
#define lasere9 0x10, 0x7C, 0x1, 0x0
#define lasere10 0x1, 0x3C, 0x80, 0x0
#define lasere11 0x10, 0x1C, 0x1, 0x0
#define lasere12 0x1, 0xC, 0x80, 0x0
#define lasere13 0x10, 0x4, 0x1, 0x0
#define lasere14 0x1, 0x0, 0x80, 0x0
#define laserea1 0x0, 0x7C, 0x0, 0x0
#define laserea2 0x0, 0x3C, 0x0, 0x0
#define laserea3 0x0, 0x1C, 0x0, 0x0
#define laserea4 0x0, 0xC, 0x0, 0x0
#define laserea5 0x0, 0x4, 0x0, 0x0

#define pat0 0x6, 0x0, 0x0, 0x60
#define pat1 0xF, 0x0, 0x0, 0xF0
#define pat2 0xF0, 0x3, 0xC0, 0xF
#define pat3 0x0, 0xFC, 0x3F, 0x0
#define pat4 0x0, 0x78, 0x1E, 0x0
#define pat5 0x0, 0x30, 0xC, 0x0
#define pat6 0xC0, 0x30, 0xC, 0x3
#define pat7 0xC6, 0x30, 0xC, 0x63
#define pat8 0x29, 0x49, 0x92, 0x94
#define pat9 0x10, 0x86, 0x61, 0x8
#define pat10 0x19, 0x86, 0x61, 0x98
#define pat11 0x1F, 0x86, 0x61, 0xF8
#define pat12 0xE0, 0x49, 0x92, 0x7

#define engraver1 0x10, 0x4, 0x0, 0x0
#define engraver2a 0x31, 0xC, 0x41, 0x0
#define engraver2b 0x31, 0xC, 0x0, 0x0
#define engraver3a 0x73, 0x1C, 0x82, 0x10
#define engraver3b 0x73, 0x1C, 0x0, 0x0
#define engraver4a 0xE7, 0x38, 0x4, 0x21
#define engraver4b 0xE7, 0x38, 0x0, 0x0
#define engraver5b 0xCE, 0x71, 0x0, 0x0
#define engraver5a 0xCE, 0x71, 0x8, 0x42
#define engraver6a 0x8C, 0xE3, 0x10, 0x84
#define engraver6b 0x8C, 0xE3, 0x0, 0x0
#define engraver7b 0x8, 0xC3, 0x0, 0x0
#define engraver7a 0x8, 0xC3, 0x20, 0x8
#define engraver8 0x0, 0x82, 0x0, 0x0

#define k1 0x1, 0x0, 0x0, 0x0
#define k2 0x21, 0x0, 0x0, 0x0
#define k3 0x29, 0x8, 0x0, 0x4
#define k4 0xA9, 0x8, 0xA, 0x4
#define k5 0xA9, 0x18, 0x8A, 0x4
#define k6 0xA0, 0x18, 0x8A, 0x4
#define k7 0x0, 0x18, 0x8A, 0x4
#define k8 0x0, 0x0, 0x8A, 0x4
#define k9 0x0, 0x0, 0x80, 0x4

#define foura 0x0, 0x80, 0x0, 0x50
#define fourb 0x0, 0xC0, 0x80, 0x52
#define fourc 0x0, 0xE0, 0x8A, 0x52
#define fourd 0x0, 0xF8, 0x8A, 0x42
#define foure 0xA0, 0xF8, 0xA, 0x42
#define fourf 0xA5, 0x78, 0x8, 0x2
// break
#define fourg 0xA4, 0x38, 0x8, 0x0
#define fourh 0x84, 0x18, 0x0, 0x0
#define fouri 0x4, 0x10, 0x0, 0x0

#define zero1 0x1, 0x0, 0x0, 0x4
#define zero2 0x21, 0x0, 0x10, 0x4
#define zero3 0x21, 0x1, 0x12, 0x4
#define zero3b 0x21, 0x48, 0x10, 0x4
#define zero4 0x23, 0x48, 0x10, 0x6
#define zero5 0x27, 0x48, 0x10, 0x7
#define zero6 0x2F, 0x48, 0x90, 0x7
#define zero7 0x2F, 0x49, 0x92, 0x7
#define zero8 0x2F, 0x1, 0x92, 0x7
#define zero9 0xF, 0x1, 0x82, 0x7
#define zero10 0xE, 0x1, 0x82, 0x3
#define zero11 0xC, 0x1, 0x82, 0x1
#define zero12 0x8, 0x1, 0x82, 0x0
#define zero13 0x0, 0x1, 0x2, 0x0

#define rzero1 0x0, 0x0, 0x0, 0x90
#define rzero2 0x0, 0x0, 0xC0, 0x94
#define rzero3 0x1, 0x0, 0xD2, 0x94
#define rzero4 0x3, 0x48, 0x92, 0x5
#define rzero5 0x26, 0x49, 0x12, 0x3 // on
#define rzero6 0x26, 0x48, 0x12, 0x3
#define rzero7 0x26, 0x8, 0x2, 0x3
#define rzero8 0x26, 0x8, 0x2, 0x1
#define rzero9 0x26, 0x8, 0x2, 0x0
#define rzero10 0x26, 0x8, 0x0, 0x0
#define rzero11 0x26, 0x0, 0x0, 0x0
#define rzero12 0x6, 0x0, 0x0, 0x0
#define rzero13 0x4, 0x0, 0x0, 0x0

#include <Keyboard.h>
#include "SoftwareSerialParity.h"

#define elementCount(x) (sizeof(x) / sizeof(x[0]))

// Custom serial class for odd parity communication with LPFK
SoftwareSerialParity softSerial(8, 9); // RX, TX

// lightArray holds the state of all 32 LEDs (4 bytes, 8 bits each)
byte lightArray[] = {0x00, 0x00, 0x00, 0x00};
boolean lightsStayOn = false; // If true, LEDs stay on after key press
int mode = 0;                 // Current mode: 0=DaVinci, 1=Logic Pro, 2=Visual Code, etc.
int keyDelay = 5;             // Delay between key presses/releases in ms

// DaVinci mode: keys to pre-light and map to shortcuts
int daVinciSet[] = {4, 5, 16, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31};
int k40Set[] = {11, 14, 23, 28};
// This array must match the keys mapped in fulfilKeyboardCommands()

unsigned long lastKeyTime = 0; // Tracks last key press time
bool screensaverActive = false;

float scaleFactor = 1.0; // movement scaling factor

void pressScaledKey() {
  pressScaledKey(scaleFactor);
}

// Helper: send direction key with scale modifiers
void pressScaledKey(float scaleFactor)
{
  if (scaleFactor == 0.1)
  {
    Keyboard.press(KEY_LEFT_ALT);
    delay(keyDelay);
    Keyboard.press(KEY_LEFT_SHIFT);
    delay(keyDelay);
  }
  else if (scaleFactor == 10.0)
  {
    Keyboard.press(KEY_LEFT_SHIFT);
    delay(keyDelay);
  }
  else if (scaleFactor == 20.0)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    delay(keyDelay);
    Keyboard.press(KEY_LEFT_SHIFT);
    delay(keyDelay);
    Keyboard.press(KEY_LEFT_ALT);
    delay(keyDelay);
  }
}

void releaseScaledKey()
{
  releaseScaledKey(scaleFactor);
}

void releaseScaledKey(float scaleFactor)
{
  if (scaleFactor == 0.1)
  {
    Keyboard.release(KEY_LEFT_SHIFT);
    delay(keyDelay);
    Keyboard.release(KEY_LEFT_ALT);
    delay(keyDelay);
  }
  else if (scaleFactor == 10.0)
  {
    Keyboard.release(KEY_LEFT_SHIFT);
    delay(keyDelay);
  }
  else if (scaleFactor == 20.0)
  {
    Keyboard.release(KEY_LEFT_ALT);
    delay(keyDelay);
    Keyboard.release(KEY_LEFT_SHIFT);
    delay(keyDelay);
    Keyboard.release(KEY_LEFT_CTRL);
    delay(keyDelay);
  }
}

// Helper: non-blocking delay that aborts on key press
bool screensaverDelay(unsigned long ms)
{
  unsigned long start = millis();
  while (millis() - start < ms)
  {
    if (Serial.available() > 0 || softSerial.read() != -1)
    {
      screensaverActive = false;
      return false;
    }
    delay(2); // Small sleep to avoid busy loop
  }
  return true;
}

// Helper: checks if a keypress is available on softSerial only (does not consume buffer)
bool isKeyPressed()
{
  int key = readAndReturn();
  if (key != -1 && key != 0x80 && key != 0x81)
    return true;
  return false;
}

// Keypress-aware delay: returns true if completed, false if aborted
bool delayKeyPressAware(unsigned long ms)
{
  unsigned long start = millis();
  while (millis() - start < ms)
  {
    if (isKeyPressed())
    {
      allLights(0); // Clear LED screen
      return false; // Aborted
    }
    delay(10); // More responsive increments
  }
  return true; // Completed
}

// Helper: play a random animation
bool playRandomAnimation()
{
  int pick = random(1, 5); // Add more if you want
  switch (pick)
  {
  case 1:
    return laserAnimation();
    break;
  case 2:
    return engraverAnimation();
    break;
  case 3:
    return patternAnimation();
    break;
  case 4:
    return lasereAnimation();
    break;
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); // onboard LED for status/debug
  softSerial.begin(9600, ODD);  // start serial with odd parity for LPFK
  delay(1000);
  softSerial.write(0x08); // 0x08: enable keyboard
  //  demoLoop(false,1);
  //  laserAnimation();       // run Pac-Man animation at startup
  mode = 1;
  flipBitInLightArray(mode, true, true); // light up mode indicator LED
#ifdef PATTERN_DESIGN_MODE
  Serial.begin(9600); // USB Serial for pattern design/debugging
#else
  Keyboard.begin(); // USB HID keyboard emulation
#endif
}

// Helper to generate sweep animation frames for demoLoop
void generateDemoLoopFrames(AnimationFrame *frames, int &frameCount, boolean keep, int stepDelay)
{
  byte sweepArray[4];
  int idx = 0;
  // Sweep left to right
  for (int i = 0; i < 32; i++)
  {
    if (!keep)
    {
      sweepArray[0] = 0;
      sweepArray[1] = 0;
      sweepArray[2] = 0;
      sweepArray[3] = 0;
    }
    int byteLoop = i / 8;
    sweepArray[byteLoop] |= (1 << (i - (byteLoop * 8)));
    frames[idx].pattern[0] = sweepArray[0];
    frames[idx].pattern[1] = sweepArray[1];
    frames[idx].pattern[2] = sweepArray[2];
    frames[idx].pattern[3] = sweepArray[3];
    frames[idx].duration = stepDelay;
    idx++;
  }
  // Sweep right to left
  for (int i = 31; i >= 0; i--)
  {
    if (!keep)
    {
      sweepArray[0] = 0;
      sweepArray[1] = 0;
      sweepArray[2] = 0;
      sweepArray[3] = 0;
    }
    int byteLoop = i / 8;
    if (keep)
    {
      sweepArray[byteLoop] &= ~(1 << (i - (byteLoop * 8)));
    }
    else
    {
      sweepArray[byteLoop] |= (1 << (i - (byteLoop * 8)));
    }
    frames[idx].pattern[0] = sweepArray[0];
    frames[idx].pattern[1] = sweepArray[1];
    frames[idx].pattern[2] = sweepArray[2];
    frames[idx].pattern[3] = sweepArray[3];
    frames[idx].duration = stepDelay;
    idx++;
  }
  // Final frame: all off
  frames[idx].pattern[0] = 0;
  frames[idx].pattern[1] = 0;
  frames[idx].pattern[2] = 0;
  frames[idx].pattern[3] = 0;
  frames[idx].duration = stepDelay;
  frameCount = idx + 1;
}

void demoLoop(boolean keep, int _delay)
{
  const int totalFrames = 32 + 32 + 1;
  AnimationFrame frames[totalFrames];
  int frameCount = 0;
  generateDemoLoopFrames(frames, frameCount, keep, _delay);
  runAnimation(frames, frameCount);
}

// Main loop: handles key events and serial commands
void loop()
{
#ifndef PATTERN_DESIGN_MODE
  if (!screensaverActive && millis() - lastKeyTime > 600000)
  {
    screensaverActive = true;
  }
  bool uninterupted = true;
  if (screensaverActive)
  {
    uninterupted = playRandomAnimation();
    if (uninterupted)
    {
      uninterupted = screensaverDelay(1000); // Wait 10s, abort if key pressed
    }
    screensaverActive = false;
    if (uninterupted)
    {
      lastKeyTime = millis() - 6000; // Reset timer after screensaver
    }
    else
    {
      lastKeyTime = millis(); // Reset timer after screensaver
    }
    // return;
  }
#endif
  // boolean keepReading = true;
  // while (keepReading)
  // {
    int received = readAndReturn(); // read key event from LPFK
    if (received != -1 && received != 129)
    {
      lastKeyTime = millis();    // Update last key time
      screensaverActive = false; // Abort screensaver
      // If lightsStayOn is true, keep LEDs on after key press
      if (lightsStayOn && received < 32)
      {
        // If key 9 is pressed and all LEDs are off, run demo
        if (received == 9 && lightArray[0] == 0 && lightArray[2] == 0 && lightArray[2] == 0 && lightArray[3] == 0)
        {
          demoLoop(false, 1);
          lightsStayOn = !lightsStayOn;
        }
        else
        {
          flipBitInLightArray(received);
          sendLights();
        }
      }
      // Mode switching and special patterns
      else if (received == 3)
      {
        everlastEngineering();
      }
      else if (received == 2)
      {
        visualCode();
      }
      #ifdef PATTERN_DESIGN_MODE
      else if (received == 9)
      {
        demoLoop(false, 1);
        lightsStayOn = !lightsStayOn;
      }
      #endif
      // else if (received == 8)
      // {
      //   logicPro(); // Logic Pro mode on button 8
      // }
      else if (received == 1)
      {
        k40Laser(); // Laser mode on button 1
      }
      else if (received == 0)
      {
        davinci();
      }
      // Normal key press: light LED and send keyboard shortcut (if in DaVinci mode)
      else if (received < 32)
      {
        flipBitInLightArray(received, 50); // causes led to blink briefly
        //        preLight();
        fulfilKeyboardCommands(received);
        sendLights();
      }
      // Special demo pattern
      else if (received == 28)
      {
        demoLoop(true, 1);
      }
      // If received is out of range, turn all LEDs on
      else
      {
        for (int i = 0; i < 4; i++)
        {
          lightArray[i] = ((byte)0xFF);
        }
      }
    // }
    // keepReading = false;
  }

  // Serial command interface for pattern creation and testing
  while (Serial.available() > 0)
  {
    String command = Serial.readStringUntil(' '); // reads command until space

    if (command == "demoMode")
    {
      String fillStr = Serial.readStringUntil(' ');
      String pauseStr = Serial.readStringUntil('\n');
      boolean fill = fillStr.toInt();
      long pause = pauseStr.toInt();
      if (pause != 0)
      {
        demoLoop(fill, pause);
      }
      else
      {
        //--Serial.println("incorrect parameters: demoMode 1 100 = demoMode 0, 100 ms delay per light");
      }
    }
    else if (command == "singleLight")
    {
      String lightStr = Serial.readStringUntil('\n');
      int light = -1;
      light = lightStr.toInt();
      if (light < 0 || light > 31)
      {
        //--Serial.println("incorrect parameters: singleLight 0-31");
      }
      else
      {
        flipBitInLightArray(light);
        sendLights();
      }
    }
    else if (command == "lightArray")
    {
      String ar0str = Serial.readStringUntil(' ');
      String ar1str = Serial.readStringUntil(' ');
      String ar2str = Serial.readStringUntil(' ');
      String ar3str = Serial.readStringUntil('\n');
      byte ar0 = (byte)ar0str.toInt();
      byte ar1 = (byte)ar1str.toInt();
      byte ar2 = (byte)ar2str.toInt();
      byte ar3 = (byte)ar3str.toInt();
      lightArray[0] = ar0;
      lightArray[1] = ar1;
      lightArray[2] = ar2;
      lightArray[3] = ar3;
      //--Serial.println(command + " " + ar0 +" "+ ar1);
      sendLights();
    }
    else if (command == "dumpLightArrays")
    {
      String letter = Serial.readStringUntil('\n');
      // Prints current LED array as a macro for copy-paste
      // Serial.print("dump current light array: \n#define ");
      Serial.print("#define ");
      Serial.print(letter);
      Serial.print(" 0x");
      Serial.print(lightArray[0], HEX);
      Serial.print(", 0x");
      Serial.print(lightArray[1], HEX);
      Serial.print(", 0x");
      Serial.print(lightArray[2], HEX);
      Serial.print(", 0x");
      Serial.println(lightArray[3], HEX);
      // Serial.print("");
    }
    else if (command == "showLightArrays")
    {
      directLightArray(letterV);
    }
    else if (command == "daVinci")
    {
      davinci();
    }
    else if (command == "logic")
    {
      logicPro();
    }
    else if (command == "everlast")
    {
      everlastEngineering();
    }
  }
}

// ANIMATION FRAMES

// Laser animation frames
const AnimationFrame laserFrames[] = {
    FRAME(30, laser1),
    FRAME(30, laser2),
    FRAME(30, laser3),
    FRAME(30, laser4),
    FRAME(30, laser5),
    FRAME(30, laser6),
    FRAME(10, laserhit1),
    FRAME(10, laserhit2),
    FRAME(10, laser6),
    FRAME(10, laserhit1),
    FRAME(10, laserhit2),
    FRAME(10, laser6),
    FRAME(10, laserhit1),
    FRAME(10, laserhit2),
    FRAME(10, laser6),
    FRAME(10, laserhit1),
    FRAME(10, laserhit2),
    FRAME(10, laser6),
    FRAME(10, laserhit1),
    FRAME(10, laserhit2),
    FRAME(10, laser6),
    FRAME(10, laserhit1),
    FRAME(10, laserhit2),
    FRAME(10, laser6),
    FRAME(10, laserhit1),
    FRAME(10, laserhit2),
    FRAME(10, laser6),
    FRAME(10, laserhit1),
    FRAME(10, laserhit2),
    FRAME(10, laser6),
    FRAME(10, laserhit1),
    FRAME(10, laserhit2),
    FRAME(10, laser6),
    FRAME(10, laserhit1),
    FRAME(10, laserhit2),
    FRAME(10, laser6),
    FRAME(30, laser5),
    FRAME(30, laser4),
    FRAME(30, laser3),
    FRAME(30, laser2),
    FRAME(30, laser1),
    FRAME(0, allOff),
};

const AnimationFrame laserbcFrames[] = {
    FRAME(30, laserb1),
    FRAME(30, laserb2),
    FRAME(30, laserb3),
    FRAME(30, laserb4),
    FRAME(30, laserb5),
    FRAME(30, laserc1),
    FRAME(30, laserc2),
    FRAME(30, laserc3),
    FRAME(30, laserc4),
    FRAME(30, laserc5),
    FRAME(30, laserc6),
    FRAME(30, laserc7),
    FRAME(30, laserc8),
    FRAME(30, laserc9),
    FRAME(30, laserc10),
    FRAME(30, laserc11),
    FRAME(0, allOff),
};

const AnimationFrame lasereFrames[] = {
    FRAME(30, lasere1),
    FRAME(30, lasere2),
    FRAME(30, lasere3),
    FRAME(30, lasere4),
    FRAME(30, lasere5),
    FRAME(30, lasere6),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(1, lasere7),
    FRAME(1, lasere8),
    FRAME(30, laserea1),
    FRAME(30, laserea2),
    FRAME(30, laserea3),
    FRAME(30, laserea4),
    FRAME(30, laserea5),
    FRAME(0, allOff),
};

// Engraver animation frames (forward and reverse sweeps)
const AnimationFrame engraverFrames[] = {
    FRAME(200, engraver1),
    FRAME(50, engraver2a), FRAME(50, engraver2b), FRAME(50, engraver2a), FRAME(50, engraver2b),
    FRAME(50, engraver3a), FRAME(50, engraver3b), FRAME(50, engraver3a), FRAME(50, engraver3b),
    FRAME(50, engraver4a), FRAME(50, engraver4b), FRAME(50, engraver4a), FRAME(50, engraver4b),
    FRAME(50, engraver5a), FRAME(50, engraver5b), FRAME(50, engraver5a), FRAME(50, engraver5b),
    FRAME(50, engraver6a), FRAME(50, engraver6b), FRAME(50, engraver6a), FRAME(50, engraver6b),
    FRAME(50, engraver7a), FRAME(50, engraver7b), FRAME(50, engraver7a), FRAME(50, engraver7b),
    FRAME(200, engraver8),
    FRAME(1000, allOff),
    // Reverse sweep
    FRAME(200, engraver8),
    FRAME(200, engraver7b),
    FRAME(200, engraver6b),
    FRAME(200, engraver5b),
    FRAME(50, engraver5a), FRAME(50, engraver5b), FRAME(50, engraver5a), FRAME(50, engraver5b),
    FRAME(200, engraver5b),
    FRAME(50, engraver5a), FRAME(50, engraver5b), FRAME(50, engraver5a), FRAME(50, engraver5b),
    FRAME(300, engraver5b),
    FRAME(300, engraver4b),
    FRAME(50, engraver4a), FRAME(50, engraver4b), FRAME(50, engraver4a), FRAME(50, engraver4b),
    FRAME(200, engraver4b),
    FRAME(50, engraver4a), FRAME(50, engraver4b), FRAME(50, engraver4a), FRAME(50, engraver4b),
    FRAME(300, engraver3b),
    FRAME(200, engraver2b),
    FRAME(200, engraver1),
    FRAME(0, allOff), // allLights(0)
};

// K40 Laser animation frames
const AnimationFrame k40LaserFrames[] = {
    FRAME(1, allOff),
    FRAME(50, k1),
    FRAME(50, k2),
    FRAME(50, k3),
    FRAME(50, k4),
    FRAME(1000, k5),
    FRAME(50, k6),
    FRAME(50, k7),
    FRAME(50, k8),
    FRAME(50, k9),
    FRAME(400, allOff),
    FRAME(50, foura),
    FRAME(50, fourb),
    FRAME(50, fourc),
    FRAME(50, fourd),
    FRAME(50, foure),
    FRAME(1000, fourf),
    FRAME(50, fourg),
    FRAME(50, fourh),
    FRAME(50, fouri),
    FRAME(400, allOff),
    FRAME(50, rzero1),
    FRAME(50, rzero2),
    FRAME(50, rzero3),
    FRAME(50, rzero4),
    FRAME(1000, rzero5),
    FRAME(50, rzero6),
    FRAME(50, rzero7),
    FRAME(50, rzero8),
    FRAME(50, rzero9),
    FRAME(50, rzero10),
    FRAME(50, rzero11),
    FRAME(50, rzero12),
    FRAME(50, rzero13),
    FRAME(1000, allOff),
    FRAME(0, letterL),
    FRAME(700, lettera),
    FRAME(0, letters),
    FRAME(0, lettere),
    FRAME(700, letterr),
    FRAME(0, allOff),
};

const AnimationFrame crossHatchFrames[] = {
    FRAME(50, crosshatch1),
    FRAME(50, crosshatch2),
    FRAME(50, crosshatch1),
    FRAME(50, crosshatch2),
    FRAME(50, crosshatch1),
    FRAME(50, crosshatch2),
    FRAME(50, crosshatch1),
    FRAME(50, crosshatch2),
    FRAME(50, crosshatch1),
    FRAME(50, crosshatch2),
    FRAME(50, crosshatch1),
    FRAME(50, crosshatch2),
    FRAME(50, crosshatch1),
    FRAME(50, crosshatch2),
    FRAME(50, crosshatch1),
    FRAME(50, crosshatch2),
    FRAME(0, allOff),
};
const AnimationFrame patternFrames[] = {
    FRAME(100, pat0),
    FRAME(100, pat1),
    FRAME(100, pat2),
    FRAME(100, pat3),
    FRAME(100, pat4),
    FRAME(100, pat5),
    FRAME(100, pat6),
    FRAME(100, pat7),
    FRAME(100, pat8),
    FRAME(100, pat9),
    FRAME(100, pat10),
    FRAME(100, pat11),
    FRAME(100, pat12),
    FRAME(100, pat5),
    FRAME(0, allOff),
};
// DaVinci mode: animates "DaVinci Resolve" on LEDs, then pre-lights shortcut keys using runAnimation
const AnimationFrame davinciFrames[] = {
    FRAME(400, allOff),
    FRAME(0, letterD),
    FRAME(0, lettera),
    FRAME(0, letterV),
    FRAME(0, letteri),
    FRAME(0, lettern),
    FRAME(0, letterc),
    FRAME(0, letteri),
    FRAME(400, allOff),
    FRAME(0, letterR),
    FRAME(0, lettere),
    FRAME(0, letters),
    FRAME(0, lettero),
    FRAME(0, letterl),
    FRAME(0, letterv),
    FRAME(0, lettere),
    FRAME(0, allOff),
};

// Visual Code mode: animates "VSCode" on LEDs using runAnimation
const AnimationFrame visualCodeFrames[] = {
    FRAME(400, allOff),
    FRAME(0, letterV),
    FRAME(400, allOff),
    FRAME(0, letterS),
    FRAME(400, allOff),
    FRAME(0, letterC),
    FRAME(0, lettero),
    FRAME(0, letterd),
    FRAME(0, lettere),
    FRAME(0, allOff),
};
// Everlast Engineering mode: animates logo and patterns using runAnimation
const AnimationFrame everlastFrames[] = {
    FRAME(400, allOff),
    FRAME(0, letterE),
    FRAME(0, letterv),
    FRAME(0, lettere),
    FRAME(0, letterr),
    FRAME(0, letterl),
    FRAME(0, lettera),
    FRAME(0, letters),
    FRAME(0, lettert),
    FRAME(400, allOff),
    FRAME(0, letterE),
    FRAME(0, lettern),
    FRAME(0, letterg),
    FRAME(0, letteri),
    FRAME(0, lettern),
    FRAME(0, lettere),
    FRAME(0, lettere),
    FRAME(0, letterr),
    FRAME(0, letteri),
    FRAME(0, lettern),
    FRAME(0, letterg),
    FRAME(0, allOff),
};

// Generic interruptible animation runner
bool runAnimation(const AnimationFrame *frames, int frameCount)
{
  for (int i = 0; i < frameCount; ++i)
  {
    for (int j = 0; j < 4; ++j)
      lightArray[j] = frames[i].pattern[j];
    sendLights();
    int duration = frames[i].duration;
    if (duration <= 0)
      duration = 250;
    if (!delayKeyPressAware(duration))
    {
      allLights(0);
      sendLights();
      return false;
    }
  }
  return true;
}

// Animations

bool laserAnimation()
{
  return runAnimation(laserFrames, sizeof(laserFrames) / sizeof(AnimationFrame));
}

bool lasereAnimation()
{
  return runAnimation(lasereFrames, sizeof(lasereFrames) / sizeof(AnimationFrame));
}

bool engraverAnimation()
{
  return runAnimation(engraverFrames, sizeof(engraverFrames) / sizeof(AnimationFrame));
}

bool crossHatchAnimation()
{
  return runAnimation(crossHatchFrames, sizeof(crossHatchFrames) / sizeof(AnimationFrame));
}

bool patternAnimation()
{
  return runAnimation(patternFrames, sizeof(patternFrames) / sizeof(AnimationFrame));
}

void davinci()
{
  runAnimation(davinciFrames, sizeof(davinciFrames) / sizeof(AnimationFrame));
  mode = 0;
  preLight();                            // light up shortcut keys
  flipBitInLightArray(mode, true, true); // mode indicator
  sendLights();
}

void k40Laser()
{
  runAnimation(k40LaserFrames, sizeof(k40LaserFrames) / sizeof(AnimationFrame));
  mode = 1;
  preLight();
  flipBitInLightArray(mode, true, true);
  sendLights();
  armTriggerCount = 0;
}

void visualCode()
{
  runAnimation(visualCodeFrames, sizeof(visualCodeFrames) / sizeof(AnimationFrame));
  mode = 2;
  preLight();
  flipBitInLightArray(mode, true, true);
  sendLights();
}

// Logic Pro mode: animates "Logic Pro" on LEDs
void logicPro()
{
  allLights(0);
  if (!delayKeyPressAware(300))
    return;
  directLightArray(letterL);
  directLightArray(lettero);
  directLightArray(letterg);
  directLightArray(letteri);
  directLightArray(letterc);
  if (!delayKeyPressAware(300))
    return;
  allLights(0);
  if (!delayKeyPressAware(300))
    return;
  directLightArray(letterP);
  directLightArray(letterr);
  directLightArray(lettero);
  if (!delayKeyPressAware(300))
    return;
  allLights(0);
  if (!delayKeyPressAware(300))
    return;
  mode = 1;
  preLight();
  flipBitInLightArray(mode, true, true);
  sendLights();
}

// Pre-lights shortcut keys for DaVinci mode
void preLight()
{
  if (mode == 0)
  { // davinci
    for (int i = 0; i < elementCount(daVinciSet); i++)
    {
      flipBitInLightArray(daVinciSet[i]);
    }
  }
  if (mode == 1)
  { // laser
    for (int i = 0; i < elementCount(k40Set); i++)
    {
      flipBitInLightArray(k40Set[i]);
    }
  }
}

void everlastEngineering()
{
  runAnimation(everlastFrames, sizeof(everlastFrames) / sizeof(AnimationFrame));
  mode = 2;
  flipBitInLightArray(mode, true, true);
  sendLights();
  allLights(0);
}

// Sets the LED array directly to a pattern and sends to device
void directLightArray(byte a, byte b, byte c, byte d)
{
  directLightArray(a, b, c, d, 200);
}

// Overload: allows custom pause between patterns
// Returns false if interrupted, true if completed
bool directLightArray(byte a, byte b, byte c, byte d, int pause)
{
  lightArray[0] = a;
  lightArray[1] = b;
  lightArray[2] = c;
  lightArray[3] = d;
  sendLights();
  return delayKeyPressAware(pause);
}

// Sends current lightArray to LPFK via serial
void sendLights()
{
  softSerial.write(0x94); // 0x94 lights, send with 4 bytes, each bit = one light
  for (int i = 0; i < elementCount(lightArray); i++)
  {
    softSerial.write((byte)reverseBits(lightArray[i])); // due to little endian, the bytes are reversed
  }
  int result = readAndReturn(); // read the return of the sendLights to remove it from the buffer
  if (result == 0x80)
  {
    //--Serial.print("There was a problem sending the light command: ");
    //--Serial.println(lightArray[0]);
    //--Serial.println(lightArray[1]);
    //--Serial.println(lightArray[2]);
    //--Serial.println(lightArray[3]);
  }
}

// Reads a byte from LPFK serial, returns key index or status
int readAndReturn()
{
  int received = softSerial.read();
  if (received != -1 && received != 0x80 && received != 0x81)
  {
    //--Serial.print("Received Keystroke: ");    //--Serial.println(received);
  }
  return received;
}

// Utility: reverses bits in a byte (for hardware compatibility)
byte reverseBits(byte num)
{
  unsigned int NO_OF_BITS = sizeof(num) * 8;
  byte reverse_num = 0, i, temp;
  for (i = 0; i < NO_OF_BITS; i++)
  {
    temp = (num & (1 << i));
    if (temp)
    {
      reverse_num |= 1UL << (NO_OF_BITS - 1) - i;
    }
  }
  return reverse_num;
}

// Utility: sets all LEDs to value b
void allLights(byte b)
{
  fillLightsArray(b);
  sendLights();
}

// Utility: fills lightArray with value b
void fillLightsArray(byte b)
{
  for (int i = 0; i < 4; i++)
  {
    lightArray[i] = b;
  }
}

// Checks if a key is in the DaVinci shortcut set
boolean isInPreLightArray(int light)
{
  if (mode == 0)
  {
    //--Serial.print(elementCount(daVinciSet));
    for (int i = 0; i < elementCount(daVinciSet); i++)
    {
      //--Serial.print(daVinciSet[i]);
      //--Serial.print(" ");
      if (daVinciSet[i] == light)
      {
        return true;
      }
    }
    //--Serial.print("\n");
    return false;
  }
  
  else if (mode == 1)
  {
    //--Serial.print(elementCount(k40Set));
    for (int i = 0; i < elementCount(k40Set); i++)
    {
      //--Serial.print(k40Set[i]);
      //--Serial.print(" ");
      if (k40Set[i] == light)
      {
        return true;
      }
    }
    //--Serial.print("\n");
    return false;
  }
}

// Flips a key's LED on/off with animation (used for DaVinci mode)
void flipBitInLightArray(int light, int pause)
{
  boolean isInArray = isInPreLightArray(light);
  //--Serial.print(isInArray);
  flipBitInLightArray(light, true, !isInArray);
  sendLights();
  if (!delayKeyPressAware(pause))
    return;
  flipBitInLightArray(light, true, isInArray);
  sendLights();
}

// Flips a key's LED on/off (no animation)
void flipBitInLightArray(int light)
{
  flipBitInLightArray(light, false, false);
}

void turnOffBitInLightArray(int light)
{
  flipBitInLightArray(light, true, true);
  flipBitInLightArray(light, true, false);
}

// Core bit-flip logic for a single LED
void flipBitInLightArray(int light, boolean ignoreLit, boolean lit)
{
  int whichByte = floor(light / 8);
  //   //--Serial.print("whichByte: "); //--Serial.println(whichByte);
  int whichBit = light - (whichByte * 8);
  //   //--Serial.print("whichBit: ");  //--Serial.println(whichBit);
  bool isBitLit = ((lightArray[whichByte]) >> whichBit) & 0x1;
  //   //--Serial.print("isBitLit: "); //--Serial.println(isBitLit);
  if (isBitLit)
  {
    if (ignoreLit && lit)
    {
      // don't unlight
    }
    else
    {
      lightArray[whichByte] = lightArray[whichByte] - (pow(2, whichBit) - 0.5); // unlight that bit
    }
  }
  else
  {
    lightArray[whichByte] = lightArray[whichByte] + (pow(2, whichBit) + 0.5); // light that bit
  }
}

void comboScaledChar(String c)
{ 
  pressScaledKey();
  Keyboard.print(c);
  delay(keyDelay);
  releaseScaledKey();
}



// DaVinci mode: maps key indices to keyboard shortcuts
void fulfilKeyboardCommands(int key)
{
  
#ifndef PATTERN_DESIGN_MODE
 
  if (mode == 0) {
    // davinci mode
    switch (key)
    {
      /*
      Ripple Delete Start of Clip to Playhead
      ⌘ shift [
      Ripple Delete Playhead to End of Clip
      ⌘ shift ]
      */
    case 4:
      //--Serial.println("Undo");
      // ⌘ z
      Keyboard.press(KEY_LEFT_GUI);
      delay(10);
      Keyboard.print("z");
      delay(10);
      Keyboard.release(KEY_LEFT_GUI);
      delay(10);
      break;
    case 5:
      //--Serial.println("Redo");
      // ⌘ shift z
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_SHIFT);
      delay(10);
      Keyboard.print("z");
      delay(10);
      Keyboard.release(KEY_LEFT_SHIFT);
      Keyboard.release(KEY_LEFT_GUI);
      break;
    case 16:
      //--Serial.println("Play/Stop");
      // spacebar
      Keyboard.print(" ");
      break;
    case 19:
      //--Serial.println("Playhead back one clip");
      // up arrow
      Keyboard.press(KEY_UP_ARROW);
      delay(10);
      Keyboard.release(KEY_UP_ARROW);
      break;
    case 20:
      //--Serial.println("Playhead ahead one clip");
      // down arrow
      Keyboard.press(KEY_DOWN_ARROW);
      delay(10);
      Keyboard.release(KEY_DOWN_ARROW);
      break;
    case 21:
      //--Serial.println("Zoom In");
      Keyboard.press(KEY_LEFT_GUI);
      delay(10);
      Keyboard.print("+");
      delay(10);
      Keyboard.release(KEY_LEFT_GUI);
      break;
    case 22:
      //--Serial.println("Ripple Delete Start of Clip to Playhead");
      // ⌘ shift [
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_SHIFT);
      delay(10);
      Keyboard.print("[");
      delay(10);
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_SHIFT);
      delay(10);
      break;
    case 23:
      //--Serial.println("Ripple Delete Playhead to End of Clip");
      // ⌘ shift ]
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_SHIFT);
      delay(10);
      Keyboard.print("]");
      delay(10);
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_SHIFT);
      break;
    case 25:
      //--Serial.println("Playhead back one frame");
      // left arrow
      Keyboard.press(KEY_LEFT_ARROW);
      delay(10);
      Keyboard.release(KEY_LEFT_ARROW);
      break;
    case 26:
      //--Serial.println("Playhead ahead one frame");
      // right arrow
      Keyboard.press(KEY_RIGHT_ARROW);
      delay(10);
      Keyboard.release(KEY_RIGHT_ARROW);
      break;
    case 27:
      //--Serial.println("Zoom Out");
      Keyboard.press(KEY_LEFT_GUI);
      delay(10);
      Keyboard.print("-");
      delay(10);
      Keyboard.release(KEY_LEFT_GUI);
      break;
    case 28:
      //--Serial.println("Select under playhead");
      // shift v
      Keyboard.press(KEY_LEFT_SHIFT);
      delay(10);
      Keyboard.print("v");
      delay(10);
      Keyboard.release(KEY_LEFT_SHIFT);
      break;
    case 29:
      //--Serial.println("Razor - in blade mode, this will slice the clip at the mouse cursor if its over a clip");
      // ⌘ b
      Keyboard.press(KEY_LEFT_GUI);
      delay(10);
      Keyboard.print("b");
      delay(10);
      Keyboard.release(KEY_LEFT_GUI);
      break;
    case 30:
      //--Serial.println("Playhead back one sec");
      // shift + left arrow
      Keyboard.press(KEY_LEFT_SHIFT);
      delay(10);
      Keyboard.press(KEY_LEFT_ARROW);
      delay(10);
      Keyboard.release(KEY_LEFT_ARROW);
      delay(10);
      Keyboard.release(KEY_LEFT_SHIFT);
      break;
    case 31:
      //--Serial.println("Playhead ahead one sec");
      // shift + right arrow
      Keyboard.press(KEY_LEFT_SHIFT);
      delay(10);
      Keyboard.press(KEY_RIGHT_ARROW);
      delay(10);
      Keyboard.release(KEY_RIGHT_ARROW);
      delay(10);
      Keyboard.release(KEY_LEFT_SHIFT);
      break;
    }
  }
  else if (mode == 1) {
    // k40 laser mode

    if (key == 28)
    {
      // arming button
      // should be pressed three times in quick succession to arm the laser
      /*
      
      */
      isArmed = false;
      if (armTriggerCount < 2) {
        flipBitInLightArray(29+armTriggerCount, true, true);  
        armTriggerCount++;
      }
      else {
        byte oldLightArray[4];
        for (int i = 0; i < 4; i++) oldLightArray[i] = lightArray[i];
        laserAnimation();
        for (int i = 0; i < 4; i++) lightArray[i] = oldLightArray[i];
        sendLights();
        isArmed = true;
      }
    }
    else if (armTriggerCount > 0)
    {
      turnOffBitInLightArray(29);
      turnOffBitInLightArray(30);
      turnOffBitInLightArray(31);
      armTriggerCount = 0;
    }

    if (key == 31) {
      if (isArmed) {
        // fire the laser
        Keyboard.press(KEY_LEFT_CTRL);
        delay(keyDelay);
        Keyboard.press(KEY_LEFT_SHIFT);
        delay(keyDelay);
        Keyboard.press(KEY_LEFT_ALT);
        delay(keyDelay);
        Keyboard.print('7');
        delay(keyDelay);
        Keyboard.release(KEY_LEFT_ALT);
        delay(keyDelay);
        Keyboard.release(KEY_LEFT_SHIFT);
        delay(keyDelay);
        Keyboard.release(KEY_LEFT_CTRL);
        isArmed = false; // disarm after firing
      }
      else {
        Keyboard.press(KEY_LEFT_CTRL);
        delay(keyDelay);
        Keyboard.press(KEY_LEFT_SHIFT);
        delay(keyDelay);
        Keyboard.press(KEY_LEFT_ALT);
        delay(keyDelay);
        Keyboard.print('0');
        delay(keyDelay);
        Keyboard.release(KEY_LEFT_ALT);
        delay(keyDelay);
        Keyboard.release(KEY_LEFT_SHIFT);
        delay(keyDelay);
        Keyboard.release(KEY_LEFT_CTRL);
      } 
    }

    if (key == 4) {
      comboScaledChar("q");
    }
    else if (key == 5) {
      comboScaledChar("w");
    }
    else if (key == 6) {
      comboScaledChar("e");
    }
    else if (key == 10) {
      comboScaledChar("a");
    }
    else if (key == 11) {
      Keyboard.press(KEY_HOME);
      delay(keyDelay);
      Keyboard.release(KEY_HOME);
    }
    else if (key == 12) {
      comboScaledChar("d");
    }
    else if (key == 16) {
      comboScaledChar("z");
    }
    else if (key == 17) {
      comboScaledChar("x");
    }
    else if (key == 18) {
      comboScaledChar("c");
    }
    else if (key == 7) {
      comboScaledChar("r");
    }
    else if (key == 8) {
      comboScaledChar("t");
    }
    else if (key == 9) {
      comboScaledChar("y");
    }
    else if (key == 13) {
      comboScaledChar("f");
    }
    else if (key == 14) {
      if (selectDeselectToggle) {
        Keyboard.print('g');
      }
      else {
        Keyboard.print('G');
      }
      selectDeselectToggle = !selectDeselectToggle;
    }
    else if (key == 15) {
      comboScaledChar("h");
    }
    else if (key == 19) {
      comboScaledChar("v");
    }
    else if (key == 20) {
      comboScaledChar("b");
    }
    else if (key == 21) {
      comboScaledChar("n");
    }


    switch (key)
    {
    // setting the scales
    case 22:
      scaleFactor = 0.1;
      // Keyboard.print("Scale Factor 0.1");
      // set scale: 0.1mm
      // set a scale factor for the laser cutter software
      // this will be used to scale the navigation movements
      break;
    case 23:
      scaleFactor = 1.0;
      // Keyboard.print("Scale Factor 1.0");
      // set scale: 1mm
      break;
    case 24:
      scaleFactor = 10.0;
      // Keyboard.print("Scale Factor 10.0");
      // set scale: 10mm
      break;
    case 25:
      scaleFactor = 20.0;
      // Keyboard.print("Scale Factor 20.0");
      // set scale: 20mm
      break;
    }
  }
#endif
}
