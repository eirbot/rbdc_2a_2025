/*
 * Copyright (c) 2025, EIRBOT
 * SPDX-License-Identifier: Apache-2.0
 */

#include "motor_eirbot.h"

//===========les cochonneries d'antony et quentin====================
#include <Arduino.h>

extern HardwareSerial to_moteur1;

static uint8_t ADDR_STP_0 = 0xE0;
static uint8_t ADDR_STP_1 = 0xE1;

int R_roue =0.05;
uint8_t addr_motor_left = 0xE0;
uint8_t addr_motor_right = 0xE1;
uint8_t set_speed_func = 0xF6;
uint8_t read_encoder = 0x30;
uint8_t stop_motor = 0xf7;
float max_speed = 0.5;

//static uint8_t read_encoder = 0x30;
static uint8_t stop = 0xF7;
static uint8_t set_speed = 0xF6;           

//HardwareSerial to_moteur1(1);

uint8_t Check(uint8_t addr, uint8_t func) {
    return (addr + func) & 0xFF;
}

void Set_params_no_data(uint8_t addr, uint8_t func){
    
    to_moteur1.write(addr);
    to_moteur1.write(func);
    to_moteur1.write(Check(addr, func));  
}

void Set_speed(uint8_t addr, uint8_t speed){
    to_moteur1.write(addr);
    to_moteur1.write(set_speed);
    to_moteur1.write(speed);
    to_moteur1.write(Check(addr, set_speed));   
}

uint8_t Percent_to_speed(float speed_percentage){
    if(speed_percentage > 0){return uint8_t(speed_percentage*1.27);}
    else{return int(-1*speed_percentage*1.27) + 128;}
}


uint16_t read_rx(){
  uint8_t reception[10];
  int i =0;
  if(to_moteur1.available()>0){
    while (to_moteur1.available() > 0) { // Vérifie s'il y a des données disponibles
      uint8_t receivedByte = (uint8_t)to_moteur1.read(); // Lit un octet (valeur entre 0 et 255)
      reception[i]= receivedByte;
      i++;
    }
    //tu met ce que tu veux ici, reception[j] correspond au j eme bit de la réponse (si tu veux enlever le e0 tu prends pas reception[0])
    for(int j=0;j<i;j++){
      Serial.write(reception[j]);
    }
  
    uint16_t sortie = 0;
    sortie =((uint16_t)reception[1])<<8 | (uint16_t)reception[2];
    // uint8_t test_fort = sortie>>8;
    // Serial.write(test_fort);
    // Serial.write(sortie);
    return sortie;
  }
  uint16_t sortie_rien;
  return sortie_rien;
}


//===========le joli code d'alban====================================

void init_steppers_communication() {
    // TODO: Init the serial or I2C communication here !
}

void reset_steppers_encoder() {
    // TODO: Only if possible, this function should communicate with
    //  stepper boards to reset encoder values.
}

void start_stepper(motor_side motor) {
    // TODO: start the motor (power up, the stepper can turn)
}

void stop_stepper(motor_side motor) {
    // TODO: stop the motor (power down, the stepper is not moving, and won't turn)
    //  if it can't be done, apply a speed of 0.0ms, to completely stop the motor.
    if (motor == motor_side::motor_left) {
        Set_params_no_data(ADDR_STP_0, stop);
    } else if (motor == motor_side::motor_right) {
        Set_params_no_data(ADDR_STP_1, stop);
    }
}

void set_stepper_speed(motor_side motor, float speed) {
    uint32_t data;
    uint8_t data_speed = 0x0;
    if (speed<0){data_speed | 0x80;}

    if (speed > max_speed){speed = max_speed;}
    float mappedValue = (speed/max_speed) * 120;
    data_speed = (uint8_t)round(mappedValue);  
    
    if (motor == motor_side::motor_left) {
        data += uint32_t(addr_motor_left)<<24;
        data += uint32_t(set_speed_func)<< 16;
        data += uint32_t(data_speed)<<8;
        data += uint32_t((data_speed + set_speed_func + addr_motor_left) & 0xFF);
        }
    else if(motor == motor_side::motor_right){
        data += uint32_t(addr_motor_right)<<24;
        data += uint32_t(set_speed_func)<< 16;
        data += uint32_t(data_speed)<<8;
        data += uint32_t((data_speed + set_speed_func + addr_motor_right) & 0xFF);
        }
    to_moteur1.write(data >> 24); 
    to_moteur1.write(data >> 16);
    to_moteur1.write(data >> 8);
    to_moteur1.write(data);
}

uint16_t get_stepper_encoder(motor_side motor) {
    // TODO: communicate with the stepper board to get the current encoder value !
    //  Value should be between 0x0000 and max 0xFFFF, and not exceed the defined
    //  "sensor_resolution" value in odometry_eirbot!

    float encoder = 0;

    if (motor == motor_side::motor_left) {
        // TODO: get current encoder value for motor left
        //  encoder = ...
        Set_params_no_data(ADDR_STP_0, read_encoder);
        delayMicroseconds(750);
        encoder = read_rx();
        //encoder = 
    } else if (motor == motor_side::motor_right) {
        // TODO: get current encoder value for motor right
        //  encoder = ...
        Set_params_no_data(ADDR_STP_1, read_encoder);
        delayMicroseconds(750);
        encoder = read_rx();
    }

    return encoder;
}


