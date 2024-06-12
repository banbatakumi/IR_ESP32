#ifndef _CORE1A_MAIN_H_
#define _CORE1A_MAIN_H_

#include "setup.h"

#define CORE1A_CONTROL_FREQ 200  // Hz

void Core1a_setup() {
      Serial2.begin(115200, SERIAL_8N1, 16, 17);
}

void Core1a_loop() {
      // UART送信
      const uint8_t send_byte_num = 6;
      uint8_t send_byte[send_byte_num];
      send_byte[0] = 0xFF;
      send_byte[1] = yaw / 2 + 90;
      send_byte[2] = ir.GetDir() / 2 + 90;
      send_byte[3] = ir.GetDis();
      send_byte[4] = can_ally_get_pass << 4 | is_ally_catch_ball << 3 | is_ally_defense << 2 | is_ally_moving << 1 | is_connect;
      send_byte[5] = 0xAA;
      Serial2.write(send_byte, send_byte_num);
      // Serial2.write(pc_command);

      // UART受信
      uint8_t recv_data = Serial2.read();

      do_rorate = (recv_data) & 1;
      on_ir_led = (recv_data >> 1) & 1;
      is_moving = (recv_data >> 2) & 1;
      is_defense = (recv_data >> 3) & 1;
      is_catch_ball = (recv_data >> 4) & 1;
      can_get_pass = (recv_data >> 5) & 1;
      do_yaw_correction = (recv_data >> 6) & 1;
}

#endif