#pragma once
#include <stdint.h>
#include <Arduino.h>

extern uint8_t CONNECT[];
extern uint8_t DISCONNECT[];
extern uint8_t REFRESH_CONNECTION[];

// movement commands
extern uint8_t MOVE_FORWARD[];
extern uint8_t MOVE_LEFT[];
extern uint8_t MOVE_RIGHT[];
extern uint8_t MOVE_BACK[];
extern uint8_t STOP_MOVING[];

// servo commands
extern uint8_t SET_SERVO_XY[];
extern uint8_t SET_SERVO_XZ[];

void set_command_args(uint8_t* command, uint8_t com_size, uint8_t arg);
