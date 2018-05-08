#include <XBOXUSB.h>
#include <SoftwareSerial.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "CommandsSender.h"
#include "DebugSerial.h"


//const int BLUETOOTH_SERIAL_SPEED = 38400;
USB Usb;
XBOXUSB Xbox(&Usb);
// 2 - rx, 3 - tx.
SoftwareSerial bluetoothSerial(2, 3);
CommandsSender com_sender(&bluetoothSerial);
bool is_connected = false;


void setup() {
#if !defined(__MIPSEL__)
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
	if (Usb.Init() == -1) {
		DEBUG_PRINT("\r\nOSC did not start");
		while (1); //halt
	}
	DEBUG_PRINTF("\r\nXBOX USB Library Started");
}

void loop() {
	Usb.Task();
	if (Xbox.Xbox360Connected) {
		if (is_connected) {
			//Xbox.setLedBlink(ALL);
			Xbox.setLedMode(ALTERNATING);
		}
		else {
			Xbox.setLedOn(LED1);
		}

		if (Xbox.getButtonClick(START)) {
			DEBUG_PRINTLN(F("Connecting..."));
			// Connecting to robot
			if (!is_connected) {
				is_connected = com_sender.connect();
			}
		}
		if (Xbox.getButtonClick(BACK)) {
			//Xbox.setLedBlink(ALL);
			DEBUG_PRINTLN(F("Disconnecting..."));
			// Disconnecting from robot
			bool res = com_sender.disconnect();
			if (res && is_connected) {
				is_connected = false;
			}
		}

		//if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
			if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
				DEBUG_PRINT(F("LeftHatX: "));
				int16_t val = Xbox.getAnalogHat(LeftHatX);
				if (val > 0) {
					val -= 7500;
					float speed = ((float)val / 25267.0) * 255;
					is_connected = com_sender.move_rigth(speed);
				}
				else {
					val += 7500;
					float speed = ((float)val / -25268.0) * 255;
					is_connected = com_sender.move_left(speed);
				}
				DEBUG_PRINT(String(val));
				DEBUG_PRINT("\t");
			}
			if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
				DEBUG_PRINT(F("LeftHatY: "));
				int16_t val = Xbox.getAnalogHat(LeftHatY);
				if (val > 0) {
					val -= 7500;
					float speed = ((float)val / 25267.0) * 255;
					is_connected = com_sender.move_forward(speed);
				}
				else {
					val += 7500;
					float speed = ((float)val / -25268.0) * 255;
					is_connected = com_sender.move_back(speed);
				}
				DEBUG_PRINT(String(val));
				DEBUG_PRINT("\t");
			}
			if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
				DEBUG_PRINT(F("RightHatX: "));
				int16_t val = Xbox.getAnalogHat(RightHatX);
				if (val > 0) {
					val -= 7500;
					float angle = ((float)val / 25267.0) * 180;
					is_connected = com_sender.set_xy_servo_angle(angle);
				}
				DEBUG_PRINT(String(val));
				DEBUG_PRINT("\t");
			}
			else {
				is_connected = com_sender.set_xy_servo_angle(0);
			}
			if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
				DEBUG_PRINT(F("RightHatY: "));
				int16_t val = Xbox.getAnalogHat(RightHatY);
				if (val > 0) {
					val -= 7500;
					float angle = ((float)val / 25267.0) * 180;
					is_connected = com_sender.set_xz_servo_angle(angle);
				}
				DEBUG_PRINT(String(val));
			}
			else {
				is_connected = com_sender.set_xz_servo_angle(0);
			}
			DEBUG_PRINTLN("");
		//}

	}
	else {
		DEBUG_PRINTLN(String("Xbox is disconnected."));
	}
	delay(1);
}
