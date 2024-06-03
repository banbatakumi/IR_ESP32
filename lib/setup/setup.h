#ifndef _SETUP_H_
#define _SETUP_H_

#include <speedFunctionsESP32.h>

#include "BluetoothSerial.h"
#include "ir.h"
#include "motor.h"
#include "simplify_deg.h"

#define DEBUG

#define CORE0A_CONTROL_FREQ 100  // Hz
#define CORE0B_CONTROL_FREQ 100  // Hz

// ピン定義
const uint8_t led_pin = 23;
const uint8_t ir_led_pin[4] = {4, 5, 18, 19};
const uint8_t motor_a_pin = 32;
const uint8_t motor_b_pin = 33;

int16_t yaw, pitch, roll;

String MACadd = "EC:94:CB:7E:05:42";
uint8_t address[6] = {0xEC, 0x94, 0xCB, 0x7E, 0x05, 0x42};
BluetoothSerial SerialBT;

Ir ir(12, 14, 27, 26, 25, 2, 15, 13);
Motor motor(32, 33);

#endif