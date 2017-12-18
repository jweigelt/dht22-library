/**
* @file DHT22.h
* @author Jan Weigelt (jan@janelo.net)
* @date 18 Dec 2017
* @brief DHT22 sensor control library for Arduino
*/

#ifndef _DHT22_H
#define _DHT22_H

//Read timeout (us)
#define DHT22_TIMEOUT 255

#include <Arduino.h>

class DHT22
{
public:
	/**
	* @brief Creates a new instance of DHT22
	* @param pin Arduino pin the sensor's D0 line is connected to
	*/
	DHT22(int pin);

	/**
	* @brief Retrieves data from the sensor
	* @note The sensor will only provide fresh data every 1~2 seconds thus this function should only be called every ~2s
	*/
	bool update();

	/**
	* @brief Gets the last (cached) temperature reading
	* @note Make sure that update() suceeded to ensure that valid data is returned.
	*/
	int16_t lastT();

	/**
	* @brief Gets the last (cached) rel. humidity reading
	* @note Make sure that update() suceeded to ensure that valid data is returned.
	*/
	int16_t lastH();

private:
	int pin;
	const uint8_t timeOut = DHT22_TIMEOUT;
	const uint8_t timeOutT = DHT22_TIMEOUT / 10;
	int16_t lastTemperature = 0;
	int16_t lastHumidity = 0;

	bool waitForPin(uint8_t state);
	bool readByte(uint8_t* buf);
};
#endif /* _DHT22_H */