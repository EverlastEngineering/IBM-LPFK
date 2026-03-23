/**
 * IBM LPFK Letter Pattern Generator
 * 
 * Generates 4-byte LED arrays for alphabet letters based on a 5x6 grid (32 LEDs)
 * 
 * LED Layout:
 *   Row 0: cols 1-4 (LEDs 0-3) - only 4 columns
 *   Row 1: cols 0-5 (LEDs 4-9) - full 6 columns  
 *   Row 2: cols 0-5 (LEDs 10-15)
 *   Row 3: cols 0-5 (LEDs 16-21)
 *   Row 4: cols 0-5 (LEDs 22-27)
 *   Row 5: cols 1-4 (LEDs 28-31) - only 4 columns
 * 
 * Each letter is 3x5 pixels in a 5x6 grid (leaving room for spacing)
 * 
 * lightArray[4] = {byte0, byte1, byte2, byte3}
 * - byte0: LEDs 0-7 (rows 0-1)
 * - byte1: LEDs 8-15 (rows 1-2)
 * - byte2: LEDs 16-23 (rows 2-3)
 * - byte3: LEDs 24-31 (rows 3-4)
 * 
 * Note: When sent to hardware, bits are reversed (reverseBits function)
 */

// 5x6 letter patterns (width 3-4 chars, height 5 chars)
// Using a 5x3 base grid for letters within the 5x6 LED space
// Characters: 3 pixels wide, 5 pixels tall

const letterPatterns = {
  // Capital letters (3x5 pixel patterns)
  'A': [
    [0,1,0],
    [1,0,1],
    [1,1,1],
    [1,0,1],
    [1,0,1]
  ],
  'B': [
    [1,1,0],
    [1,0,1],
    [1,1,0],
    [1,0,1],
    [1,1,0]
  ],
  'C': [
    [0,1,1],
    [1,0,0],
    [1,0,0],
    [1,0,0],
    [0,1,1]
  ],
  'D': [
    [1,1,0],
    [1,0,1],
    [1,0,1],
    [1,0,1],
    [1,1,0]
  ],
  'E': [
    [1,1,1],
    [1,0,0],
    [1,1,0],
    [1,0,0],
    [1,1,1]
  ],
  'F': [
    [1,1,1],
    [1,0,0],
    [1,1,0],
    [1,0,0],
    [1,0,0]
  ],
  'G': [
    [0,1,1],
    [1,0,0],
    [1,0,1],
    [1,0,1],
    [0,1,1]
  ],
  'H': [
    [1,0,1],
    [1,0,1],
    [1,1,1],
    [1,0,1],
    [1,0,1]
  ],
  'I': [
    [1,1,1],
    [0,1,0],
    [0,1,0],
    [0,1,0],
    [1,1,1]
  ],
  'J': [
    [1,1,1],
    [0,0,1],
    [0,0,1],
    [1,0,1],
    [0,1,0]
  ],
  'K': [
    [1,0,1],
    [1,0,1],
    [1,1,0],
    [1,0,1],
    [1,0,1]
  ],
  'L': [
    [1,0,0],
    [1,0,0],
    [1,0,0],
    [1,0,0],
    [1,1,1]
  ],
  'M': [
    [1,0,1],
    [1,1,1],
    [1,0,1],
    [1,0,1],
    [1,0,1]
  ],
  'N': [
    [1,0,1],
    [1,1,1],
    [1,1,1],
    [1,0,1],
    [1,0,1]
  ],
  'O': [
    [0,1,0],
    [1,0,1],
    [1,0,1],
    [1,0,1],
    [0,1,0]
  ],
  'P': [
    [1,1,0],
    [1,0,1],
    [1,1,0],
    [1,0,0],
    [1,0,0]
  ],
  'Q': [
    [0,1,0],
    [1,0,1],
    [1,0,1],
    [1,1,1],
    [0,1,1]
  ],
  'R': [
    [1,1,0],
    [1,0,1],
    [1,1,0],
    [1,0,1],
    [1,0,1]
  ],
  'S': [
    [0,1,1],
    [1,0,0],
    [0,1,0],
    [0,0,1],
    [1,1,0]
  ],
  'T': [
    [1,1,1],
    [0,1,0],
    [0,1,0],
    [0,1,0],
    [0,1,0]
  ],
  'U': [
    [1,0,1],
    [1,0,1],
    [1,0,1],
    [1,0,1],
    [0,1,0]
  ],
  'V': [
    [1,0,1],
    [1,0,1],
    [1,0,1],
    [1,0,1],
    [0,1,0]
  ],
  'W': [
    [1,0,1],
    [1,0,1],
    [1,0,1],
    [1,1,1],
    [1,0,1]
  ],
  'X': [
    [1,0,1],
    [1,0,1],
    [0,1,0],
    [1,0,1],
    [1,0,1]
  ],
  'Y': [
    [1,0,1],
    [1,0,1],
    [0,1,0],
    [0,1,0],
    [0,1,0]
  ],
  'Z': [
    [1,1,1],
    [0,0,1],
    [0,1,0],
    [1,0,0],
    [1,1,1]
  ],
  
  // Lowercase letters (using 3x5 but starting lower in the grid)
  'a': [
    [0,0,0],
    [1,0,1],
    [1,0,1],
    [1,1,1],
    [1,0,1]
  ],
  'b': [
    [1,0,0],
    [1,0,0],
    [1,1,0],
    [1,0,1],
    [1,1,0]
  ],
  'c': [
    [0,0,0],
    [0,1,1],
    [1,0,0],
    [1,0,0],
    [0,1,1]
  ],
  'd': [
    [0,0,1],
    [0,0,1],
    [0,1,1],
    [1,0,1],
    [0,1,1]
  ],
  'e': [
    [0,0,0],
    [0,1,0],
    [1,1,1],
    [1,0,0],
    [0,1,1]
  ],
  'f': [
    [0,1,0],
    [1,0,0],
    [1,1,0],
    [1,0,0],
    [1,0,0]
  ],
  'g': [
    [0,0,0],
    [1,0,1],
    [1,0,1],
    [0,1,1],
    [1,0,1]
  ],
  'h': [
    [1,0,0],
    [1,0,0],
    [1,1,0],
    [1,0,1],
    [1,0,1]
  ],
  'i': [
    [0,1,0],
    [0,0,1],
    [0,1,0],
    [0,1,0],
    [0,1,0]
  ],
  'j': [
    [0,0,1],
    [0,0,1],
    [0,0,1],
    [1,0,1],
    [0,1,0]
  ],
  'k': [
    [1,0,0],
    [1,0,1],
    [1,1,0],
    [1,0,1],
    [1,0,1]
  ],
  'l': [
    [0,1,0],
    [0,1,0],
    [0,1,0],
    [0,1,0],
    [0,1,1]
  ],
  'm': [
    [0,0,0],
    [1,0,1],
    [1,1,1],
    [1,0,1],
    [1,0,1]
  ],
  'n': [
    [0,0,0],
    [1,1,0],
    [1,0,1],
    [1,0,1],
    [1,0,1]
  ],
  'o': [
    [0,0,0],
    [0,1,0],
    [1,0,1],
    [1,0,1],
    [0,1,0]
  ],
  'p': [
    [0,0,0],
    [1,1,0],
    [1,0,1],
    [1,1,0],
    [1,0,0]
  ],
  'q': [
    [0,0,0],
    [0,1,1],
    [1,0,1],
    [0,1,1],
    [0,0,1]
  ],
  'r': [
    [0,0,0],
    [1,0,1],
    [1,1,0],
    [1,0,0],
    [1,0,0]
  ],
  's': [
    [0,0,0],
    [0,1,1],
    [1,0,0],
    [0,0,1],
    [1,1,0]
  ],
  't': [
    [0,1,0],
    [1,1,1],
    [0,1,0],
    [0,1,0],
    [0,0,1]
  ],
  'u': [
    [0,0,0],
    [1,0,1],
    [1,0,1],
    [1,0,1],
    [0,1,1]
  ],
  'v': [
    [0,0,0],
    [1,0,1],
    [1,0,1],
    [1,0,1],
    [0,1,0]
  ],
  'w': [
    [0,0,0],
    [1,0,1],
    [1,0,1],
    [1,1,1],
    [1,0,1]
  ],
  'x': [
    [0,0,0],
    [1,0,1],
    [0,1,0],
    [1,0,1],
    [1,0,1]
  ],
  'y': [
    [0,0,0],
    [1,0,1],
    [1,0,1],
    [0,1,1],
    [0,0,1]
  ],
  'z': [
    [0,0,0],
    [1,1,1],
    [0,0,1],
    [0,1,0],
    [1,1,0]
  ]
};

// LED grid offset - letters start at row 1, column 1 to fit nicely
const ROW_OFFSET = 1;
const COL_OFFSET = 1;

// Map: 32 LEDs indexed 0-31
// Row 0: cols 1-4 (0-3)  [4 LEDs - columns 0 and 5 don't exist]
// Row 1: cols 0-5 (4-9)  [6 LEDs]
// Row 2: cols 0-5 (10-15) [6 LEDs]
// Row 3: cols 0-5 (16-21) [6 LEDs]
// Row 4: cols 0-5 (22-27) [6 LEDs]  
// Row 5: cols 1-4 (28-31) [4 LEDs - columns 0 and 5 don't exist]

// Valid LED positions in the 6-column grid
// For rows 1-4 (indices 4-27), full 6 columns available
// For rows 0 and 5 (indices 0-3 and 28-31), only columns 1-4 available

function getLEDIndex(row, col) {
  // Handle edge cases for rows 0 and 5 (only columns 1-4 valid)
  if (row === 0 && (col < 1 || col > 4)) return null;
  if (row === 5 && (col < 1 || col > 4)) return null;
  if (row >= 1 && row <= 4 && (col < 0 || col > 5)) return null;
  if (row < 0 || row > 5) return null;
  
  // Calculate LED index based on row
  // Row 0: 0-3 (col 1-4 maps to 0-3)
  // Row 1: 4-9 (col 0-5 maps to 4-9)
  // Row 2: 10-15
  // Row 3: 16-21
  // Row 4: 22-27
  // Row 5: 28-31 (col 1-4 maps to 28-31)
  
  if (row === 0) return col - 1;        // cols 1-4 -> 0-3
  if (row >= 1 && row <= 4) return (row * 6) + col;  // rows 1-4: 4 + col
  if (row === 5) return 28 + (col - 1); // cols 1-4 -> 28-31
  
  return null;
}

function reverseBits(byte) {
  let result = 0;
  for (let i = 0; i < 8; i++) {
    if ((byte & (1 << i)) !== 0) {
      result |= 1 << (7 - i);
    }
  }
  return result;
}

function letterToLightArray(letter) {
  const pattern = letterPatterns[letter.toLowerCase()] || letterPatterns[letter];
  if (!pattern) {
    console.log(`Warning: No pattern found for letter '${letter}'`);
    return [0x00, 0x00, 0x00, 0x00];
  }
  
  // Initialize 4 bytes (32 LEDs)
  const bytes = [0x00, 0x00, 0x00, 0x00];
  
  // Place the letter pattern in the grid
  for (let row = 0; row < 5; row++) {
    for (let col = 0; col < 3; col++) {
      if (pattern[row][col] === 1) {
        const gridRow = row + ROW_OFFSET;
        const gridCol = col + COL_OFFSET;
        const ledIndex = getLEDIndex(gridRow, gridCol);
        
        if (ledIndex !== null && ledIndex >= 0 && ledIndex < 32) {
          const byteIndex = Math.floor(ledIndex / 8);
          const bitIndex = ledIndex % 8;
          bytes[byteIndex] |= (1 << bitIndex);
        }
      }
    }
  }
  
  return bytes;
}

function bytesToHex(bytes) {
  return bytes.map(b => '0x' + b.toString(16).toUpperCase().padStart(2, '0'));
}

// Generate all alphabet letters
function generateAlphabet() {
  const uppercase = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
  const lowercase = 'abcdefghijklmnopqrstuvwxyz';
  const allLetters = uppercase + lowercase;
  
  console.log('// IBM LPFK Letter Light Arrays');
  console.log('// Generated by letterPatternGenerator.js');
  console.log('// Format: 4 bytes (LEDs 0-31)');
  console.log('');
  
  const results = {};
  
  for (const letter of allLetters) {
    const bytes = letterToLightArray(letter);
    const hexStr = bytesToHex(bytes);
    const reversedHex = bytesToHex(bytes.map(reverseBits));
    
    results[letter] = {
      raw: bytes,
      hex: hexStr,
      reversed: reversedHex,
      arduinoMacro: hexStr.join(', ')
    };
    
    // Print Arduino #define format
    console.log(`#define letter${letter} ${hexStr.join(', ')} // "${letter}"`);
  }
  
  console.log('');
  console.log('// Reversed (for Arduino direct use):');
  console.log('');
  
  for (const letter of allLetters) {
    const bytes = letterToLightArray(letter);
    const reversedHex = bytesToHex(bytes.map(reverseBits));
    console.log(`#define letter${letter}_rev ${reversedHex.join(', ')} // "${letter}"`);
  }
  
  return results;
}

// Visualize a letter pattern
function visualizeLetter(letter) {
  const pattern = letterPatterns[letter.toLowerCase()] || letterPatterns[letter];
  if (!pattern) {
    console.log(`No pattern for: ${letter}`);
    return;
  }
  
  console.log(`\nLetter: ${letter}`);
  console.log('Grid (6 cols x 6 rows):');
  
  // Print empty grid first
  const display = [];
  for (let r = 0; r < 6; r++) {
    display[r] = [];
    for (let c = 0; c < 6; c++) {
      display[r][c] = '·';
    }
  }
  
  // Place pattern
  for (let row = 0; row < 5; row++) {
    for (let col = 0; col < 3; col++) {
      if (pattern[row][col] === 1) {
        const gridRow = row + ROW_OFFSET;
        const gridCol = col + COL_OFFSET;
        display[gridRow][gridCol] = '█';
      }
    }
  }
  
  // Print with LED indices
  for (let r = 0; r < 6; r++) {
    let line = `Row ${r}: `;
    for (let c = 0; c < 6; c++) {
      line += display[r][c] + ' ';
    }
    const ledIdx = getLEDIndex(r, c-1);
    console.log(line);
  }
  
  // Show bytes
  const bytes = letterToLightArray(letter);
  console.log('\nBytes (normal):   ', bytesToHex(bytes));
  console.log('Bytes (reversed):  ', bytesToHex(bytes.map(reverseBits)));
}

// Run
console.log('=== IBM LPFK Letter Pattern Generator ===\n');
generateAlphabet();

console.log('\n\n=== Sample Visualizations ===\n');
visualizeLetter('A');
visualizeLetter('D');
visualizeLetter('a');
