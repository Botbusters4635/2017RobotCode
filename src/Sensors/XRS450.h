/*
Creates singleton class for XRS450 Gyroscope
 */

#ifndef BOTBUSTERSREBIRTH_XRS450_SINGLETON_H
#define BOTBUSTERSREBIRTH_XRS450_SINGLETON_H

#include <WPILib.h>

/**
 * Creates a singleton class for XRS450 Gyroscope
 * @param spi_port_id SPI Port (normally SPI::PORT::kOnboardCS0)
 */
class XRS450 : public ADXRS450_Gyro {

private:
	explicit XRS450(SPI::Port spi_port_id);
	
	XRS450(const XRS450 &);
	
	XRS450 &operator=(const XRS450 &);

public:
	static XRS450 &getInstance(SPI::Port spi_port_id) {
		static XRS450 singletonInstance(spi_port_id);
		return singletonInstance;
	}
};

#endif
