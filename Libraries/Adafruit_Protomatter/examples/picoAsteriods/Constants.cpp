#define WIDTH       64
#define HEIGHT      64


#define ROID_VERT   10          // Average Number of Vertices in Each Asteroid
#define ROID_SPD    5          // Max Starting speed of Asteroid in pixels
#define ROID_JAG    60          // jaggedness of the Asteroids (0 = none, 100 = lots)
#define FPS         60.0
#define FRICTION    0.7         // friction coefficient of space (0 = none, 1 = lots)
#define SHOW_BOUNDARY false      // Shows element collision boundary (0 = Off, 1 = On)
#define SHOW_CENTRE   false      // Shows element Centre (0 = Off, 1 = On)

#define ASTEROID_BIG 6
#define ASTEROID_MED 4
#define ASTEROID_SML 2


#define SHIP_SIZE   5          // ship height in pixels
#define SHIP_BLINK_DUR  0.1     // duration in seconds of a single blink during ship's invisibility
#define SHIP_INV_DUR    3       // duration of the ship's invisibility in seconds
#define SHIP_THRUST     5       // acceleration of the ship in pixels per second per second
#define SHIP_TURN_SPD   360     // turn speed in degrees per second
#define SHIP_MAX_SPEED  20      // max speed (pixels) the ship can travel in one frame
#define SHIP_EXPLOSION_DUR 2.0  // How long the Ship Explodes
#define SHIP_EXPLOSION_VEL 20   // Explosion Velocity

#define LASER_DIST          0.4       // max distance laser can travel as fraction of screen width
#define LASER_EXPLODE_DUR   0.5       // duration of the laser (explosion in seconds)
#define LASER_MAX           10        // maximum number of lasers on screen at once
#define LASER_SPD           500       // speed of lasers in pixels per second

#define GAMEFONT            "Assetts/VectorWaves.ttf"

#define LEVEL_START_DELAY   3
#define BONUSLIFE           10000