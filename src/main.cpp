#include <Arduino.h>
#include "DataBank.h"
#include "DataStruct.h"


#define SECONDS_FACTOR 1000000
#define TIME_TO_SLEEP  1


void print_wakeup_reason();


RTC_DATA_ATTR uint8_t bootCount = 0;

const int MEMORY_SIZE = 2000;

RTC_DATA_ATTR byte memoryStart[MEMORY_SIZE];




void setup() {
  	Serial.begin(128000);

  	Serial.println("[Boot] Boot number: " + String(++bootCount));

	print_wakeup_reason();




	esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * SECONDS_FACTOR);
	Serial.println("[Boot] Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds\n");

	Serial.println("[Boot] Going to sleep now \n\n");
  	delay(500);
  	Serial.flush(); 
  	esp_deep_sleep_start();
}

void loop() {
  	Serial.println("Hello World!");
  	delay(100); 
}


void print_wakeup_reason(){
	esp_sleep_wakeup_cause_t wakeup_reason;

	wakeup_reason = esp_sleep_get_wakeup_cause();

	switch(wakeup_reason)
	{
		case ESP_SLEEP_WAKEUP_EXT0 : 
			Serial.println("[Boot] Wakeup caused by external signal using RTC_IO"); 
			break;
		case ESP_SLEEP_WAKEUP_EXT1 : 
			Serial.println("[Boot] Wakeup caused by external signal using RTC_CNTL"); 
			break;
		case ESP_SLEEP_WAKEUP_TIMER : 
			Serial.println("[Boot] Wakeup caused by timer"); 
			break;
		case ESP_SLEEP_WAKEUP_TOUCHPAD : 
			Serial.println("[Boot] Wakeup caused by touchpad"); 
			break;
		case ESP_SLEEP_WAKEUP_ULP : 
			Serial.println("[Boot] Wakeup caused by ULP program"); 
			break;
		default : 
			Serial.printf("[Boot] Wakeup was not caused by deep sleep: %d\n",wakeup_reason); 
			break;
  	}
}
