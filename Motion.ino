//The standard atmosphere: 101325 Pa[1] or 1013.25 millibars or hectopascals. 29.92 inHg
#define SEA_LVL_PRESSURE 102160

ADXL345  accelerometer;
BMP085   barometer;
HMC5883L magnetometer;
L3G4200D gyroscope;

int16_t accel[3];
int16_t gyro[3];
int16_t mag[3];
float gravity[3];

float temperature;
float pressure;
float altitude;

void setupMotion()
{
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif
    accelerometer.initialize();
    barometer.initialize();
    gyroscope.initialize();
    magnetometer.initialize();
    
    if (accelerometer.testConnection() && barometer.testConnection() && gyroscope.testConnection() && magnetometer.testConnection()) {
        accelerometer.setRange(ADXL345_RANGE_2G); // 256 = 1g
        gyroscope.setFullScale(2000);
    } else {
        FMC_STATUS = STATUS_MOTION_ERROR;
    }
}

void readMotion()
{
    accelerometer.getAcceleration(&accel[0], &accel[1], &accel[2]);
    gyroscope.getAngularVelocity(&gyro[0], &gyro[1], &gyro[2]);
    magnetometer.getHeading(&mag[0], &mag[1], &mag[2]);
    barometer.setControl(BMP085_MODE_TEMPERATURE);
    delay(barometer.getMeasureDelayMicroseconds()/1000);
    temperature = barometer.getTemperatureC();
    barometer.setControl(BMP085_MODE_PRESSURE_3);
    delay(barometer.getMeasureDelayMicroseconds()/1000);
    pressure = barometer.getPressure();
    altitude = barometer.getAltitude(pressure, SEA_LVL_PRESSURE);
}

void calcMotion()
{
    for (int i = 0; i < 3; i++) {
        gravity[i] = accel[i] / 256.0;
    }
}

void printMotion()
{
    Serial.print(accel[0]);
    Serial.print("\t");
    Serial.print(accel[1]);
    Serial.print("\t");
    Serial.print(accel[2]);
    Serial.print("\t");
    Serial.print(gyro[0]);
    Serial.print("\t");
    Serial.print(gyro[1]);
    Serial.print("\t");
    Serial.print(gyro[2]);
    Serial.print("\t");
    Serial.print(gravity[0]);
    Serial.print("\t");
    Serial.print(gravity[1]);
    Serial.print("\t");
    Serial.print(gravity[2]);
    Serial.print("\t");
    Serial.print(temperature);
    Serial.print("\t");
    Serial.print(pressure);
    Serial.print("\t");
    Serial.print(altitude);
    Serial.println("");
}
