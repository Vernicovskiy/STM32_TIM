#include "main.h"

#define LED_ON() GPIOA->BSRR = GPIO_BSRR_BS5;
#define LED_OFF() GPIOA->BSRR = GPIO_BSRR_BR5;
#define TIM_EnableIT_UPDATE(TIMx) SET_BIT(TIMx->DIER, TIM_DIER_UIE)
#define TIM_EnableCounter(TIMx) SET_BIT(TIMx->CR1, TIM_CR1_CEN)


	__IO uint32_t Tim10_Count = 0;



	void led_init(){

		RCC->AHB1ENR	|= RCC_AHB1ENR_GPIOAEN; //RCC on for GPIO C
	    GPIOA->MODER &= ~0x00000C00; /* clear pin mode */
	    GPIOA->MODER |= GPIO_MODER_MODER5_0; /* set pin to output mode */

	}

	void tim10_init(){
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN; // тактирование
		NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); // глоб прер
		TIM10->PSC = 16000;// преддел
		//CLEAR_REG(TIM10->ARR);
		TIM10->ARR	= 999;
	}


	void TIM1_UP_TIM10_IRQHandler(void){

		if(READ_BIT(TIM10->SR, TIM_SR_UIF)){

			TIM10->SR &= ~TIM_SR_UIF;
			switch(Tim10_Count)
			{
			case 0: LED_ON(); break;
			case 1: LED_OFF(); break;
			}

			Tim10_Count++;
			if(Tim10_Count>1)
			{
				Tim10_Count = 0;
			}

		}


	}

int main(void){

	led_init();
	tim10_init();
	TIM_EnableIT_UPDATE(TIM10);
	TIM_EnableCounter(TIM10);


	while(1){

	}
}
