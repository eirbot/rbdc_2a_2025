/*
 * Copyright (c) 2025, EIRBOT
 * SPDX-License-Identifier: Apache-2.0
 */

//coucou


#include "robot_eirbot.h"
#include <Arduino.h>

#include <HardwareSerial.h>
HardwareSerial to_moteur1(1);

void setup() {

    // Setup the serial
    Serial.begin(115200);
    to_moteur1.begin(115200,SERIAL_8N1,16,17);
    terminal_printf("[MAIN] Init ...\n");

    // Setup Eirbot RBDC thread
    terminal_printf("Setup EIRBOT robot base control thread for a differential robot\n");
    start_eirbot_rbdc_control_thread();

    // enable robot moves at start
    robot_can_move = true;
    // include common.h to your lidar detection thread, to edit global "robot_can_move" variable

    terminal_printf("[MAIN] Init Done.\n");
}

void loop() {
    
    // Do the square indefinitely
    robot_goto(1.0, 0.0, true);

    delay(5000);
    robot_goto(0.0, 0.0, true);
    //robot_goto(1.0, 1.0);
    //robot_goto(0.0, 1.0);
    //robot_goto(0.0, 0.0);

    // Wait 5 sec and do another square
    delay(5000); // in ms
    
}


//########################### la boucle ouverte de quentin ###############################
/*
#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial to_moteur1(1);


uint8_t addr_motor_left = 0xE0;
uint8_t addr_motor_right = 0xE1;
uint8_t set_speed_func = 0xF6;
uint8_t read_encoder = 0x30;
uint8_t stop_motor = 0xf7;
float max_speed = 1.0;

void set_stepper_speed(bool motor, float speed) {
    uint32_t data;
    uint8_t data_speed = 0x0;
    if (speed<0){data_speed | 0x80;}

    if (speed > max_speed){speed = max_speed;}
    float mappedValue = (speed/max_speed) * 120;
    data_speed = (uint8_t)round(mappedValue);    
    
    if (motor) {
        data += uint32_t(addr_motor_left)<<24;
        data += uint32_t(set_speed_func)<< 16;
        data += uint32_t(data_speed)<<8;
        data += uint32_t((data_speed + set_speed_func + addr_motor_left) & 0xFF);
        }
    else{
        data += uint32_t(addr_motor_right)<<24;
        data += uint32_t(set_speed_func)<< 16;
        data += uint32_t(data_speed)<<8;
        data += uint32_t((data_speed + set_speed_func + addr_motor_right) & 0xFF);
        }
    // Serial.write(data >> 24); 
    // Serial.write(data >> 16);
    // Serial.write(data >> 8);
    // Serial.write(data);
}

void setup() {
  // Initialize Serial Monitor
    Serial.begin(115200);
    to_moteur1.begin(115200,SERIAL_8N1,16,17);
    delay(1000);
}

void loop() {
    set_stepper_speed(1, 10);
}
*/