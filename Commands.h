#pragma once
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

void set_command_args(uint8_t* command, uint8_t com_size, uint8_t arg);