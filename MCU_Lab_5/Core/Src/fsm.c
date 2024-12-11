/*
 * fsm.c
 *
 *  Created on: Dec 10, 2024
 *      Author: thaik
 */

#include "fsm.h"

uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

int status = INIT;
uint8_t cmd_flag = INIT;
uint8_t cmd_data[MAX_CMD_SIZE];
uint8_t cmd_data_index = 0;
int ADC_value = 0;

int isCmdEqualToRST(uint8_t str[]){
	if (str[0] == 'R' && str[1] == 'S' && str[2] == 'T') {
		return 1;
	}
	return 0;
}

int isCmdEqualToOK(uint8_t str[]){
	if (str[0] == 'O' && str[1] == 'K')
		return 1;
	return 0;
}
// HELPER FUNCTION END
void command_parser_fsm(ADC_HandleTypeDef* hadc1, UART_HandleTypeDef* huart2) {
	char str[50];
	switch(status) {
		case INIT:
			if(temp == '!')
				status = READING;
			break;
		case READING:
			// start to update cmd_data
			if(temp != '!' && temp != '#') {
				cmd_data[cmd_data_index++] = temp;
				// invalid input
				if(cmd_data_index > 3) {
					status = STOP;
					cmd_data_index = 0;
				}
			}
			// check if end with '#' or start with more '!'
			if (temp == '#' || temp == '!') {
				status = STOP;
				cmd_data_index = 0;
			}
			break;
		case STOP:
			// check if valid command RST
			if (isCmdEqualToRST(cmd_data) == 1){
				cmd_flag = RST;
				ADC_value = HAL_ADC_GetValue(hadc1);
				HAL_UART_Transmit(huart2, (void *)str, sprintf(str, "!ADC=%d#\r\n",ADC_value), 500);
				setTimer1(3000); // delay 3s to wait for command '!OK'
			}
			// check if valid command OK
			else if (isCmdEqualToOK(cmd_data) == 1){
				cmd_flag = OK;
			}
			status = INIT;
			break;
		default:
			break;
	}
}

void uart_communication_fsm(ADC_HandleTypeDef* hadc1, UART_HandleTypeDef* huart2) {
	char str[50];
	switch(cmd_flag){
		case INIT:
			break;
		case RST:
			// print adc value each 1s until command '!OK'
			if(timer1_flag == 1){
				ADC_value = HAL_ADC_GetValue(hadc1);
				HAL_UART_Transmit(huart2, (void *)str, sprintf(str, "!ADC=%d#\r\n",ADC_value), 500);
				setTimer1(1000);
			}
		    break;
		case OK:
			ADC_value = -1;
			cmd_flag = INIT;
			break;
		default:
			break;
	}
}
