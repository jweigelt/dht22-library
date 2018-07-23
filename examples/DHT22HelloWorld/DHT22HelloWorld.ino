/**
* @file DHT22HelloWorld.ino
* @author Jan Weigelt (jan@janelo.net)
* @date 18 Dec 2017
* @brief DHT22 sensor basic example
*/

#include "DHT22.h"

//The sensor's D0 pin is connected to pin 15 of the ESP/Arduino
DHT22 dht = DHT22(15);

void setup() {
	Serial.begin(115200);
}

void loop() {
	//DHT22's sensing period is ~2s
	delay(2000);

	/*
	* Call update to retrieve fresh data from the sensor
	* update() returns false if the communication to the sensor wasn't successful
	* or corrupt data was detected
	*/
	if (dht.update()) {

		/*
		* lastH() and lastT() will return cached values aquired during the last update
		* rel. Humidity is returned in percent, multiplied by 10 (50.4% would be 504)
		* Temperature is returned in °C, multiplied by 10 (21.5°C would be 215)
		*/
		Serial.printf("RH: %i, T: %i \r\n", dht.lastH(), dht.lastT());
	}
	else {
		Serial.println("Failed to read from sensor.");
	}
}
