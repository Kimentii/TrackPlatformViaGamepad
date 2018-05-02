#include <XBOXUSB.h>
#include <SoftwareSerial.h>
#include <FastCRC.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "Commands.h"

const int BLUETOOTH_SERIAL_SPEED = 9600;
const int BUF_SIZE = 255;
FastCRC16 crc_calculator;
USB Usb;
XBOXUSB Xbox(&Usb);
// 2 - rx, 3 - tx.
SoftwareSerial bluetoothSerial(2, 3);

void write_command(uint8_t* command, uint8_t com_length) {
	uint8_t* command_with_length = new uint8_t[com_length + 1];
	bluetoothSerial.write(com_length);
	command_with_length[0] = com_length;
	for (int i = 0; i < com_length; i++) {
		bluetoothSerial.write(command[i]);
		command_with_length[i + 1] = command[i];
	}
	uint16_t crc = crc_calculator.modbus(command_with_length, com_length + 1);
	bluetoothSerial.write((uint8_t)crc);
	bluetoothSerial.write((uint8_t)(crc >> 8));
	delete command_with_length;
}

uint8_t synchronous_read(SoftwareSerial& serial) {
	int i = 10000;
	for (; i > 0 && !serial.available(); i--);
	if (i <= 0) {
		Serial.print("Read time out.");
		return -1;
	}
	uint8_t com_size = bluetoothSerial.read();
}

bool read_one_answer() {
	uint8_t com_size = synchronous_read(bluetoothSerial);
	char buf[BUF_SIZE] = { 0 };
	Serial.printf("com_size: %d\n", com_size);
	for (int i = 0; i < com_size && i < BUF_SIZE; i++) {
		buf[i] = synchronous_read(bluetoothSerial);
	}
	uint16_t crc = (uint8_t)synchronous_read(bluetoothSerial);
	crc << 8;
	crc = (uint8_t)synchronous_read(bluetoothSerial);
	Serial.print("ans: ");
	Serial.println(buf);
	Serial.flush();
	return !strcmp(buf, "OK");
}

void setup() {
	Serial.begin(115200);
	bluetoothSerial.begin(BLUETOOTH_SERIAL_SPEED);
#if !defined(__MIPSEL__)
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
	if (Usb.Init() == -1) {
		Serial.print(F("\r\nOSC did not start"));
		while (1); //halt
	}
	Serial.print(F("\r\nXBOX USB Library Started"));
}

void loop() {
	Usb.Task();
	if (Xbox.Xbox360Connected) {
		Xbox.setRumbleOn(0, 0);

		if (Xbox.getButtonClick(START)) {
			Xbox.setLedMode(ALTERNATING);
			Serial.println(F("Start"));
			// Connecting to robot
			write_command(CONNECT, sizeof(CONNECT));
			read_one_answer();
			read_one_answer();
		}
		if (Xbox.getButtonClick(BACK)) {
			Xbox.setLedBlink(ALL);
			Serial.println(F("Back"));
			// Disconnecting from robot
			write_command(DISCONNECT, sizeof(DISCONNECT));
			read_one_answer();
			read_one_answer();
		}

		if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
			if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
				Serial.print(F("LeftHatX: "));
				Serial.print(Xbox.getAnalogHat(LeftHatX));
				Serial.print("\t");
			}
			if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
				Serial.print(F("LeftHatY: "));
				Serial.print(Xbox.getAnalogHat(LeftHatY));
				Serial.print("\t");
			}
			if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
				Serial.print(F("RightHatX: "));
				Serial.print(Xbox.getAnalogHat(RightHatX));
				Serial.print("\t");
			}
			if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
				Serial.print(F("RightHatY: "));
				Serial.print(Xbox.getAnalogHat(RightHatY));
			}
			Serial.println();
		}

		if (Xbox.getButtonClick(UP)) {
			Xbox.setLedOn(LED1);
			Serial.println(F("Up"));
		}
		if (Xbox.getButtonClick(DOWN)) {
			Xbox.setLedOn(LED4);
			Serial.println(F("Down"));
		}
		if (Xbox.getButtonClick(LEFT)) {
			Xbox.setLedOn(LED3);
			Serial.println(F("Left"));
		}
		if (Xbox.getButtonClick(RIGHT)) {
			Xbox.setLedOn(LED2);
			Serial.println(F("Right"));
		}

		if (Xbox.getButtonClick(A)) {
			Serial.println(F("A"));
			if (bluetoothSerial.available())
				bluetoothSerial.read();
			bluetoothSerial.write("A");
		}
		if (Xbox.getButtonClick(B)) {
			Serial.println(F("B"));
			if (bluetoothSerial.available())
				bluetoothSerial.read();
			bluetoothSerial.write("B");
		}
		if (Xbox.getButtonClick(X)) {
			Serial.println(F("X"));
			if (bluetoothSerial.available())
				bluetoothSerial.read();
			bluetoothSerial.write("X");
		}
		if (Xbox.getButtonClick(Y)) {
			Serial.println(F("Y"));
			if (bluetoothSerial.available())
				bluetoothSerial.read();
			bluetoothSerial.write("Y");
		}
	}
	delay(1);
}