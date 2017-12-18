/**
* @file DHT22.cpp
* @author Jan Weigelt (jan@janelo.net)
* @date 18 Dec 2017
* @brief DHT22 sensor control library for Arduino
*/

#include "DHT22.h"

DHT22::DHT22(int pin)
{
	this->pin = pin;
	pinMode(pin, INPUT);
}

bool DHT22::update()
{
	/*
	 * Sending the initialization sequence:
	 * - wait for idle condition (D0 high)
	 * - pull bus low
	 * - wait 1000us (max. time DHT22 needs to notice)
	 */
	if (!waitForPin(HIGH)) return false;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delayMicroseconds(1000);

	/*
	 * Waiting for confirmation from the sensor:
	 * - the sensor will pull D0 low after 20~40us
	 * - D0 will be kept low for 80us
	 * - D0 will be released and kept idle (high) for 80us
	 */
	pinMode(pin, INPUT);
	delayMicroseconds(10);

	if (!waitForPin(LOW)) return false;
	if (!waitForPin(HIGH)) return false;

	/*
	 * Reading data from the sensor:
	 * - total data size is 40bits (5 x 8bit, MSB first)
	 * - 16bit int rel. humidity (% x 10)
	 * - 16bit int temperature (°C x 10)
	 * - 8 bit int checksum
	 */
	uint8_t csum, csumc;
	if(!readByte(((uint8_t*)&lastHumidity) + 1)) return false;
	if (!readByte((uint8_t*)&lastHumidity)) return false;
	if (!readByte(((uint8_t*)&lastTemperature) + 1)) return false;
	if (!readByte((uint8_t*)&lastTemperature)) return false;
	if (!readByte(&csum)) return false;

	/*
	* The checksum is calculated by summing all data bytes
	* (Only check 8 LSB)
	*/
	csumc = *((uint8_t*)&lastHumidity)
		  + *((uint8_t*)&lastHumidity + 1)
		  + *((uint8_t*)&lastTemperature)
		  + *((uint8_t*)&lastTemperature + 1);

	/*
	 * Sign bit:
	 * - MSB 1 -> negative, MSB 0 -> positive
	 * - the databits do not get inverted for negative values
	 *   (which means they have to be inverted to match stdint format)
	 */
	if ((lastTemperature & (1 << 15)) > 0) {
		lastTemperature ^= ~(1 << 15);
	}

	return (csumc == csum);
}

bool DHT22::waitForPin(uint8_t state)
{
	/*
	 * Wait for a given pin-condition
	 * If the desired condition is not observed in a specified time,
	 * false is returned. This prevents infinte loops on fault condition.
	 */
	uint8_t ctr = 0;
	while ((digitalRead(pin) != state) && ctr < timeOut) {
		ctr++;
		delayMicroseconds(1);
	}
	return (ctr < timeOut);
}

bool DHT22::readByte(uint8_t* buf)
{
	/*
	 * Reading data bits:
	 * - before each bit, D0 is pulled low for 50us
	 * - D0 is now released and kept high for 26~28us (0) or 70us (1)
	 */

	uint8_t len;
	*buf = 0;

	for (uint8_t i = 0; i < 8; i++) {
		len = 0;
		if (!waitForPin(LOW)) return false;
		if (!waitForPin(HIGH)) return false;

		while ((digitalRead(pin) != LOW) && timeOutT > len) {
			len++;
			delayMicroseconds(10);
		}
		if (timeOutT <= len) return false;
		*buf |= (((len > 3) ? 1 : 0) << (7 - i));
	}
	return true;
}

int16_t DHT22::lastT()
{
	return lastTemperature;
}

int16_t DHT22::lastH()
{
	return lastHumidity;
}