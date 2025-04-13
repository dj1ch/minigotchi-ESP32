//                            USER DEFINED SETTINGS
//                         (For M5Stack AtomS3(R) Dev Kit)
//
//   Set driver type, pins used and SPI control method etc
//
//   See the User_Setup_Select.h file if you wish to be able to define multiple
//   setups and then easily select which setup file is used by the compiler.

// ##############################################################################################
//
// Section 1. Call up the right driver file and any options for it
//
// ##############################################################################################

#define CGRAM_OFFSET

// Only define one driver, the other one must be commented out
#define ST7789_DRIVER // AtomS3 Dev Kit driver

// ##############################################################################################

// For ST7789, define the colour order IF the blue and red are swapped on your display
#define TFT_RGB_ORDER TFT_RGB // Colour order Red-Green-Blue

// For AtomS3, define the pixel width and height in portrait orientation
#define TFT_WIDTH 128  // AtomS3 Dev Kit
#define TFT_HEIGHT 128 // AtomS3 Dev Kit

// ##############################################################################################
// If colours are inverted (white shows as black) then uncomment one of the next
// 2 lines try both options, one of the options should correct the inversion.

// #define TFT_INVERSION_ON
// #define TFT_INVERSION_OFF
// ##############################################################################################

// Backlight control
#define TFT_BACKLIGHT_ON HIGH // HIGH for AtomS3 Dev Kit

// ##############################################################################################
//
// Section 2. Define the pins that are used to interface with the display here
//
// ##############################################################################################

// AtomS3 Dev Kit pins
#define TFT_CS   7  // Chip select control pin
#define TFT_DC   6  // Data Command control pin
#define TFT_RST  5  // Reset pin
#define TFT_MOSI 4  // MOSI pin
#define TFT_SCLK 3  // SCLK pin
#define TFT_BL   2  // Backlight control pin

// ##############################################################################################
//
// Section 3. Other options
//
// ##############################################################################################

// Define the SPI clock frequency
#define SPI_FREQUENCY 40000000 // 40MHz for AtomS3 Dev Kit

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY 20000000

// The ESP32 has 2 free SPI ports i.e. VSPI and HSPI, the VSPI is the default
// #define USE_HSPI_PORT // Uncomment if you need to use HSPI port