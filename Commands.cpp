#include "Commands.h"

void set_command_args(uint8_t* command, uint8_t com_size, uint8_t arg) {
	char val3 = arg % 10 + '0';
	//Serial.printf("val3: %d\n", val3);
	arg /= 10;
	char val2 = arg % 10 + '0';
	//Serial.printf("val2: %d\n", val2);
	arg /= 10;
	char val1 = arg % 10 + '0';
	//Serial.printf("val1: %d\n", val1);
	command[com_size - 1] = val3;
	command[com_size - 2] = val2;
	command[com_size - 3] = val1;
}
