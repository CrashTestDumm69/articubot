#ifndef HUB_DIFFDRIVE__HUB_COMMS_HPP_
#define HUB_DIFFDRIVE__HUB_COMMS_HPP_

#ifdef _WIN64
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <sstream>
#include <libserial/SerialPort.h>
#include <iostream>


LibSerial::BaudRate convert_baud_rate(int baud_rate)
{
	switch (baud_rate) 
	{
		case 1200: return LibSerial::BaudRate::BAUD_1200;
		case 1800: return LibSerial::BaudRate::BAUD_1800;
		case 2400: return LibSerial::BaudRate::BAUD_2400;
		case 4800: return LibSerial::BaudRate::BAUD_4800;
		case 9600: return LibSerial::BaudRate::BAUD_9600;
		case 19200: return LibSerial::BaudRate::BAUD_19200;
		case 38400: return LibSerial::BaudRate::BAUD_38400;
		case 57600: return LibSerial::BaudRate::BAUD_57600;
		case 115200: return LibSerial::BaudRate::BAUD_115200;
		case 230400: return LibSerial::BaudRate::BAUD_230400;
	default:
		std::cout << "Error! Baud rate " << baud_rate << " not supported! Default to 115200" << std::endl;
		return LibSerial::BaudRate::BAUD_115200;
	}
}

class HubComms 
{

	int count_l = 0;
	int count_r = 0; 
	public:

		HubComms() = default;

		void connect(const std::string &serial_device, int32_t baud_rate, int32_t timeout_ms)
		{  
			timeout_ms_ = timeout_ms;
			serial_conn_.Open(serial_device);
			serial_conn_.SetBaudRate(convert_baud_rate(baud_rate));
		}

		void disconnect()
		{
			serial_conn_.Close();
		}

		bool connected() const
		{
			return serial_conn_.IsOpen();
		}

		void send_empty_msg(){}

		std::string sendMsg(const std::string &msg_to_send)
		{
			serial_conn_.Write(msg_to_send);
			std::string response = "";
			try
			{
				serial_conn_.ReadLine(response,'\n',timeout_ms_);
			}
			catch(...)
			{
				return response;
			}
			return response;
		}

		void read_motor(double &vval_1, double &vval_2, int &pval_1, int &pval_2)
		{
			std::string response = sendMsg("r\r");
			const char* delimiter = " ";
			char line[20];
			strcpy(line,response.c_str());
			char* token = strtok(line,delimiter);
			try
			{
				vval_1 = atoi(token);
				token = strtok(NULL,delimiter);
		    	pval_1 = atoi(token);
		    	token = strtok(NULL,delimiter);
		    	vval_2 = atoi(token);
		    	token = strtok(NULL,delimiter);
		    	pval_2 = atoi(token);
		    }
		    catch(...)
		    {
		    	return;
		    }	
		}

		void set_motor_values(int val_l, int val_r)
		{
			std::stringstream ss;
		    ss << "s " << val_l << " " << val_r << "\r";
		    sendMsg(ss.str());
    	}

private:
    LibSerial::SerialPort serial_conn_;
    int timeout_ms_;
};

#endif // DIFFDRIVE_ARDUINO_ARDUINO_COMMS_HPP