/*
*
*   Created on: 29/03/2022
*   Author: Mario Paja
*
*
*/

#include "SSD1306.h"
#include "mbed.h"
#include "Font.h"
#include "commands.h"


SSD1306::SSD1306(PinName sda, PinName scl, char displayAddress) {
	currentTextPosition = 0;
	displayBuffer = new char[1024];
	if (!displayBuffer) {
		printf("SSD1306: Framebuffer allocation failed!\r\n");
		while (true) {}
	}

	_I2C = new I2C(sda, scl);
	I2CAddress = displayAddress;
	deleteI2C = false;
#ifdef SSD1306_DEBUG
	printf("SSD1306 debug: fb = 0x%08.8X\r\n", displayBuffer);
#endif
}

SSD1306::SSD1306(I2C& busI2C, char displayAddress) {
	_I2C = &busI2C;
	currentTextPosition = 0;
	displayBuffer = new char[1024];
	if (!displayBuffer) {
		printf("SSD1306: Framebuffer allocation failed!\r\n");
		while (true) {}
	}
	displayAddress = displayAddress;
	deleteI2C = true;
}

void SSD1306::setSpeed(speedMode spd) {
	switch (spd) {
	case Slow:
		_I2C->frequency(100000);
		break;
	case Medium:
		_I2C->frequency(400000);
		break;
	case Fast:
		_I2C->frequency(1000000);
		break;
	}
}

int SSD1306::sendCommandData(char c, char c_or_d, char lastitem) {
	int res;

	_I2C->start();
	res = _I2C->write(I2CAddress);
	if (!res) goto terminate_transaction;
	res = _I2C->write(c_or_d | lastitem);
	if (!res) goto terminate_transaction;
	res = _I2C->write(c);
	if (!res) goto terminate_transaction;

terminate_transaction:
	_I2C->stop();
	return res;
}

int SSD1306::sendCommand(char command) {
	return sendCommandData(command, SSD1306_IS_COMMAND, SSD1306_IS_LAST);
}

int SSD1306::sendData(char data) {
	return sendCommandData(data, SSD1306_IS_DATA, SSD1306_IS_LAST);
}

int SSD1306::init(void) {
	static const char comando[] = { 0x80, SSD1306_DISPLAYOFF,
										 0x80, SSD1306_CHARGEPUMP,
										 0x80, 0x14,
										 0x80, SSD1306_MEMORYMODE,
										 0x80, 0x00,
										 0x80, SSD1306_SEGREMAP | 0x1,
										 0x80, SSD1306_COMSCANDEC,
										 0x80, SSD1306_SETBRIGHTNESS,
										 0x80, 0x7F,
										 0x00, SSD1306_DISPLAYON
	};

	return _I2C->write(I2CAddress, comando, sizeof comando);
}

void SSD1306::scroll(bool refresh) {
	for (int i = 128; i < 1024; i++)
		displayBuffer[i - 128] = displayBuffer[i];

	for (int i = 896; i < 1024; i++)
		displayBuffer[i] = 0;

	if (refresh)
		refreshDisplay();
}

void SSD1306::setCursor(char row, char column) {
	currentTextPosition = row * 128 + column * 8;
}

void SSD1306::printChar(char c, bool refresh) {
	int idx;

	idx = c * 8;
	if (currentTextPosition == 1024) {
		scroll(refresh);
		currentTextPosition = 896;
	}
	for (int i = 0; i < 8; i++) {
		displayBuffer[currentTextPosition] = charset[idx + i];
		currentTextPosition++;
	}
	if (refresh)
		refreshDisplay();
}

void SSD1306::printString(char* s, bool refresh) {
	while (*s) printChar(*s++, refresh);
}

void SSD1306::printf(const char* fmt, ...) {
	char* s = new char[129];
	va_list args;

	va_start(args, fmt);
	vsnprintf(s, 128, fmt, args);
	printString(s, false);
	delete[] s;
	va_end(args);
}


void SSD1306::refreshDisplay(void) {

	sendCommand(0xb0);
	sendCommand(SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
	sendCommand(SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
	sendCommand(SSD1306_SETSTARTLINE | 0x0); // line #0

	_I2C->start();
	_I2C->write(I2CAddress);
	_I2C->write(0x40);

	for (int i = 0; i < 1024; i++)
		_I2C->write(displayBuffer[i]);

	_I2C->stop();
}


void SSD1306::setBrightness(char brightness) {
	sendCommand(SSD1306_SETBRIGHTNESS);
	sendCommand(brightness);
}

void SSD1306::clearScreen() {
	for (int i = 0; i < 1024; i++)
		displayBuffer[i] = 0;

	setCursor(0, 0);
	currentTextPosition = 0;
	refreshDisplay();
}

void SSD1306::printPixel(char x, char y, printMode mode, bool refresh) {
	x = x % 128;
	y = y % 64;

	switch (mode) {
	case Normal:
		displayBuffer[(y / 8) * 128 + x] |= (1 << (y % 8));
		break;
	case Inverse:
		displayBuffer[(y / 8) * 128 + x] &= ~(1 << (y % 8));
		break;
	case Xor:
		displayBuffer[(y / 8) * 128 + x] ^= (1 << (y % 8));
		break;
	}
	if (refresh)
		refreshDisplay();
}

bool SSD1306::getPixelState(char x, char y) {
	x = x % 128;
	y = y % 64;

	if (displayBuffer[(y / 8) * 128 + x] & (1 << (y % 8)))
		return true;
	else
		return false;
}


void SSD1306::sleep() {
	sendCommand(0xAE);
}

void SSD1306::wake() {
	sendCommand(0xAF);
}

void SSD1306::turnOff() {
	sendCommand(0xAE);
}

void SSD1306::turnOn() {
	sendCommand(0xAF);
}
