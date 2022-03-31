/*
*
*   Created on: 29/03/2022
*   Author: Mario Paja
*
*
*/

#ifndef MBED_SSD1306_LIBRARY
#define MBED_SSD1306_LIBRARY

#include "mbed.h"

#define SSD1306_IS_COMMAND         0x00
#define SSD1306_IS_DATA            0x40
//#define SSD1306_IS_NOT_LAST        0x80
#define SSD1306_IS_LAST            0x00

#define SSD1306_SETBRIGHTNESS 0x81
//#define SSD1306_DISPLAYALLON_RESUME 0xA4
//#define SSD1306_DISPLAYALLON 0xA5
//#define SSD1306_NORMALDISPLAY 0xA6
//#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

//#define SSD1306_SETDISPLAYOFFSET 0xD3
//#define SSD1306_SETCOMPINS 0xDA

//#define SSD1306_SETVCOMDETECT 0xDB

//#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
//#define SSD1306_SETPRECHARGE 0xD9

//#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20

//#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

//#define SSD1306_EXTERNALVCC 0x1
//#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
//#define SSD1306_ACTIVATE_SCROLL 0x2F
//#define SSD1306_DEACTIVATE_SCROLL 0x2E
//#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
//#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
//#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
//#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
//#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

/**
 *  SSD1306
 *  Library enables interaction with SSD1306 (128x64) OLED display
 *  Code was tested on a Nucleo-64 F446RE board
 *
 * Example of use:
 * @code
	#include "mbed.h"
	#include "SSD1306.h"
	
	SSD1306 display (D14, D15);

	int main()
	{
		display.setSpeed(SSD1306::Medium);
		display.init();

		display.clearScreen();

		display.setCursor(1,2);
		display.printf ("Hello World");

		display.refreshDisplay();

		unsigned char level = 255;
		display.setBrightness(level);

		while (true) {
			wait_us (20000);
		}
	}
 * @endcode
 */

/*!
* @class       SSD1306
* @abstract    One of the best space interceptor.
* @discussion  The mythic spaceship used by the
*              Rebellion against the Empire.
*/
class SSD1306
{
public:
	/*!
	 * @enum        printMode
	 *              Select print mode
	 * @constant    Normal The point is set on the display
	 * @constant    Inverse The point is erased on the display
	 * @constant    Xor Erase pixel if it is on, otherwise set it on
	 */
	enum printMode
	{
		Normal,
		Inverse,
		Xor
	};

	


	/**
	 * \brief Select I2C bus speed
	 * \param Slow I2C frequency is set to 100 kHz
	 * \param Medium I2C frequency is set to 400 kHz
	 * \param Fast I2C frequency is set to 1 MHz. Use it only with short connections to host
	 */
	enum speedMode
	{
		Slow,
		Medium,
		Fast
	};

	/**
	 * \brief Creates an instance of a SSD1306 specifying I2C pins to use
	 *
	 * \param SDA I2C data line pin
	 * \param SCL I2C clock line pin
	 */
	SSD1306(PinName SDA, PinName SCL, char displayAddress = 0x78);

	/**
	 * Create an instance of a SSD1306
	 *
	 * @param busI2C I2C object
	 * @param displayAddress I2C addr of SSD1306 controller
	 */
	SSD1306(I2C& busI2C, char displayAddress = 0x78);

	/**
	 * \brief Set the frequency of the I2C interface
	 *
	 * \param speedHz The bus frequency in hertz
	 */
	void setSpeed(speedMode speedHz);

	/**
	 * \brief Initialize the SSD1306 display
	 */
	int init(void);

	/**
	 * \brief Scroll up, one text line
	 *
	 * \param refresh (Optional) Refresh Display
	 */
	void scroll(bool refresh = false);

	/**
	 * \brief Print a character
	 *
	 * \param _char ASCII code of the character to print. For more info visit https://www.ascii-codes.com/
	 * \param refresh (Optional) Refresh Display
	 */
	void printChar(char _char, bool refresh = false);


	/**
	 * \brief Print constant pointer string
	 *
	 * \param fmt constant pointer string
	 */
	void printf(const char* fmt, ...);

	/**
	 * \brief Set printing cursor
	 *
	 * \param row Expected integer value (0-7)
	 * \param column Expected integer value (0-15)
	 */
	void setCursor(char row, char column);

	/**
	 * \brief Refresh display.
	 * \brief Send data from memory to display
	 */
	void refreshDisplay(void);

	/**
	 * \brief Set display brightness
	 *
	 * \param brightnessLevel Expected integer value (0-255)
	 */
	void setBrightness(char brightnessLevel);

	/**
	 * \brief Clear screen from all text
	 * \brief Resets cursor to (0,0)
	 */
	void clearScreen();

	/**
	 * \brief Prints one pixel.
	 *
	 * \param x			X Coordinate (0-127)
	 * \param y			Y Coordinate (0-63)
	 * \param mode		Select print mode, otherwise Normal
	 * \param refresh	(Optional) Refresh Display
	 */
	void printPixel(char x, char y, printMode mode = Normal, bool refresh = false);

	/**
	 * \brief Returns pixel state
	 *
	 * \param x X Coordinate (0-127)
	 * \param y Y Coordinate (0-63)
	 * \return true If the pixel was set, or false otherwise
	 */
	bool getPixelState(char x, char y);

	/**
	 * Draw line using Bresenham Algorithm
	 *
	 * @param xStart Value (0-127)
	 * @param yStart Value (0-63)
	 * @param xEnd Value (0-127)
	 * @param yEnd Value (0-63)
	 * @param mode. Select print mode, otherwise Normal
	 * @param refresh (Optional) Refresh Display
	 */
	void drawLine(char xStart, char yStart, char xEnd, char yEnd, printMode mode = Normal, bool refresh = false);

	/**
	 * Turn the whole display off.
	 * Reset display configuration
	 */
	void turnOff();

	/** Turn the whole display on.  Used during initialisation. */
	void turnOn();

	/**
	 * \brief Puts display on sleep
	 */
	void sleep();

	void wake();

	/**
	 * \brief 
	 * \param c 
	 * \param c_or_d 
	 * \param lastitem 
	 * \return 
	 */
	int sendCommandData(char c, char c_or_d, char lastitem);
	// Sends a command or data and signal if it is not the last command/data in a list

	virtual ~SSD1306()
	{
		if (!deleteI2C)
			delete _I2C;
		delete displayBuffer;
	}

private:
	/**
	 * Print C string
	 *
	 * @param String C string
	 * @param refresh (Optional) Refresh Display
	 */
	void printString(char* String, bool refresh = false);

protected:
	I2C* _I2C; // I2C object
	char I2CAddress; // I2C address of SSD1306 controller
	char* displayBuffer; // pointer to display buffer (1024 bytes)
	bool deleteI2C;
	int currentTextPosition; // Current text position (referred to screen address memory)
	int sendCommand(char c); // Sends a I2C command to SSD1306
	int sendData(char d); // Sends I2C data to SSD1306  
};

#endif
