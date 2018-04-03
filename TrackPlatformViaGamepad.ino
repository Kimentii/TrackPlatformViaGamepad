#include <XBOXUSB.h>
#include <SoftwareSerial.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXUSB Xbox(&Usb);
SoftwareSerial bluetoothSerial(2, 3);

void setup() {
	Serial.begin(115200);
	bluetoothSerial.begin(38400);
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
		if (Xbox.getButtonPress(L2) || Xbox.getButtonPress(R2)) {
			Serial.print("L2: ");
			Serial.print(Xbox.getButtonPress(L2));
			Serial.print("\tR2: ");
			Serial.println(Xbox.getButtonPress(R2));
			Xbox.setRumbleOn(Xbox.getButtonPress(L2), Xbox.getButtonPress(R2));
		}
		else
			Xbox.setRumbleOn(0, 0);

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
			bluetoothSerial.write("");
		}
		if (Xbox.getButtonClick(B)) {
			Serial.println(F("B"));
			if (bluetoothSerial.available())
				bluetoothSerial.read();
			bluetoothSerial.write("");
		}
		if (Xbox.getButtonClick(X)) {
			Serial.println(F("X"));
			if (bluetoothSerial.available())
				bluetoothSerial.read();
			bluetoothSerial.write("");
		}
		if (Xbox.getButtonClick(Y)) {
			Serial.println(F("Y"));
			if (bluetoothSerial.available())
				bluetoothSerial.read();
			bluetoothSerial.write("");
		}
	}
	delay(1);
}