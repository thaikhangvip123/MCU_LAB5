/*
 * software_timer.c
 *
 *  Created on: Oct 30, 2024
 *      Author: thaik
 */
#include "software_timer.h"
#define TIMER_CYCLE 10

int timer1_flag = 0;
int timer2_flag = 0;


int timer1_counter = 0;
int timer2_counter = 0;


void setTimer1(int duration) {
  timer1_counter = duration / TIMER_CYCLE;
  timer1_flag = 0;
}
void setTimer2(int duration) {
  timer2_counter = duration / TIMER_CYCLE;
  timer2_flag = 0;
}
void timer_Run(){
	if(timer1_counter > 0){
		timer1_counter--;
		if(timer1_counter == 0) timer1_flag = 1;
	}
	if(timer2_counter > 0){
		timer2_counter--;
		if(timer2_counter == 0) timer2_flag = 1;
	}
}
