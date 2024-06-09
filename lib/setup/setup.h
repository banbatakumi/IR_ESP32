#ifndef _SETUP_H_
#define _SETUP_H_

#include "BluetoothSerial.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "ir.h"
#include "motor.h"
#include "pid.h"
#include "simplify_deg.h"

#define ROBOT_1

// ピン定義
const uint8_t led_pin = 23;
const uint8_t ir_led_pin[4] = {4, 5, 18, 19};

Ir ir(12, 14, 27, 26, 25, 2, 15, 13);
Motor motor(32, 33);
MPU6050 mpu;
PID motorPID;
BluetoothSerial SerialBT;

// MPU control/status vars
uint8_t devStatus;       // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;     // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64];  // FIFO storage buffer

// orientation/motion vars
Quaternion q;         // [w, x, y, z]         quaternion container
VectorFloat gravity;  // [x, y, z]            gravity vector
float ypr[3];         // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

float yaw_correction;  // yaw軸の補正値
int16_t yaw;

bool is_connect;

// メインマイコンから受け取るデータ
bool do_rorate;
bool is_moving;
bool is_defense;
bool is_catch_ball;
bool can_get_pass;
bool on_ir_led;

// blutoothで受け取るデータ
bool is_ally_moving;
bool is_ally_defense;
bool is_ally_catch_ball;
bool can_ally_get_pass;

#endif