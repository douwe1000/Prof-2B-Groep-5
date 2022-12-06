/*
 * DAC.c
 *
 *  Created on: Nov 30, 2022
 *      Author: douwe
 */

#include "main.h"
#include  <math.h>
#include "settings.h"

extern SPI_HandleTypeDef hspi1;
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds);
/*******************************************************************/
/*   put_DA written by J.F. van der Bent
 *   2 aguments 0 -> select channel A 1 select channel B
 *   load 0-4096
 *   no return
 *   uses DWT_Delay_us function for holdoff LDAC
 *   date 12 sept 2022
 *   version 1.0
 */
void Put_DA(unsigned char channel,unsigned short load)
{
	 unsigned char data[2]={0x30,0x00}; // default unbuffered | Gain 1x |  active mode
	 data[1] = load & 0x00FF;
	 data[0] = data[0]|(load & 0x0F00)>>8;
	 unsigned char *point = data ; // place truncated load over both bytes

	 if (channel)
		 {
		 data[0] = data[0]|1<<7;			// set bit 7 default channel A if set channel B
		 }

	 HAL_GPIO_WritePin(SS1_GPIO_Port, SS1_Pin, GPIO_PIN_RESET);
	 HAL_SPI_Transmit(&hspi1, point, 2, 1000);	// send DA data over SPI
	 HAL_GPIO_WritePin(GPIOB, ldac_Pin, GPIO_PIN_RESET);
	 DWT_Delay_us(10);		// settling time signal + PCB cap
	 HAL_GPIO_WritePin(GPIOB, ldac_Pin, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(SS1_GPIO_Port, SS1_Pin, GPIO_PIN_SET);

}

/*******************************************************************/
/*   put__DA written by J.F. van der Bent
 *   2 aguments  channel A load 0-4095 channel B load 0-4095
 *   no output
 *   uses DWT_Delay_us function for holdoff LDAC
 *   date 12 sept 2022
 *   version 1.0
 */
void Put__DA(unsigned short chan_A,unsigned short chan_B) // both channels
{
	 unsigned char data_A[2]={0x30,0x00}; // default unbuffered | Gain 1x |  active mode
	 unsigned char data_B[2]={0x30,0x00}; // default unbuffered | Gain 1x |  active mode

	 	 data_A[1] = chan_A & 0x00FF;
		 data_A[0] = data_A[0]|(chan_A & 0x0F00)>>8;
		 unsigned char *point_A = data_A ; // place truncated load over both bytes
		 data_B[1] = chan_B & 0x00FF;
		 data_B[0] = data_B[0]|(chan_B & 0x0F00)>>8;
		 unsigned char *point_B = data_B ; // place truncated load over both bytes


		 data_B[0] = data_B[0]|1<<7;			// Select channel B


		 HAL_GPIO_WritePin(SS1_GPIO_Port, SS1_Pin, GPIO_PIN_RESET);
		 HAL_SPI_Transmit(&hspi1, point_A, 2, 1000);	// send DA data over SPI
		 HAL_GPIO_WritePin(SS1_GPIO_Port, SS1_Pin, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(SS1_GPIO_Port, SS1_Pin, GPIO_PIN_RESET);
		 HAL_SPI_Transmit(&hspi1, point_B, 2, 1000);	// send DA data over SPI
		 HAL_GPIO_WritePin(SS1_GPIO_Port, SS1_Pin, GPIO_PIN_SET);


		 HAL_GPIO_WritePin(GPIOB, ldac_Pin, GPIO_PIN_RESET);
		 DWT_Delay_us(10);		// settling time signal + PCB cap
		 HAL_GPIO_WritePin(GPIOB, ldac_Pin, GPIO_PIN_SET);


}
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds)
{
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (HAL_RCC_GetHCLKFreq() / 1000000);
  au32_microseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_microseconds-au32_ticks);
}
uint32_t DWT_Delay_Init(void)
{
    /* Disable TRC */
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
    /* Enable TRC */
    CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

    /* Disable clock cycle counter */
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
    /* Enable  clock cycle counter */
    DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

    /* Reset the clock cycle counter value */
    DWT->CYCCNT = 0;

    /* 3 NO OPERATION instructions */
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");

    /* Check if clock cycle counter has started */
    if(DWT->CYCCNT)
    {
       return 0; /*clock cycle counter started*/
    }
    else
    {
      return 1; /*clock cycle counter not started*/
    }
}
