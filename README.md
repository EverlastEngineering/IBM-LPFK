# IBM LPFK Arduino Controller

## Overview
This project controls an IBM LPFK (Lighted Programmable Function Keyboard) using an Arduino. The LPFK is a 32-key keypad, each with an individual LED. The code enables USB HID keyboard emulation and per-key LED control, supporting custom patterns and modes for different workflows (e.g., DaVinci Resolve, Logic Pro, Visual Studio Code).

## Device Reference
- **Keys/LEDs:** 32, arranged in a 4x8 grid.
- **LED Indexing:** LED 0 is top left, LED 31 is bottom right. Indices increment left-to-right, then top-to-bottom.

![IBM LPFK](lpfk_device.jpg)

## Approach
- **LED Control:**
  - The LED states are stored in `lightArray[4]` (each byte = 8 LEDs).
  - Patterns (letters, shapes) are defined as macros (e.g., `#define letterD ...`).
  - Patterns are created by manually lighting keys, then dumping the array via serial command.
- **Keyboard Emulation:**
  - Uses Arduino `Keyboard` library to send shortcuts to the host computer.
  - Only DaVinci mode has custom key mappings; other modes are for visual effects.
- **Serial Commands:**
  - Used for testing, creating patterns, and debugging.
  - Examples: `singleLight`, `lightArray`, `dumpLightArrays`, `demoMode`, etc.

## Workflow for Creating New Patterns
1. Use serial commands to light up desired LEDs on the device.
2. Call `dumpLightArrays [name]` to print the current LED array.
3. Copy the output and add it as a new macro (e.g., `#define letterX ...`) in your code.
4. Use the macro in functions or serial commands to display the pattern.

## Modes
- **DaVinci (mode 0):**
  - Custom key mappings for video editing shortcuts.
  - Pre-lights specific keys defined in `daVinciSet`.
- **Logic Pro, Visual Code, Everlast Engineering:**
  - Only display LED patterns; no custom key mappings.

## Serial Commands Reference
- `demoMode [fill] [pause]` — Runs demo loop with specified fill and delay.
- `singleLight [n]` — Lights up a single LED (0–31).
- `lightArray [a0] [a1] [a2] [a3]` — Directly sets the LED array.
- `dumpLightArrays [name]` — Prints the current LED array as a macro definition.
- `showLightArrays` — Displays the pattern for `letterV`.
- `daVinci` / `logic` / `everlast` — Switches to the corresponding mode/pattern.

## Unknowns / To-Do
- Only DaVinci mode has custom key mappings. If you want mappings for other modes, you'll need to define them.
- The LED patterns are created manually; if you want to automate or visualize this, consider writing a helper script or tool.
- The code assumes the LPFK is wired and working with the specified serial protocol and pinout.

## File Structure
- `lpfk/lpfk.ino` — Main Arduino sketch with all logic and macros.
- `lpfk/SoftwareSerialParity.*` — Custom serial library for odd parity communication.
- `main/main.ino` — (Not documented here; likely for other experiments.)

## Credits
Original code and macros by @jasoncopp. Documentation and comments by GitHub Copilot.
