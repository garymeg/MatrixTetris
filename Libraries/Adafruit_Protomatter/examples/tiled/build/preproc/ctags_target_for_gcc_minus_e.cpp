# 1 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\tiled\\tiled.ino"
/* ----------------------------------------------------------------------
"Tiled" Protomatter library example sketch. Demonstrates use of multiple
RGB LED matrices as a single larger drawing surface. This example is
written for two 64x32 matrices (tiled into a 64x64 display) but can be
adapted to others. If using MatrixPortal, larger multi-panel tilings like
this should be powered from a separate 5V DC supply, not the USB port
(this example works OK because the graphics are very minimal).

PLEASE SEE THE "simple" EXAMPLE FOR AN INTRODUCTORY SKETCH.
------------------------------------------------------------------------- */

# 13 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\tiled\\tiled.ino" 2

/* ----------------------------------------------------------------------
The RGB matrix must be wired to VERY SPECIFIC pins, different for each
microcontroller board. This first section sets that up for a number of
supported boards.
------------------------------------------------------------------------- */
# 103 "D:\\Documents\\Arduino\\libraries\\Adafruit_Protomatter\\examples\\tiled\\tiled.ino"
uint8_t rgbPins[] = {6, 7, 8, 9, 10, 11};
uint8_t addrPins[] = {2, 3, 4, 5};
uint8_t clockPin = 12;
uint8_t latchPin = 13;
uint8_t oePin = 14;

/* ----------------------------------------------------------------------
Matrix initialization is explained EXTENSIVELY in "simple" example sketch!
It's very similar here, but we're passing an extra argument to define the
matrix tiling along the vertical axis: -2 means there are two matrices
(or rows of matrices) arranged in a "serpentine" path (the second matrix
is rotated 180 degrees relative to the first, and positioned below).
A positive 2 would indicate a "progressive" path (both matrices are
oriented the same way), but usually requires longer cables.
------------------------------------------------------------------------- */

Adafruit_Protomatter matrix(
  64, // Width of matrix (or matrices, if tiled horizontally)
  6, // Bit depth, 1-6
  1, rgbPins, // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  false, // No double-buffering here (see "doublebuffer" example)
  -2); // Row tiling: two rows in "serpentine" path

// SETUP - RUNS ONCE AT PROGRAM START --------------------------------------

void setup(void) {
  Serial.begin(9600);

  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if(status != PROTOMATTER_OK) {
    // DO NOT CONTINUE if matrix setup encountered an error.
    for(;;);
  }

  // Since this program has no animation, all the drawing can be done
  // here in setup() rather than loop(). It's just a few basic shapes
  // that span across the matrices...nothing showy, the goal of this
  // sketch is just to demonstrate tiling basics.

  matrix.drawLine(0, 0, matrix.width() - 1, matrix.height() - 1,
    matrix.color565(255, 0, 0)); // Red line
  matrix.drawLine(matrix.width() - 1, 0, 0, matrix.height() - 1,
    matrix.color565(0, 0, 255)); // Blue line
  int radius = min(matrix.width(), matrix.height()) / 2;
  matrix.drawCircle(matrix.width() / 2, matrix.height() / 2, radius,
    matrix.color565(0, 255, 0)); // Green circle

  // AFTER DRAWING, A show() CALL IS REQUIRED TO UPDATE THE MATRIX!

  matrix.show(); // Copy data to matrix buffers
}

// LOOP - RUNS REPEATEDLY AFTER SETUP --------------------------------------

void loop(void) {
  // Since there's nothing more to be drawn, this loop() function just
  // prints the approximate refresh rate of the matrix at current settings.
  Serial.print("Refresh FPS = ~");
  Serial.println(matrix.getFrameCount());
  delay(1000);
}
