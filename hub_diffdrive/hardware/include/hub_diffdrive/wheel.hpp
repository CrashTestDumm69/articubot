#ifndef HUB_DIFFDRIVE__WHEEL_HPP
#define HUB_DIFFDRIVE__WHEEL_HPP

#include <string>

class Wheel
{
	public:
		std::string name = "";
		double cmd = 0;
		double vel = 0;
		double pos = 0;
		int count = 0;
		int rev = 0;

	Wheel() = default;

	Wheel(const std::string &wheel_name)
	{
		setup(wheel_name);
	}

	void setup(const std::string &wheel_name)
	{
		name = wheel_name;
	}
};


#endif // DIFFDRIVE_ARDUINO_WHEEL_HPP