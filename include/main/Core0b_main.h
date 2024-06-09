#ifndef _CORE0B_MAIN_H_
#define _CORE0B_MAIN_H_

#include "setup.h"

#define CORE0B_CONTROL_FREQ 500  // Hz
#define MAX_POWER 100

void Core0b_setup() {
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
#ifdef ROBOT_1
      mpu.setXAccelOffset(-3428);
      mpu.setYAccelOffset(-308);
      mpu.setZAccelOffset(882);
      mpu.setXGyroOffset(-328);
      mpu.setYGyroOffset(-534);
      mpu.setZGyroOffset(-106);
#endif
#ifdef ROBOT_2
      mpu.setXAccelOffset(-692);
      mpu.setYAccelOffset(270);
      mpu.setZAccelOffset(946);
      mpu.setXGyroOffset(-30);
      mpu.setYGyroOffset(123);
      mpu.setZGyroOffset(37);
#endif

      // make sure it worked (returns 0 if so)
      if (devStatus == 0) {
            // Calibration Time: generate offsets and calibrate our MPU6050
            // mpu.CalibrateAccel(5);
            // mpu.CalibrateGyro(5);
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
      motorPID.SelectType(PID_TYPE);
      motorPID.SetGain(8, 0, 0.7);
      motorPID.SetILimit(100);
}

void Core0b_loop() {
      if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {  //  read a packet from FIFO
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            yaw = ypr[0] * 180 / M_PI - yaw_correction;
      }
      if (do_rorate) {
            int16_t power;
            motorPID.Compute(yaw, 0);
            power = motorPID.Get();
            if (abs(power) > MAX_POWER) power = MAX_POWER * (abs(power) / power);
            motor.Run(power);
      } else {
            motor.Run(0);
      }
      Serial.print(">yaw:");
      Serial.println(yaw);
}

#endif