#include <SPI.h>
#include "VirtualWire.h"
#include "nRF24L01.h"
#include "RF24.h"
#include "RadioUtil.h"

#define RF24_TO_GROUND    0xF0F0F0F0E1LL
#define RF24_TO_FMU       0xF0F0F0F0D2LL

#define RF433_RECV_PIN 2
#define RF24_CE_PIN    9
#define RF24_CSN_PIN   10
#define LED_Y_PIN      15
#define LED_B_PIN      16
#define LED_G_PIN      17

RF24 radio(RF24_CE_PIN, RF24_CSN_PIN);

int pm_mgpm3 = 0;

void setupRF433()
{
    vw_set_rx_pin(RF433_RECV_PIN);
    vw_setup(2000);
    vw_rx_start();
}

void setupRF24()
{
    radio.begin();
    radio.enableDynamicPayloads();
    radio.setChannel(21);
    radio.setRetries(15,15);
    radio.openWritingPipe(RF24_TO_FMU);
    radio.openReadingPipe(1, RF24_TO_GROUND);
    radio.startListening();
}

void setup()
{
    delay(5000);
    Serial.begin(38400);
    pinMode(LED_G_PIN, OUTPUT);
    pinMode(LED_B_PIN, OUTPUT);
    pinMode(LED_Y_PIN, OUTPUT);
    
    digitalWrite(LED_G_PIN, HIGH);
    digitalWrite(LED_B_PIN, HIGH);
    digitalWrite(LED_Y_PIN, HIGH);

    Serial.println("Setup RF433");
    setupRF433();

    Serial.println("Setup RF24");
    setupRF24();
    
    Serial.println("Ready in 5 seconds");
    delay(5000);
    digitalWrite(LED_G_PIN, HIGH);
}

void loop()
{
    receiveRF24Data();
    receiveRF433Data();
    delay(1);
}

void receiveRF24Data()
{
    if (radio.available()) {
        digitalWrite(LED_B_PIN, true);
        uint8_t size = radio.getDynamicPayloadSize();
        uint8_t len = size / 2;
        int16_t carrier[len];
        radio.read(&carrier, size);
        Serial.print("RF24:\t");
        for (int i = 0; i < len; i++) {
            Serial.print(carrier[i]);
            Serial.print("\t");
        }
        Serial.println("");
    } else {
        digitalWrite(LED_B_PIN, false);
    }
}

void receiveRF433Data()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
	int i;
        digitalWrite(LED_Y_PIN, true);
        if (buf[0] == RF_CARRIER_TYPE_PM25) {
            pm_mgpm3 = buf[1];
        }
	Serial.print("RF433: ");
	
	for (i = 0; i < buflen; i++)
	{
	    Serial.print(buf[i]);
	    Serial.print('\t');
	}
	Serial.println("");
    } else {
        digitalWrite(LED_Y_PIN, false);
    }
}

