#include <Arduino.h>

#include "BluetoothSerial.h"
#include "mpu6050_setup.h"

int16_t yaw, pitch, roll;

BluetoothSerial SerialBT;

void setup() {
      pinMode(23, OUTPUT);
      SetupMpu6050();  // mpu6050の初期設定

      Serial.begin(9600);
      SerialBT.begin("ROBOT_1_ESP32");
}
void loop() {
      if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {  //  read a packet from FIFO
            digitalWrite(23, HIGH);
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            yaw = ypr[0] * 180 / M_PI - yaw_correction;
            pitch = ypr[2] * 180 / M_PI;
            roll = ypr[1] * 180 / M_PI * -1;

            Serial.print(", yaw: ");
            Serial.print(yaw);
            Serial.print(", pitch: ");
            Serial.print(pitch);
            Serial.print(", roll: ");
            Serial.println(roll);
      } else {
            digitalWrite(23, LOW);
      }
}