#include <XBOXUSB.h>
#include <SoftwareSerial.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "CommandsSender.h"


const int BLUETOOTH_SERIAL_SPEED = 38400;
USB Usb;
XBOXUSB Xbox(&Usb);
// 2 - rx, 3 - tx.
SoftwareSerial bluetoothSerial(2, 3);
CommandsSender com_sender(&bluetoothSerial);


void setup() {
	Serial.begin(115200);
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
			com_sender.connect();
		}
		if (Xbox.getButtonClick(BACK)) {
			Xbox.setLedBlink(ALL);
			Serial.println(F("Back"));
			// Disconnecting from robot
			com_sender.disconnect();
		}

		if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
			if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
				Serial.print(F("LeftHatX: "));
				int16_t val = Xbox.getAnalogHat(LeftHatX);
				if (val > 0) {
					val -= 7500;
					float speed = ((float)val / 25267.0) * 255;
					com_sender.move_rigth(speed);
				}
				else {
					val += 7500;
					float speed = ((float)val / -25268.0) * 255;
					com_sender.move_left(speed);
				}
				Serial.print(val);
				Serial.print("\t");
			}
			if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
				Serial.print(F("LeftHatY: "));
				int16_t val = Xbox.getAnalogHat(LeftHatY);
				if (val > 0) {
					val -= 7500;
					float speed = ((float)val / 25267.0) * 255;
					com_sender.move_forward(speed);
				}
				else {
					val += 7500;
					float speed = ((float)val / -25268.0) * 255;
					com_sender.move_back(speed);
				}
				Serial.print(val);
				Serial.print("\t");
			}
			if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
				Serial.print(F("RightHatX: "));
				int16_t val = Xbox.getAnalogHat(RightHatX);
				if (val > 0) {
					float angle = ((float)val / 32768.0) * 180;
					com_sender.set_xy_servo_angle(angle);
				}
				Serial.print(val);
				Serial.print("\t");
			}
			if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
				Serial.print(F("RightHatY: "));
				int16_t val = Xbox.getAnalogHat(RightHatY);
				if (val > 0) {
					float angle = ((float)val / 32768.0) * 180;
					com_sender.set_xz_servo_angle(angle);
				}
				Serial.print(val);
			}
			Serial.println();
		}

	}
	delay(1);
}
