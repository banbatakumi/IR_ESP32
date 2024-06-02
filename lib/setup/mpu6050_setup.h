#ifndef MPU6050_SETUP_H
#define MPU6050_SETUP_H

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

MPU6050 mpu;

// MPU control/status vars
uint8_t devStatus;       // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;     // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64];  // FIFO storage buffer

// orientation/motion vars
Quaternion q;         // [w, x, y, z]         quaternion container
VectorFloat gravity;  // [x, y, z]            gravity vector
float ypr[3];         // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

float yaw_correction;

void SetupMpu6050() {
      Wire.begin();
      Wire.setClock(400000);

      Serial.println(F("Initializing I2C devices..."));
      mpu.initialize();

      // verify connection
      Serial.println(F("Testing device connections..."));
      Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

      // load and configure the DMP
      Serial.println(F("Initializing DMP..."));
      devStatus = mpu.dmpInitialize();

      // supply your own gyro offsets here, scaled for min sensitivity
      mpu.setXAccelOffset(-330);
      mpu.setYAccelOffset(736);
      mpu.setZAccelOffset(1212);
      mpu.setXGyroOffset(-118);
      mpu.setYGyroOffset(94);
      mpu.setZGyroOffset(-17);

      // make sure it worked (returns 0 if so)
      if (devStatus == 0) {
            // Calibration Time: generate offsets and calibrate our MPU6050
            // mpu.CalibrateAccel(3);
            // mpu.CalibrateGyro(3);
            // mpu.PrintActiveOffsets();
            // turn on the DMP, now that it's ready
            Serial.println(F("Enabling DMP..."));
            mpu.setDMPEnabled(true);

            // set our DMP Ready flag so the main loop() function knows it's okay to use it
            Serial.println(F("DMP ready!"));

            // get expected DMP packet size for later comparison
            packetSize = mpu.dmpGetFIFOPacketSize();
      } else {
            // ERROR!
            // 1 = initial memory load failed
            // 2 = DMP configuration updates failed
            // (if it's going to break, usually the code will be 1)
            Serial.print(F("DMP Initialization failed (code "));
            Serial.print(devStatus);
            Serial.println(F(")"));
      }
}

#endif