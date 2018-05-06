#include <SoftwareSerial.h>

#include "IConnector.h"

IConnector::IConnector(Stream* ptr) : device_(ptr)
{
	device_->setTimeout(50);
}

IConnector::IConnector(int rx, int tx, unsigned long speed)
{
	SoftwareSerial* serial_ptr = new SoftwareSerial(rx, tx);
	serial_ptr->begin(speed);
	serial_ptr->listen();

	device_ = serial_ptr;
	device_->setTimeout(50);
}

void IConnector::write_answer(uint8_t* answer_ptr, int length)
{
	for (int i = 0; i < length; ++i)
	{
		device_->print((reinterpret_cast<char*>(answer_ptr))[i]);
	}
}

bool IConnector::is_need_to_read_message()
{
	return device_->available();
}

int IConnector::read_message(uint8_t* pointer, int max_length)
{
	static const uint8_t kCrcLength = sizeof(uint16_t);
	static const uint8_t kLengthLength = sizeof(uint8_t);

	if (device_->readBytes(pointer, kLengthLength) != kLengthLength)
	{
		return 0;
	}
	const uint8_t payload_length = pointer[0];
	int16_t package_length = (payload_length + kCrcLength + kLengthLength);
	package_length = (package_length > max_length) ? max_length : package_length;
	return (device_->readBytes(pointer + kLengthLength, package_length - kLengthLength) + kLengthLength);
}


IConnector::~IConnector()
{
}
