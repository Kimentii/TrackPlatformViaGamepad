#include <XBOXUSB.h>
#include <SoftwareSerial.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

const int BLUETOOTH_SERIAL_SPEED = 9600;
USB Usb;
XBOXUSB Xbox(&Usb);
// 2 - rx, 3 - tx.
SoftwareSerial bluetoothSerial(2, 3);

void writeConnectCommand() {
	byte com[] = { 0x03, 0x04, 0x01, 0x04, 0xF2, 0x41 };
	for (int i = 0; i < 6; i++) {
		bluetoothSerial.write(com[i]);
	}
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
			writeConnectCommand();
			delay(200);
			while (bluetoothSerial.available()) {
				char c = bluetoothSerial.read();
				Serial.println(c, HEX);
			}
		}
		if (Xbox.getButtonClick(BACK)) {
			Xbox.setLedBlink(ALL);
			Serial.println(F("Back"));
			// Disconnecting from robot
			bluetoothSerial.write("040103", HEX);
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