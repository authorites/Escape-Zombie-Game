/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */

uint8_t _clear_screen[10] = "\x1b[2J";
uint8_t set_point[10] = "\x1b[H";
uint8_t greeting_text[100] = "";
uint8_t o[1] = {219};
uint8_t back_ground[100] = "\x1b[48;5;8m";
uint8_t _player[1000] =  "\x1b[38;5;124m"
					   "#### #### ##### ###  ### #### \x1b[1B\x1b[30D"
					   "   # #  # # # # #  #  #  #    \x1b[1B\x1b[30D"
					   "  #  #  # # # # ####  #  ###  \x1b[1B\x1b[30D"
					   " #   #  # # # # #  #  #  #    \x1b[1B\x1b[30D"
					   "#### #### # # # ###  ### #### \x1b[1B\x1b[30D"
					   "\x1b[0m";
uint8_t start[100] = "PRESS ANY KEY TO START";
uint8_t name[100] = "DEVELOPED BY AMNARD LUNGSUN (6130627221) 2020";
uint8_t key_input;
uint8_t clear_line[10] = "\x1b[1K";
int count;
int cha_r;
int cha_c;
int* zom_r;
int* zom_c;
int zom_d;
int m_r;
int m_c;
int*p_r;
int*p_c;
int destination;
int is_game_over;
int scores;
uint8_t score_current[100] = "";
uint8_t game_over[100] = "\333\333\333\333\333\333\333\333\333\333\333\x1b[1B\x1b[11D"
						 "\333\x1b[2;37;40mGAME OVER\x1b[0m\333\x1b[1B\x1b[11D"
						 "\333\333\333\333\333\333\333\333\333\333\333\x1b[1B\x1b[11D";
uint8_t clear_charector[1] = " ";
uint8_t *charector;
uint8_t out[100] = "";
uint8_t test_text[100] = "";
uint8_t check[1] = "#";
uint8_t zombie[1] = "X";
uint8_t player[1] = {219};
int zombie_pos[15] = {21,22,8};
int zom_current;
int zom_max = 5;
uint8_t map[3000] = "#############    #####    ##################################################\x1b[1B\x1b[76D"
					"#    ##    ##    ## ##    ##        ########################################\x1b[1B\x1b[76D"
					"# ## ## ## ##    ## ##    ## ## ### ########################################\x1b[1B\x1b[76D"
					"# ##    ## ##    ## ##    ## ## ### ########################################\x1b[1B\x1b[76D"
					"# ## ##### ######## ######## ## ### ########################################\x1b[1B\x1b[76D"
					"# ##                                ########################################\x1b[1B\x1b[76D"
					"# ##### ## ######## ######## ###### ########################################\x1b[1B\x1b[76D"
					"# ##    ##            ##     ###### ########################################\x1b[1B\x1b[76D"
					"# ## ## ## ## ####### ## ##  ###### ########################################\x1b[1B\x1b[76D"
					"#    ##    ## ##   ##    ##         ########################################\x1b[1B\x1b[76D"
					"# ##### ##### ##      ##### ################################################\x1b[1B\x1b[76D"
					"#    ##    ## ##   ##    ##         ########################################\x1b[1B\x1b[76D"
					"# ## ## ## ## ####### ## ##  ###### ########################################\x1b[1B\x1b[76D"
					"# ##    ##            ##     ###### ########################################\x1b[1B\x1b[76D"
					"# ##### ## ######## ######## ###### ########################################\x1b[1B\x1b[76D"
					"# ##                                ########################################\x1b[1B\x1b[76D"
					"# ## ##### ######## ######## ## ### ########################################\x1b[1B\x1b[76D"
					"# ##    ## ##    ## ##    ## ## ### ########################################\x1b[1B\x1b[76D"
					"# ## ## ## ##    ## ##    ## ## ### ########################################\x1b[1B\x1b[76D"
					"#    ##    ##    ## ##    ##        ########################################\x1b[1B\x1b[76D"
					"#############    #####    ##################################################\x1b[1B\x1b[76D";

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */


void set_cur_to(int row, int col){
	uint8_t buffer[10] = "";
	sprintf(buffer, "\x1b[%d;%dH", row, col);
	HAL_UART_Transmit(&huart2, buffer, sizeof(buffer), HAL_MAX_DELAY);
}

void clear_screen(){
	HAL_UART_Transmit(&huart2, _clear_screen, sizeof(_clear_screen), HAL_MAX_DELAY);
}

void animation_E(int row, int col) {
	for(int i = 4; i > 0; i--) {
		set_cur_to(1+row, i+col);
		HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
		HAL_Delay(150);
	}
	for(int i = 2; i < 6; i++) {
			set_cur_to(i+row, 1+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 2; i < 5; i++) {
			set_cur_to(5+row, i+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 2; i < 5; i++) {
			set_cur_to(3+row, i+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}

}

void animation_C(int row, int col) {
	for(int i = 4; i > 0; i--) {
		set_cur_to(1+row, i+col);
		HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
		HAL_Delay(150);
	}
	for(int i = 2; i < 6; i++) {
			set_cur_to(i+row, 1+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 2; i < 5; i++) {
			set_cur_to(5+row, i+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
}

void animation_A(int row, int col) {
	for(int i = 5; i > 0; i--) {
			set_cur_to(i+row, 1+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 2; i < 5; i++) {
			set_cur_to(1+row, i+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 2; i < 6; i++) {
			set_cur_to(i+row, 4+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 2; i < 5; i++) {
			set_cur_to(3+row, i+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}

}

void animation_P(int row, int col) {
	for(int i = 5; i > 0; i--) {
			set_cur_to(i+row, 1+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 2; i < 5; i++) {
			set_cur_to(1+row, i+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 2; i < 4; i++) {
			set_cur_to(i+row, 4+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 4; i > 1; i--) {
			set_cur_to(3+row, i+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}

}

void animation_S(int row, int col) {
	for(int i = 4; i > 0; i--) {
			set_cur_to(1+row, i+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 2; i < 4; i++) {
			set_cur_to(i+row, 1+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 2; i < 5; i++) {
			set_cur_to(3+row, i+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 4; i < 6; i++) {
			set_cur_to(i+row, 4+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
	for(int i = 4; i > 0; i--) {
			set_cur_to(5+row, i+col);
			HAL_UART_Transmit(&huart2, o, sizeof(o), HAL_MAX_DELAY);
			HAL_Delay(150);
	}
}

void disp_start_scene() {
	clear_screen();
	animation_E(2,25);
	animation_S(2,30);
	animation_C(2,35);
	animation_A(2,40);
	animation_P(2,45);
	animation_E(2,50);
	set_cur_to(9, 26);
	HAL_UART_Transmit(&huart2, _player, sizeof(_player), HAL_MAX_DELAY);

	set_cur_to(18, 30);
	HAL_UART_Transmit(&huart2, start, sizeof(start), HAL_MAX_DELAY);

	set_cur_to(22, 18);
	HAL_UART_Transmit(&huart2, name, sizeof(name), HAL_MAX_DELAY);

	count = 0;
	key_input = 0;
	while(key_input == 0) {
		HAL_Delay(10);
		if(count == 70) {
			set_cur_to(18, 52);
			HAL_UART_Transmit(&huart2, clear_line, sizeof(clear_line), HAL_MAX_DELAY);
		}
		if(count == 90) {
			set_cur_to(18, 30);
			HAL_UART_Transmit(&huart2, start, sizeof(start), HAL_MAX_DELAY);
			count = 0;
		}
		count++;
		set_cur_to(25, 80);
	}
	clear_screen();
}

void disp_map() {
	uint8_t box[1] = {177};
	uint8_t space[10] = "\40";
	set_cur_to(3, 3);
	for(int r = 3;r < 24; r++) {
		for(int c = 3; c < 79; c++) {
				set_cur_to(r, c);
			  	if(map[((r-3)*85)+(c-3)] == 35) {
			  		HAL_UART_Transmit(&huart2, box, sizeof(box), HAL_MAX_DELAY);
			  	} else {
			  		HAL_UART_Transmit(&huart2, space, sizeof(space), HAL_MAX_DELAY);
			  	}
			  	HAL_Delay(2);
		}

	}
	key_input = 0;
	cha_c = 20;
	cha_r = 13;
	set_cur_to(cha_r, cha_c);
	HAL_UART_Transmit(&huart2, player, sizeof(player), HAL_MAX_DELAY);
	score_inintial();
	zom_current = 1;
	zombie_generate();
//	HAL_UART_Transmit(&huart2, map, sizeof(map), HAL_MAX_DELAY);
		sprintf(test_text, "press W(up) A(left) S(down) D(Right)" );
		set_cur_to(2, 3);
		HAL_UART_Transmit(&huart2, test_text, sizeof(test_text), HAL_MAX_DELAY);
}

void move(uint8_t input,int who) {
	if(who == 1) { //zombie
		m_r = *zom_r;
		m_c = *zom_c;
		p_r = zom_r;
		p_c = zom_c;
		charector = zombie;
	} else {
		m_r = cha_r;
		m_c = cha_c;
		p_r = &cha_r;
		p_c = &cha_c;
		charector = player;
	}

	switch (input)
	{
	    case 97:// a
	    	if(!moveable(m_r, m_c-1)) break;
	    	if(collision(m_r, m_c-1,who)) {
	    		is_game_over = 1;
	    		break;
	    	}
	    	set_cur_to(m_r, m_c);
	    	HAL_UART_Transmit(&huart2, clear_charector, sizeof(clear_charector), HAL_MAX_DELAY);
	    	(*p_c)--;
	    	set_cur_to(*p_r, *p_c);
	    	HAL_UART_Transmit(&huart2, charector, sizeof(charector), HAL_MAX_DELAY);
	    	break;

	    case 100:// d
	    	if(!moveable(m_r, m_c+1)) break;
	    	if(collision(m_r, m_c+1,who)) {
	    		is_game_over = 1;
	    		break;
	    	}
	    	set_cur_to(m_r, m_c);
	    	HAL_UART_Transmit(&huart2, clear_charector, sizeof(clear_charector), HAL_MAX_DELAY);
	    	(*p_c)++;
	    	set_cur_to(*p_r, *p_c);
	    	HAL_UART_Transmit(&huart2, charector, sizeof(charector), HAL_MAX_DELAY);
	    	break;

	    case 115:// s
	    	if(!moveable(m_r+1, m_c)) break;
	    	if(collision(m_r+1, m_c,who)) {
	    		is_game_over = 1;
	    		break;
	    	}
	    	set_cur_to(m_r, m_c);
	    	HAL_UART_Transmit(&huart2, clear_charector, sizeof(clear_charector), HAL_MAX_DELAY);
	    	(*p_r)++;
	    	set_cur_to(*p_r, *p_c);
	    	HAL_UART_Transmit(&huart2, charector, sizeof(charector), HAL_MAX_DELAY);
	    	break;

	    case 119:// w
	    	if(!moveable(m_r-1, m_c)) break;
	    	if(collision(m_r-1, m_c,who)) {
	    		is_game_over = 1;
	    		break;
	    	}
	    	set_cur_to(m_r, m_c);
	    	HAL_UART_Transmit(&huart2, clear_charector, sizeof(clear_charector), HAL_MAX_DELAY);
	    	(*p_r)--;
	    	set_cur_to(*p_r, *p_c);
	    	HAL_UART_Transmit(&huart2, charector, sizeof(charector), HAL_MAX_DELAY);
	    	break;

	    default:
	    	break;

	}
	set_cur_to(25, 80);
}

int moveable(int row, int col) {
//	out = &map[((row-3)*85)+(col-3)];
//	sprintf(test_text, "ASCII value = %c decimal = %u" ,map[((row-3)*85)+(col-3)], map[((row-3)*85)+(col-3)]);
//	set_cur_to(1, 1);
//	HAL_UART_Transmit(&huart2, test_text, sizeof(test_text), HAL_MAX_DELAY);
//	HAL_Delay(1000);
//	set_cur_to(1, 30);
//	HAL_UART_Transmit(&huart2, clear_line, sizeof(clear_line), HAL_MAX_DELAY);
	if(map[((row-3)*85)+(col-3)] == 35) return 0;
	return 1;
}

void zombie_generate() {
	int z = zom_current-1;
	if(z%2 == 1) {
		zombie_pos[(z*3)] = 22;
		zombie_pos[(z*3)+1] = 22;
		zombie_pos[(z*3)+2] = 8;
	} else {
		zombie_pos[(z*3)] = 4;
		zombie_pos[(z*3)+1] = 22;
		zombie_pos[(z*3)+2] = 5;
	}
}

void zombie_update() {

	if(zom_current <= scores/60 && zom_current < zom_max) {
		zom_current++;
		zombie_generate();

	}

	for(int z = 0; z < zom_current; z++) {
		zom_r = &zombie_pos[(z*3)];
		zom_c = &zombie_pos[(z*3)+1];
		zom_d = zombie_pos[(z*3)+2];
		switch (zom_d)
		{
			case 8:
				if(moveable(*zom_r-1, *zom_c)) {
					if(moveable(*zom_r, *zom_c-1)||moveable(*zom_r, *zom_c+1)) {
						destination = rand()%3;
						switch (destination)
						{
							case 0:
								move(119,1);
								break;
							case 1:
								if(moveable(*zom_r, *zom_c-1)) {
									move(97,1);
									zombie_pos[(z*3)+2] = 4;
								} else {
									move(100,1);
									zombie_pos[(z*3)+2] = 6;
								}
								break;
							case 2:
								if(moveable(*zom_r, *zom_c+1)) {
									move(100,1);
									zombie_pos[(z*3)+2] = 6;
								} else {
									move(97,1);
									zombie_pos[(z*3)+2] = 4;
								}
								break;
							default:
								break;
						}
					} else {
						move(119,1);
					}

				} else {
					if(moveable(*zom_r, *zom_c-1)||moveable(*zom_r, *zom_c+1)) {
						destination = rand()%2;
						switch (destination)
						{
							case 0:
								if(moveable(*zom_r, *zom_c+1)) {
									move(100,1);
									zombie_pos[(z*3)+2] = 6;
								} else {
									move(97,1);
									zombie_pos[(z*3)+2] = 4;
								}
								break;
							case 1:
								if(moveable(*zom_r, *zom_c-1)) {
									move(97,1);
									zombie_pos[(z*3)+2] = 4;
								} else {
									move(100,1);
									zombie_pos[(z*3)+2] = 6;
								}
								break;
							default:
								break;
						}
					} else {
						move(115,1);
						zombie_pos[(z*3)+2] = 5;
					}
				}
				break;
			case 4:
				if(moveable(*zom_r, *zom_c-1)) {
					if(moveable(*zom_r-1, *zom_c)||moveable(*zom_r+1, *zom_c)) {
						destination = rand()%3;
						switch (destination)
						{
							case 0:
								move(97,1);
								break;
							case 1:
								if(moveable(*zom_r-1, *zom_c)) {
									move(119,1);
									zombie_pos[(z*3)+2] = 8;
								} else {
									move(115,1);
									zombie_pos[(z*3)+2] = 5;
								}
								break;
							case 2:
								if(moveable(*zom_r+1, *zom_c)) {
									move(115,1);
									zombie_pos[(z*3)+2] = 5;
								} else {
									move(119,1);
									zombie_pos[(z*3)+2] = 8;
								}
								break;
							default:
								break;
						}
					} else {
						move(97,1);
					}

				} else {
					if(moveable(*zom_r-1, *zom_c)||moveable(*zom_r+1, *zom_c)) {
						destination = rand()%2;
						switch (destination)
						{
							case 0:
								if(moveable(*zom_r-1, *zom_c)) {
									move(119,1);
									zombie_pos[(z*3)+2] = 8;
								} else {
									move(115,1);
									zombie_pos[(z*3)+2] = 5;
								}
								break;
							case 1:
								if(moveable(*zom_r+1, *zom_c)) {
									move(115,1);
									zombie_pos[(z*3)+2] = 5;
								} else {
									move(119,1);
									zombie_pos[(z*3)+2] = 8;
								}
								break;
							default:
								break;
						}
					} else {
						move(100,1);
						zombie_pos[(z*3)+2] = 6;
					}
				}
				break;
			case 5:
				if(moveable(*zom_r+1, *zom_c)) {
					if(moveable(*zom_r, *zom_c-1)||moveable(*zom_r, *zom_c+1)) {
						destination = rand()%3;
						switch (destination)
						{
							case 0:
								move(115,1);
								break;
							case 1:
								if(moveable(*zom_r, *zom_c-1)) {
									move(97,1);
									zombie_pos[(z*3)+2] = 4;
								} else {
									move(100,1);
									zombie_pos[(z*3)+2] = 6;
								}
								break;
							case 2:
								if(moveable(*zom_r, *zom_c+1)) {
									move(100,1);
									zombie_pos[(z*3)+2] = 6;
								} else {
									move(97,1);
									zombie_pos[(z*3)+2] = 4;
								}
								break;
							default:
								break;
						}
					} else {
						move(115,1);
					}

				} else {
					if(moveable(*zom_r, *zom_c-1)||moveable(*zom_r, *zom_c+1)) {
						destination = rand()%2;
						switch (destination)
						{
							case 0:
								if(moveable(*zom_r, *zom_c+1)) {
									move(100,1);
									zombie_pos[(z*3)+2] = 6;
								} else {
									move(97,1);
									zombie_pos[(z*3)+2] = 4;
								}
								break;
							case 1:
								if(moveable(*zom_r, *zom_c-1)) {
									move(97,1);
									zombie_pos[(z*3)+2] = 4;
								} else {
									move(100,1);
									zombie_pos[(z*3)+2] = 6;
								}
								break;
							default:
								break;
						}
					} else {
						move(119,1);
						zombie_pos[(z*3)+2] = 8;
					}
				}
				break;
			case 6:
				if(moveable(*zom_r, *zom_c+1)) {
					if(moveable(*zom_r-1, *zom_c)||moveable(*zom_r+1, *zom_c)) {
						destination = rand()%3;
						switch (destination)
						{
							case 0:
								move(100,1);
								break;
							case 1:
								if(moveable(*zom_r-1, *zom_c)) {
									move(119,1);
									zombie_pos[(z*3)+2] = 8;
								} else {
									move(115,1);
									zombie_pos[(z*3)+2] = 5;
								}
								break;
							case 2:
								if(moveable(*zom_r+1, *zom_c)) {
									move(115,1);
									zombie_pos[(z*3)+2] = 5;
								} else {
									move(119,1);
									zombie_pos[(z*3)+2] = 8;
								}
								break;
							default:
								break;
						}
					} else {
						move(100,1);
					}

				} else {
					if(moveable(*zom_r-1, *zom_c)||moveable(*zom_r+1, *zom_c)) {
						destination = rand()%2;
						switch (destination)
						{
							case 0:
								if(moveable(*zom_r-1, *zom_c)) {
									move(119,1);
									zombie_pos[(z*3)+2] = 8;
								} else {
									move(115,1);
									zombie_pos[(z*3)+2] = 5;
								}
								break;
							case 1:
								if(moveable(*zom_r+1, *zom_c)) {
									move(115,1);
									zombie_pos[(z*3)+2] = 5;
								} else {
									move(119,1);
									zombie_pos[(z*3)+2] = 8;
								}
								break;
							default:
								break;
						}
					} else {
						move(97,1);
						zombie_pos[(z*3)+2] = 4;
					}
				}
				break;
			default:
				break;
		}

	}
//	 sprintf(out, "random = %d value = %d     " ,destination,rand());
//	 set_cur_to(2, 1);
//	 HAL_UART_Transmit(&huart2, out, sizeof(out), HAL_MAX_DELAY);
//	 HAL_Delay(400);
//	 set_cur_to(2, 30);
//	 HAL_UART_Transmit(&huart2, clear_line, sizeof(clear_line), HAL_MAX_DELAY);
}

int collision(int row, int col,int who) {
	if(who == 1) {
		if(row == cha_r && col == cha_c) return 1;
	} else {
		for(int z = 0; z < zom_current; z++) {
				zom_r = &zombie_pos[(z*3)];
				zom_c = &zombie_pos[(z*3)+1];
				if(row == zom_r && col == zom_c) return 1;
		}
	}
	return 0;
}

void player_update(uint8_t input) {
	move(input,0);
}

void score_inintial() {
	scores = 0;
}

void score_update() {
	sprintf(score_current,"SCORES : %d  ", scores);
	scores++;
	set_cur_to(2, 66);
	HAL_UART_Transmit(&huart2, score_current, sizeof(score_current), HAL_MAX_DELAY);
	set_cur_to(25, 80);
}

void gameOver() {
	key_input = 0;
	set_cur_to(10, 34);
	HAL_UART_Transmit(&huart2, game_over, sizeof(game_over), HAL_MAX_DELAY);
	set_cur_to(25, 80);
	HAL_Delay(3000);
	while(key_input == 0) {
	}
	clear_screen();
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	HAL_UART_Receive_IT(&huart2, &key_input, sizeof(key_input));
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  int frame = 0;
  int frame_rate = 300000;

  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  HAL_Delay(2000);
  srand(time(0));
  HAL_UART_Receive_IT(&huart2, &key_input, sizeof(key_input));

  while (1)
  {
		is_game_over = 0;
		disp_start_scene();
		disp_map();

		while(!is_game_over) {
			if(frame%10000 == 0) {
				player_update(key_input);
				key_input = 0;
			}
			if(frame == frame_rate) {
				zombie_update();
				score_update();
				frame = 0;
			}
			frame++;
		}

		gameOver();


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//			 sprintf(test_text, "r = %d c = %d d = %d r = %d c = %d d = %d            " ,zombie_pos[0],zombie_pos[1], zombie_pos[2],zombie_pos[3],zombie_pos[4], zombie_pos[5]);
		//			 set_cur_to(1, 1);
		//		 	HAL_UART_Transmit(&huart2, test_text, sizeof(test_text), HAL_MAX_DELAY);
		//		 	HAL_Delay(400);
		//		 	set_cur_to(1, 30);
		//		 	HAL_UART_Transmit(&huart2, clear_line, sizeof(clear_line), HAL_MAX_DELAY);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
