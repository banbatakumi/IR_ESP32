#include <Arduino.h>

#include "mpu6050_setup.h"
#include "setup.h"

void Core0a(void *args) {
      const uint32_t control_period = (1.0f) / CORE0A_CONTROL_FREQ * 1000000;  // Hz→μsに変換

      pinMode(led_pin, OUTPUT);

#ifdef MASTER
      SerialBT.begin("ROBOT_1_ESP32", true);
      bool connected = SerialBT.connect(address);
      if (connected) {
            Serial.println("Connect OK");
      } else {
            while (!SerialBT.connected(10000)) Serial.println("No connect");
      }
      if (SerialBT.disconnect()) Serial.println("Disconnected Succesfully!");

      SerialBT.connect();
#endif
#ifdef SLAVE
      SerialBT.begin("ROBOT_2_ESP32");
#endif
#ifdef DEBUG
      SerialBT.begin("Crescent_ESP32");
#endif

      char databox;
      while (1) {
            uint64_t current_time = micros();  // 現在経過時間を取得

#ifdef MASTER
            if (SerialBT.connected(10000)) {
                  if (a == 0) SerialBT.write('L');
                  if (a == 1) SerialBT.write('T');
                  a = 1 - a;

                  if (SerialBT.available()) {
                        databox = SerialBT.read();
                        Serial.println(databox);

                        if (databox == 'L') {
                              digitalWrite(led_pin, LOW);
                        }
                        if (databox == 'T') {
                              digitalWrite(led_pin, HIGH);
                        }
                  }
            } else {
                  bool connected = SerialBT.connect(address);
                  if (connected) {
                        Serial.println("Connect OK");
                  } else {
                        while (!SerialBT.connected(10000)) Serial.println("No connect");
                  }
            }
            delay(500);
#endif
#ifdef SLAVE
            if (SerialBT.available()) {
                  databox = SerialBT.read();
                  Serial.println(databox);

                  if (databox == 'L') {
                        digitalWrite(led_pin, LOW);
                        SerialBT.write('L');
                  }
                  if (databox == 'T') {
                        digitalWrite(led_pin, HIGH);
                        SerialBT.write('T');
                  }
            }
#endif
#ifdef DEBUG
            SerialBT.println("HelloWorld ");
#endif

            delay(1);
            int32_t extra_time = control_period - (micros() - current_time);  // 処理にかかった時間と余剰時間の差
            if (extra_time > 0) {
                  delayMicroseconds(extra_time);
            } else {
                  // Serial.println("Core0a processing time exceeded");
            }
      }
}

void Core0b(void *args) {
      const uint32_t control_period = (1.0f) / CORE0B_CONTROL_FREQ * 1000000;  // Hz→μsに変換

      SetupMpu6050();  // mpu6050の初期設定

      while (1) {
            uint64_t current_time = micros();  // 現在経過時間を取得

            if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {  //  read a packet from FIFO
                  mpu.dmpGetQuaternion(&q, fifoBuffer);
                  mpu.dmpGetGravity(&gravity, &q);
                  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
                  yaw = ypr[0] * 180 / M_PI - yaw_correction;
                  pitch = ypr[2] * 180 / M_PI;
                  roll = ypr[1] * 180 / M_PI * -1;

                  // Serial.print("yaw: ");
                  // Serial.print(yaw);
                  // Serial.print(", pitch: ");
                  // Serial.print(pitch);
                  // Serial.print(", roll: ");
                  // Serial.println(roll);
            }

            delay(1);
            int32_t extra_time = control_period - (micros() - current_time);  // 処理にかかった時間と余剰時間の差
            if (extra_time > 0) {
                  delayMicroseconds(extra_time);
            } else {
                  // Serial.println("Core0b processing time exceeded");
            }
      }
}

void setup() {
      Serial.begin(115200);
      Serial.println("device start");
      Serial2.begin(57600, SERIAL_8N1, 16, 17);

      xTaskCreatePinnedToCore(Core0a, "Core0a", 4096, NULL, 1, NULL, 0);
      xTaskCreatePinnedToCore(Core0b, "Core0b", 4096, NULL, 2, NULL, 0);

      pinMode(ir_led_pin[0], OUTPUT);
      pinMode(ir_led_pin[1], OUTPUT);
      pinMode(ir_led_pin[2], OUTPUT);
      pinMode(ir_led_pin[3], OUTPUT);
}

void loop() {
      ir.Read();
      Serial.println(ir.GetDir());

      // UART送信
      const uint8_t send_byte_num = 4;
      uint8_t send_byte[send_byte_num];
      send_byte[0] = 0xFF;
      send_byte[1] = ir.GetDir() / 2 + 90;
      send_byte[2] = ir.GetDis();
      send_byte[3] = 0xAA;
      Serial2.write(send_byte, send_byte_num);

      // for (uint8_t i = 0; i < 8; i++) {
      //       digitalWrite(ir_led_pin[0], HIGH);
      //       delayMicroseconds(12);
      //       digitalWrite(ir_led_pin[0], LOW);
      //       delayMicroseconds(12);
      // }
      // for (uint8_t i = 0; i < 4; i++) {
      //       digitalWrite(ir_led_pin[1], HIGH);
      //       delayMicroseconds(12);
      //       digitalWrite(ir_led_pin[1], LOW);
      //       delayMicroseconds(12);
      // }
      // for (uint8_t i = 0; i < 4; i++) {
      //       digitalWrite(ir_led_pin[2], HIGH);
      //       delayMicroseconds(12);
      //       digitalWrite(ir_led_pin[2], LOW);
      //       delayMicroseconds(12);
      // }
      // for (uint8_t i = 0; i < 4; i++) {
      //       digitalWrite(ir_led_pin[3], HIGH);
      //       delayMicroseconds(12);
      //       digitalWrite(ir_led_pin[3], LOW);
      //       delayMicroseconds(12);
      // }
      delay(1);
}