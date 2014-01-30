#include <SPI.h>
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#endif

#include "nRF24L01.h"
#include "RF24.h"
#include "RadioUtil.h"

#include "I2Cdev.h"
#include "ADXL345.h"
#include "BMP085.h"
#include "HMC5883L.h"
#include "L3G4200D.h"

#define STATUS_NO_ERROR     0
#define STATUS_MOTION_ERROR 11

#define RF24_CE_PIN    9
#define RF24_CSN_PIN   10
#define LED_Y_PIN      15
#define LED_B_PIN      16
#define LED_G_PIN      17

uint8_t FMC_STATUS = STATUS_NO_ERROR;

void setup()
{
    delay(1000);
    Serial.begin(38400);
    pinMode(LED_G_PIN, OUTPUT);
    pinMode(LED_B_PIN, OUTPUT);
    pinMode(LED_Y_PIN, OUTPUT);
    
    digitalWrite(LED_G_PIN, HIGH);
    digitalWrite(LED_B_PIN, HIGH);
    digitalWrite(LED_Y_PIN, HIGH);
    
    Serial.println("Setup Motion Sensor");
    setupMotion();

    Serial.println("Setup RF24");
    setupRF24();
    
    Serial.println("Ready in 5 seconds");
    delay(5000);
    digitalWrite(LED_G_PIN, HIGH);
    digitalWrite(LED_B_PIN, LOW);
    digitalWrite(LED_Y_PIN, LOW);
}

void loop()
{
    receiveRF24Data();
    readMotion();
    calcMotion();
    printMotion();
    delay(20);
}

