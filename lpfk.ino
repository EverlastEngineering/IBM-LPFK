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

// #define PATTERN_DESIGN_MODE // Comment out for normal HID operation

// Animation frame structure
struct AnimationFrame {
  byte pattern[4];
  int duration;
};

#define FRAME(pause, pat) { {pat}, pause }

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
#define lettera 0x0, 0x70, 0x92, 0xE4   // "a"
#define letterV 0x29, 0x49, 0x12, 0x45  // "V"
#define letteri 0x40, 0x0, 0x4, 0x21    // "i"
#define lettern 0x0, 0x28, 0x96, 0x94   // "n"
#define letterc 0xC0, 0x48, 0x82, 0x64  // "c"
#define letterR 0x27, 0x49, 0x8E, 0x92  // "R"
#define lettere 0xC0, 0x48, 0x9E, 0x60  // "e"
#define letters 0xC0, 0x8, 0xC, 0x64    // "s"
#define lettero 0xC0, 0x48, 0x92, 0x64  // "o"
#define letterl 0x42, 0x10, 0x4, 0x21   // "l"
#define letterv 0x0, 0x88, 0x22, 0x45   // "v"

#define crosshatch1 0xAA, 0x56, 0x6A, 0x55 // crosshatch pattern
#define crosshatch2 0x55, 0xA9, 0x95, 0xAA // crosshatch pattern

#define letterL 0x21, 0x8, 0x82, 0xF0      // "L"
#define letterg 0xC0, 0x49, 0x9C, 0x64     // "g"
#define letterP 0x27, 0x49, 0x8E, 0x10     // "P"
#define letterr 0x0, 0x38, 0x92, 0x10      // "r"

#define letterE 0x2F, 0x78, 0x82, 0xF0     // "E"
#define lettert 0x84, 0x70, 0x8, 0x42      // "t"

#define letterC 0x26, 0x9, 0x82, 0x64      // "C"
#define letterd 0x8, 0x71, 0x92, 0xE4      // "d"
#define letterS 0x26, 0x19, 0x98, 0x64     // "S"

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

#include <Keyboard.h>
#include "SoftwareSerialParity.h"

#define elementCount(x) (sizeof(x) / sizeof(x[0]))

// Custom serial class for odd parity communication with LPFK
SoftwareSerialParity softSerial(8, 9); // RX, TX

// lightArray holds the state of all 32 LEDs (4 bytes, 8 bits each)
byte lightArray[] = {0x00, 0x00, 0x00, 0x00};
boolean lightsStayOn = false; // If true, LEDs stay on after key press
int mode = 0; // Current mode: 0=DaVinci, 1=Logic Pro, 2=Visual Code, etc.

// DaVinci mode: keys to pre-light and map to shortcuts
int daVinciSet[] = {4,5,16,19,20,21,22,23,25,26,27,28,29,30,31};
// This array must match the keys mapped in fulfilKeyboardCommands()

unsigned long lastKeyTime = 0; // Tracks last key press time
bool screensaverActive = false;

// Helper: non-blocking delay that aborts on key press
void screensaverDelay(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (Serial.available() > 0 || softSerial.read() != -1) {
      screensaverActive = false;
      break;
    }
    delay(10); // Small sleep to avoid busy loop
  }
}

// Helper: checks if a keypress is available on softSerial only (does not consume buffer)
bool isKeyPressed() {
  int key = readAndReturn();
  if (key != -1 && key != 0x80 && key != 0x81) return true;
  return false;
}

// Keypress-aware delay: returns true if completed, false if aborted
bool delayKeyPressAware(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (isKeyPressed()) {
      allLights(0); // Clear LED screen
      return false; // Aborted
    }
    delay(10); // More responsive increments
  }
  return true; // Completed
}

// Helper: play a random animation
void playRandomAnimation() {
  laserAnimation();
  // int pick = random(0, 4); // Add more if you want
  // switch (pick) {
  //   case 0:
  //     demoLoop(false, 30);
  //     break;
  //   case 1:
  //     laserAnimation();
  //     break;
  //   case 2:
  //     k40Laser();
  //     break;
  //   case 3:
  //     everlastEngineering();
  //     break;
  // }
}

void setup() {
   pinMode(LED_BUILTIN, OUTPUT); // onboard LED for status/debug
   softSerial.begin(9600,ODD);  // start serial with odd parity for LPFK
   delay(1000); 
   softSerial.write(0x08); // 0x08: enable keyboard
  //  k40Laser();
  //  laserAnimation();       // run Pac-Man animation at startup
  mode = 1;
   flipBitInLightArray(mode,true,true); // light up mode indicator LED
#ifdef PATTERN_DESIGN_MODE
   Serial.begin(9600);     // USB Serial for pattern design/debugging
#else
   Keyboard.begin();       // USB HID keyboard emulation
#endif
}

void demoLoop(boolean keep) {
  demoLoop(keep, 1); // default delay = 1ms
}

// Animates LEDs in a sweep pattern (left-to-right, then right-to-left)
// If keep==true, lights stay on; else, LEDs are cleared between steps
void demoLoop(boolean keep, int _delay) {
  allLights(0x00); // turn off all LEDs
  int stepDelay = max(_delay, 100); // Ensure minimum visible delay
  // Sweep from left to right
  for (int i = 0; i < 32; i++) {
    int byteLoop = floor(i/8); // Determine which byte in lightArray this LED is in
    if (!keep) fillLightsArray(0); // If not keeping lights, clear all before lighting next
    // Light up the current LED by setting its bit in the correct byte
    lightArray[byteLoop] = lightArray[byteLoop] + pow(2,i-(byteLoop*8))  + 0.5;
    sendLights(); // Send updated LED state to LPFK
    if (!delayKeyPressAware(stepDelay)) return;
  }
  // Sweep from right to left
  for (int i = 31; i > -1; i--) {
    int byteLoop = floor(i/8); // Determine which byte in lightArray this LED is in
    int factor = 1;
    if (!keep) {
      fillLightsArray(0); // If not keeping lights, clear all before lighting next
    }
    else {
      factor = -1; // If keeping lights, subtract to turn off LEDs one by one
    }
    // Turn off (or on) the current LED by adjusting its bit in the correct byte
    lightArray[byteLoop] = lightArray[byteLoop] + (factor * pow(2,i-(byteLoop*8)))  + 0.5;
    sendLights(); // Send updated LED state to LPFK
    if (!delayKeyPressAware(stepDelay)) return;
  }
  allLights(0x00); // turn off all LEDs at the end
}

// Main loop: handles key events and serial commands
void loop() {
#ifndef PATTERN_DESIGN_MODE
  if (!screensaverActive && millis() - lastKeyTime > 6000) {
    screensaverActive = true;
  }
  if (screensaverActive) {
    playRandomAnimation();
    screensaverDelay(10000); // Wait 10s, abort if key pressed
    // If still active, play another
    if (screensaverActive) {
      playRandomAnimation();
      screensaverDelay(10000);
    }
    screensaverActive = false;
    lastKeyTime = millis(); // Reset timer after screensaver
    return;
  }
#endif
  boolean keepReading = true;
  while (keepReading) {
    int received = readAndReturn(); // read key event from LPFK
    if (received != -1 && received != 129) {
      lastKeyTime = millis(); // Update last key time
      screensaverActive = false; // Abort screensaver
      // If lightsStayOn is true, keep LEDs on after key press
      if (lightsStayOn && received < 32) {
        // If key 9 is pressed and all LEDs are off, run demo
        if (received == 9 && lightArray[0] == 0 && lightArray[2] == 0 && lightArray[2] == 0 && lightArray[3] == 0 ) {
          demoLoop(false,1);
          lightsStayOn = !lightsStayOn;
        }
        else {
          flipBitInLightArray(received);
          sendLights();
        }
      }
      // Mode switching and special patterns
      else if (received == 3) {
        everlastEngineering();
      }
      else if (received == 2) {
        visualCode();
      }
      else if (received == 9) {
        demoLoop(false,1);
        lightsStayOn = !lightsStayOn;
      }
      else if (received == 1) {
        k40Laser(); // Laser mode on button 1
        mode = 1;
        preLight();
        flipBitInLightArray(mode,true,true);
        sendLights();
      }
      else if (received == 0) {
        davinci();
      }
      // Normal key press: light LED and send keyboard shortcut (if in DaVinci mode)
      else if (received < 32) {
        flipBitInLightArray(received,50);
//        preLight();
        sendLights();
        fulfilKeyboardCommands(received);
      }
      // Special demo pattern
      else if (received == 28) {
        demoLoop(true,1);
      }
      // If received is out of range, turn all LEDs on
      else {
         for (int i = 0; i < 4; i++) {
          lightArray[i] = ((byte) 0xFF);
         }
      }
    }
    keepReading = false;
  }

  // Serial command interface for pattern creation and testing
  while (Serial.available() > 0) {
    String command = Serial.readStringUntil(' '); // reads command until space

    if (command == "demoMode") {
      String fillStr = Serial.readStringUntil(' ');
      String pauseStr = Serial.readStringUntil('\n');
      boolean fill = fillStr.toInt();
      long pause = pauseStr.toInt();
      if (pause != 0) {
        demoLoop(fill,pause);  
      }
      else {
        //--Serial.println("incorrect parameters: demoMode 1 100 = demoMode 0, 100 ms delay per light");
      }
    }
    else if (command == "singleLight") {
      String lightStr = Serial.readStringUntil('\n');
      int light = -1;
      light = lightStr.toInt();
      if (light < 0 || light > 31) {
        //--Serial.println("incorrect parameters: singleLight 0-31");
      }
      else {
        flipBitInLightArray(light);
        sendLights();
      }
    }
    else if (command == "lightArray") {
      String ar0str = Serial.readStringUntil(' ');
      String ar1str = Serial.readStringUntil(' ');
      String ar2str = Serial.readStringUntil(' ');
      String ar3str = Serial.readStringUntil('\n');
      byte ar0 = (byte) ar0str.toInt();
      byte ar1 = (byte) ar1str.toInt();
      byte ar2 = (byte) ar2str.toInt();
      byte ar3 = (byte) ar3str.toInt();
      lightArray[0] = ar0;
      lightArray[1] = ar1;
      lightArray[2] = ar2;
      lightArray[3] = ar3;
      //--Serial.println(command + " " + ar0 +" "+ ar1);
      sendLights();
    }
    else if (command == "dumpLightArrays") {
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
    else if (command == "showLightArrays") {
      directLightArray(letterV);
    }
    else if (command == "daVinci") {
      davinci();
    }
    else if (command == "logic") {
      logicPro();
    }
    else if (command == "everlast") {
      everlastEngineering();
    }
  }
}

// Laser animation frames
const AnimationFrame laserFrames[] = {
  FRAME(30, laser1),
  FRAME(30, laser2),
  FRAME(30, laser3),
  FRAME(30, laser4),
  FRAME(30, laser5),
  FRAME(30, laser6),
  FRAME(120, laserhit1),
  FRAME(120, laserhit2),
  FRAME(120, laser6),
  FRAME(120, laserhit1),
  FRAME(120, laserhit2),
  FRAME(120, laser6),
  FRAME(120, laserhit1),
  FRAME(120, laserhit2),
  FRAME(120, laser6),
  FRAME(120, laserhit1),
  FRAME(120, laserhit2),
  FRAME(120, laser6),
  FRAME(30, laser5),
  FRAME(30, laser4),
  FRAME(30, laser3),
  FRAME(30, laser2),
  FRAME(30, laser1),
  FRAME(2500, allOff), // allLights(0)
};

// Engraver animation frames (forward and reverse sweeps)
const AnimationFrame engraverFrames[] = {
  FRAME(500, engraver1),
  FRAME(50, engraver2a), FRAME(50, engraver2b), FRAME(50, engraver2a), FRAME(50, engraver2b),
  FRAME(50, engraver3a), FRAME(50, engraver3b), FRAME(50, engraver3a), FRAME(50, engraver3b),
  FRAME(50, engraver4a), FRAME(50, engraver4b), FRAME(50, engraver4a), FRAME(50, engraver4b),
  FRAME(50, engraver5a), FRAME(50, engraver5b), FRAME(50, engraver5a), FRAME(50, engraver5b),
  FRAME(50, engraver6a), FRAME(50, engraver6b), FRAME(50, engraver6a), FRAME(50, engraver6b),
  FRAME(50, engraver7a), FRAME(50, engraver7b), FRAME(50, engraver7a), FRAME(50, engraver7b),
  FRAME(500, engraver8),
  // Reverse sweep
  FRAME(500, engraver8),
  FRAME(50, engraver7a), FRAME(50, engraver7b), FRAME(50, engraver7a), FRAME(50, engraver7b),
  FRAME(50, engraver6a), FRAME(50, engraver6b), FRAME(50, engraver6a), FRAME(50, engraver6b),
  FRAME(50, engraver5a), FRAME(50, engraver5b), FRAME(50, engraver5a), FRAME(50, engraver5b),
  FRAME(50, engraver4a), FRAME(50, engraver4b), FRAME(50, engraver4a), FRAME(50, engraver4b),
  FRAME(50, engraver3a), FRAME(50, engraver3b), FRAME(50, engraver3a), FRAME(50, engraver3b),
  FRAME(50, engraver2a), FRAME(50, engraver2b), FRAME(50, engraver2a), FRAME(50, engraver2b),
  FRAME(500, engraver1),
  FRAME(0, allOff), // allLights(0)
};

// K40 Laser animation frames
const AnimationFrame k40LaserFrames[] = {
  FRAME(300, allOff),
  FRAME(300, letterK),
  FRAME(300, number4),
  FRAME(300, number0),
  FRAME(500, allOff),
  FRAME(300, letterL),
  FRAME(300, lettera),
  FRAME(300, letters),
  FRAME(300, lettere),
  FRAME(300, letterr),
  FRAME(300, allOff),
};

// Generic interruptible animation runner
bool runAnimation(const AnimationFrame* frames, int frameCount) {
  for (int i = 0; i < frameCount; ++i) {
    for (int j = 0; j < 4; ++j) lightArray[j] = frames[i].pattern[j];
    sendLights();
    if (!delayKeyPressAware(frames[i].duration)) {
      allLights(0);
      sendLights();
      return false;
    }
  }
  return true;
}

void laserAnimation() {
  runAnimation(laserFrames, sizeof(laserFrames)/sizeof(AnimationFrame));
}

void engraverAnimation() {
  runAnimation(engraverFrames, sizeof(engraverFrames)/sizeof(AnimationFrame));
}

void k40Laser() {
  runAnimation(k40LaserFrames, sizeof(k40LaserFrames)/sizeof(AnimationFrame));
}

// DaVinci mode: animates "DaVinci Resolve" on LEDs, then pre-lights shortcut keys
void davinci() {
  allLights(0);
  if (!delayKeyPressAware(300)) return;
  directLightArray(letterD);
  directLightArray(lettera);
  directLightArray(letterV);
  directLightArray(letteri);
  directLightArray(lettern);
  directLightArray(letterc);
  directLightArray(letteri);
  if (!delayKeyPressAware(300)) return;
  allLights(0);
  if (!delayKeyPressAware(300)) return;
  directLightArray(letterR);
  directLightArray(lettere);
  directLightArray(letters);
  directLightArray(lettero);
  directLightArray(letterl);
  directLightArray(letterv);
  directLightArray(lettere);
  if (!delayKeyPressAware(300)) return;
  allLights(0);
  if (!delayKeyPressAware(300)) return;
  mode = 0;
  preLight(); // light up shortcut keys
  flipBitInLightArray(mode,true,true); // mode indicator
  sendLights();
}

// Visual Code mode: animates "VSCode" on LEDs
void visualCode() {
  allLights(0);
  if (!delayKeyPressAware(300)) return;
  directLightArray(letterV);
  if (!delayKeyPressAware(300)) return;
  allLights(0);
  if (!delayKeyPressAware(300)) return;
  directLightArray(letterS);
  if (!delayKeyPressAware(300)) return;
  allLights(0);
  if (!delayKeyPressAware(300)) return;
  directLightArray(letterC);
  directLightArray(lettero);
  directLightArray(letterd);
  directLightArray(lettere);
  if (!delayKeyPressAware(300)) return;
  mode = 2;
  preLight();
  flipBitInLightArray(mode,true,true);
  sendLights();
}

// Logic Pro mode: animates "Logic Pro" on LEDs
void logicPro() {
  allLights(0);
  if (!delayKeyPressAware(300)) return;
  directLightArray(letterL);
  directLightArray(lettero);
  directLightArray(letterg);
  directLightArray(letteri);
  directLightArray(letterc);
  if (!delayKeyPressAware(300)) return;
  allLights(0);
  if (!delayKeyPressAware(300)) return;
  directLightArray(letterP);
  directLightArray(letterr);
  directLightArray(lettero);
  if (!delayKeyPressAware(300)) return;
  allLights(0);
  if (!delayKeyPressAware(300)) return;
  mode = 1;
  preLight();
  flipBitInLightArray(mode,true,true);
  sendLights();
}

// Pre-lights shortcut keys for DaVinci mode
void preLight() {
  if (mode == 0) { // davinci
    for (int i = 0; i<elementCount(daVinciSet); i++)
    {
        flipBitInLightArray(daVinciSet[i]);
    }
  }
}

// Everlast Engineering mode: animates logo and patterns
void everlastEngineering() {
  int pause = 250;
  directLightArray(crosshatch1);
  directLightArray(crosshatch2);
  directLightArray(crosshatch1);
  directLightArray(crosshatch2);
  allLights(0);
  if (!delayKeyPressAware(300)) return;
  directLightArray(letterE, pause);
  directLightArray(letterv, pause);
  directLightArray(lettere, pause);
  directLightArray(letterr, pause);
  directLightArray(letterl, pause);
  directLightArray(lettera, pause);
  directLightArray(letters, pause);
  directLightArray(lettert, pause);
  if (!delayKeyPressAware(200)) return;
  allLights(0);
  if (!delayKeyPressAware(200)) return;
  directLightArray(letterE, pause);
  directLightArray(lettern, pause);
  directLightArray(letterg, pause);
  directLightArray(letteri, pause);
  directLightArray(lettern, pause);
  directLightArray(lettere, pause);
  directLightArray(lettere, pause);
  directLightArray(letterr, pause);
  directLightArray(letteri, pause);
  directLightArray(lettern, pause);
  directLightArray(letterg, pause);
  allLights(0);
  if (!delayKeyPressAware(1000)) return;
  directLightArray(crosshatch1);
  directLightArray(crosshatch2);
  directLightArray(crosshatch1);
  directLightArray(crosshatch2);
  if (!delayKeyPressAware(300)) return;
  pause = 100;
  directLightArray(pat0, pause);
  directLightArray(pat1, pause);
  directLightArray(pat2, pause);
  directLightArray(pat3, pause);
  directLightArray(pat4, pause);
  directLightArray(pat5, pause);
  directLightArray(pat6, pause);
  directLightArray(pat7, pause);
  directLightArray(pat8, pause);
  directLightArray(pat9, pause);
  directLightArray(pat10, pause);
  directLightArray(pat11, pause);
  directLightArray(pat12, pause);
  directLightArray(pat5, pause);
  demoLoop(1,1);
  demoLoop(0,1);
  mode = 2;
  flipBitInLightArray(mode,true,true);
  sendLights();
  allLights(0);
}

// Sets the LED array directly to a pattern and sends to device
void directLightArray(byte a, byte b, byte c, byte d) {
   directLightArray( a,  b,  c,  d, 200);
}

// Overload: allows custom pause between patterns
// Returns false if interrupted, true if completed
bool directLightArray(byte a, byte b, byte c, byte d, int pause) {
  lightArray[0] = a;
  lightArray[1] = b;
  lightArray[2] = c;
  lightArray[3] = d;
  sendLights();
  return delayKeyPressAware(pause);
}

// Sends current lightArray to LPFK via serial
void sendLights() {
  softSerial.write(0x94);  // 0x94 lights, send with 4 bytes, each bit = one light
  for (int i = 0; i< elementCount(lightArray);i++) 
  {
    softSerial.write((byte) reverseBits(lightArray[i])); //due to little endian, the bytes are reversed 
  }
  int result = readAndReturn(); // read the return of the sendLights to remove it from the buffer
  if (result == 0x80) {
    //--Serial.print("There was a problem sending the light command: ");
    //--Serial.println(lightArray[0]);
    //--Serial.println(lightArray[1]);
    //--Serial.println(lightArray[2]);
    //--Serial.println(lightArray[3]);
  }
}

// Reads a byte from LPFK serial, returns key index or status
int readAndReturn() {
  int received = softSerial.read();
  if (received != -1 && received != 0x80 && received != 0x81) {
    //--Serial.print("Received Keystroke: ");    //--Serial.println(received);
  }
  return received;
}

// Utility: reverses bits in a byte (for hardware compatibility)
byte reverseBits(byte num)
{
    unsigned int  NO_OF_BITS = sizeof(num) * 8;
    byte reverse_num = 0, i, temp;
    for (i = 0; i < NO_OF_BITS; i++)
    {
        temp = (num & (1 << i));
        if(temp) {
          reverse_num |= 1UL << (NO_OF_BITS - 1) - i;
        }
    }
    return reverse_num;
}

// Utility: sets all LEDs to value b
void allLights(byte b) {
  fillLightsArray(b);
  sendLights();
}

// Utility: fills lightArray with value b
void fillLightsArray(byte b) {
  for (int i = 0; i < 4; i++) {
    lightArray[i] = b;
  }
}

// Checks if a key is in the DaVinci shortcut set
boolean isInPreLightArray(int light) {
  if (mode == 0) {
    //--Serial.print(elementCount(daVinciSet));
    for (int i = 0; i < elementCount(daVinciSet); i++) {
      //--Serial.print(daVinciSet[i]);
      //--Serial.print(" ");
      if (daVinciSet[i] == light) {
        return true;
      }
    }
    //--Serial.print("\n");
    return false;
  }
}

// Flips a key's LED on/off with animation (used for DaVinci mode)
void flipBitInLightArray(int light, int pause) {
  boolean isInArray = isInPreLightArray(light);
  //--Serial.print(isInArray);
  flipBitInLightArray(light, true, !isInArray);
  sendLights();
  if (!delayKeyPressAware(pause)) return;
  flipBitInLightArray(light, true, isInArray);
  sendLights();
}

// Flips a key's LED on/off (no animation)
void flipBitInLightArray(int light) {
  flipBitInLightArray(light, false, false);
}

// Core bit-flip logic for a single LED
void flipBitInLightArray(int light, boolean ignoreLit, boolean lit) {
  int whichByte = floor(light/8);
//   //--Serial.print("whichByte: "); //--Serial.println(whichByte);
  int whichBit = light - (whichByte * 8);
//   //--Serial.print("whichBit: ");  //--Serial.println(whichBit);
  bool isBitLit = ((lightArray[whichByte]) >> whichBit) & 0x1;
//   //--Serial.print("isBitLit: "); //--Serial.println(isBitLit);
  if (isBitLit) {
    if (ignoreLit && lit) {
      //don't unlight
    }
    else {
      lightArray[whichByte] = lightArray[whichByte] - (pow(2,whichBit) - 0.5); // unlight that bit
    }
  }
  else {
    lightArray[whichByte] = lightArray[whichByte] + (pow(2,whichBit) + 0.5); // light that bit
  }
}

// Utility: sets LEDs to random values
void randomLights(){
  for (int i = 0; i<4; i++) {
    lightArray[i] = random(0, 255);
  }
  sendLights();
}

// DaVinci mode: maps key indices to keyboard shortcuts
void fulfilKeyboardCommands(int key) {
#ifndef PATTERN_DESIGN_MODE
  if (mode == 0) {
    //davinci mode
    switch (key) {
          /*
    Ripple Delete Start of Clip to Playhead
     ⌘ shift [
    Ripple Delete Playhead to End of Clip
     ⌘ shift ]
    */
      case 4:
        //--Serial.println("Undo");
        //⌘ z
        Keyboard.press(KEY_LEFT_GUI);
        delay(10);
        Keyboard.print("z");
        delay(10);
        Keyboard.release(KEY_LEFT_GUI);
        delay(10);
        break;
      case 5:
        //--Serial.println("Redo");
        //⌘ shift z
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
        //spacebar
        Keyboard.print(" ");
        break;
      case 19:
        //--Serial.println("Playhead back one clip");
        //up arrow
        Keyboard.press(KEY_UP_ARROW);
        delay(10);   
        Keyboard.release(KEY_UP_ARROW);
        break;
      case 20:
        //--Serial.println("Playhead ahead one clip");
        //down arrow
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
        //⌘ shift [
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
        //⌘ shift ]
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
        //left arrow
        Keyboard.press(KEY_LEFT_ARROW);
        delay(10);
        Keyboard.release(KEY_LEFT_ARROW);
        break;
      case 26:
        //--Serial.println("Playhead ahead one frame");
        //right arrow
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
        //shift v 
        Keyboard.press(KEY_LEFT_SHIFT);
        delay(10);
        Keyboard.print("v");
        delay(10);
        Keyboard.release(KEY_LEFT_SHIFT);
        break;
      case 29:
        //--Serial.println("Razor - in blade mode, this will slice the clip at the mouse cursor if its over a clip");
        //⌘ b
        Keyboard.press(KEY_LEFT_GUI);
        delay(10);
        Keyboard.print("b");
        delay(10);
        Keyboard.release(KEY_LEFT_GUI);
        break;
      case 30:
        //--Serial.println("Playhead back one sec");
        //shift + left arrow
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
        //shift + right arrow
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
#endif
}


