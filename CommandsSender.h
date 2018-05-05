#pragma once
#include "Commands.h"
#include <SoftwareSerial.h>
#include <FastCRC.h>

// communication
uint8_t CONNECT[] = { 4,1,4 };
uint8_t DISCONNECT[] = { 4,2 };
uint8_t REFRESH_CONNECTION[]{ 4,3 };

// movement commands
uint8_t MOVE_FORWARD[] = { 1,6,0,0,0 };
uint8_t MOVE_LEFT[] = { 1,0x0A, '-', 0,0,0 };
uint8_t MOVE_RIGHT[] = { 1, 0x0A, 0,0,0 };
uint8_t MOVE_BACK[] = { 1,6,'-',0,0,0 };
uint8_t STOP_MOVING[] = { 1,5 };

// servo commands
uint8_t SET_SERVO_XY[] = { 3,5,'1',';',0,0,0 };
uint8_t SET_SERVO_XZ[] = { 3,5,'2', ';', 0,0,0 };

const int SERIAL_SPEED = 9600;
const int BUF_SIZE = 255;

class CommandsSender {
private:
	SoftwareSerial* serial;
	FastCRC16 crc_calculator;
	
void write_command(uint8_t* command, uint8_t com_length) {
	uint8_t* command_with_length = new uint8_t[com_length + 1];
	serial->write(com_length);
	command_with_length[0] = com_length;
	for (int i = 0; i < com_length; i++) {
		serial->write(command[i]);
		command_with_length[i + 1] = command[i];
	}
	uint16_t crc = crc_calculator.modbus(command_with_length, com_length + 1);
	serial->write((uint8_t)crc);
	serial->write((uint8_t)(crc >> 8));
	delete command_with_length;
}

uint8_t synchronous_read() {
	int i = 10000;
	for (; i > 0 && !serial->available(); i--);
	if (i <= 0) {
		Serial.print("Read time out.");
		return -1;
	}
	uint8_t val = serial->read();
	return val;
}

bool read_one_answer() {
	uint8_t com_size = synchronous_read();
	char buf[BUF_SIZE] = { 0 };
	//Serial.printf("com_size: %d\n", com_size);
	for (int i = 0; i < com_size && i < BUF_SIZE; i++) {
		buf[i] = synchronous_read();
	}
	uint16_t crc = (uint8_t)synchronous_read();
	crc <<= 8;
	crc = (uint8_t)synchronous_read();
	/*Serial.print("ans: ");
	Serial.println(buf);
	Serial.flush();*/
	return !strcmp(buf, "OK");
}

bool read_two_answer() {
	bool ans1 = read_one_answer();
	bool ans2 = read_one_answer();
	return ans1 && ans2;
}

public:
	CommandsSender(SoftwareSerial* serial) {
		this->serial = serial;
		serial->begin(SERIAL_SPEED);
	}
	
	// communication
	bool connect() {
		write_command(CONNECT, sizeof(CONNECT));
		return read_two_answer();
	}

	bool disconnect() {
		write_command(DISCONNECT, sizeof(DISCONNECT));
		return read_two_answer();
	}

	// movement
	bool move_rigth(uint8_t speed) {
		set_command_args(MOVE_RIGHT, sizeof(MOVE_RIGHT), speed);
		write_command(MOVE_RIGHT, sizeof(MOVE_RIGHT));
		return read_two_answer();
	}

	bool move_left(uint8_t speed) {
		set_command_args(MOVE_LEFT, sizeof(MOVE_LEFT), speed);
		write_command(MOVE_LEFT, sizeof(MOVE_LEFT));
		return read_two_answer();
	}

	bool move_forward(uint8_t speed) {
		set_command_args(MOVE_FORWARD, sizeof(MOVE_FORWARD), speed);
		write_command(MOVE_FORWARD, sizeof(MOVE_FORWARD));
		return read_two_answer();
	}

	bool move_back(uint8_t speed) {
		set_command_args(MOVE_BACK, sizeof(MOVE_BACK), speed);
		write_command(MOVE_BACK, sizeof(MOVE_BACK));
		return read_two_answer();
	}

	// servo
	bool set_xy_servo_angle(uint8_t angle) {
		set_command_args(SET_SERVO_XY, sizeof(SET_SERVO_XY), angle);
		write_command(SET_SERVO_XY, sizeof(SET_SERVO_XY));
		return read_two_answer();
	}

	bool set_xz_servo_angle(uint8_t angle) {
		set_command_args(SET_SERVO_XZ, sizeof(SET_SERVO_XZ), angle);
		write_command(SET_SERVO_XZ, sizeof(SET_SERVO_XZ));
		return read_two_answer();
	}

	~CommandsSender() {
		serial->end();
	}
};