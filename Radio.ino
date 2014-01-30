#define RF24_TO_GROUND    0xF0F0F0F0E1LL
#define RF24_TO_FMU       0xF0F0F0F0D2LL

RF24 radio(RF24_CE_PIN, RF24_CSN_PIN);

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

