![Static Badge](https://img.shields.io/badge/Unic_Lab-green)
![Static Badge](https://img.shields.io/badge/STM32-red)
![GitHub last commit (by committer)](https://img.shields.io/github/last-commit/Vernicovskiy/STM32_TIM)
![GitHub Repo stars](https://img.shields.io/github/stars/Vernicovskiy/STM32_TIM)
![GitHub watchers](https://img.shields.io/github/watchers/Vernicovskiy/STM32_TIM)
![GitHub top language](https://img.shields.io/github/languages/top/Vernicovskiy/STM32_TIM)







# Обзор таймера общего назначения TIM10 в режиме обычного счета
 * NUCLEO-F401RE
 * STM32F401RET6U
 * ARM Cortex M4
 * CMSIS
 * STM32 CubeIDE v1.13.2

>Программа реализует мерцание светодиода LD2 , подключенным анодом к ножке микроконтроллера PA5 ,  с периодом 2 секунды с использованием задержки по прерыванию от 16-битного таймера общего назначения.

---
<details>
  <summary>Принцип работы режима обычного счета таймера общего назначения</summary>
  
  ![Diagram of System Timer (SysTick)](image.png)
</details>   

---
Для инициализации таймера общего назначения использована функция `tim10_init()` Ключевым является настройка *источника тактирования*, *разрешение прерываний*,*настройка преддлеителя* и *включение таймера путем загрузки значения в регистр перезагрузки*.
```C
void tim10_init(){
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN; 
		NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); 
		TIM10->PSC = 16000;
		TIM10->ARR	= 999;
	}   
```

#### Добавлен обработчик для таймера
```C
__IO uint32_t Tim10_Count = 0;

void TIM1_UP_TIM10_IRQHandler(void){

		if(READ_BIT(TIM10->SR, TIM_SR_UIF)){

			TIM10->SR &= ~TIM_SR_UIF;

			switch(Tim10_Count)
			{
			case 0: LED_ON(); break;
			case 1: LED_OFF(); break;
			}

			Tim10_Count++;

			if(Tim10_Count>1) Tim10_Count = 0;
    }


		}
```
Обработчик вызывается каждую секунду. Чтобы помигать светодиодом с определенной частотой добавлена переменная-счетчик `Tim10_Count`. Когда обработчик прерывания вызывается первый раз значение переменной-счетчика равно нулю и светодиод зажигается и увеличивается значение счетчика, когда обработчик прерывания вызывается второй раз переменная счетчика уже равно единице и светодиод гаснет, затем счетчик опять сбрасывается в ноль и цикл повторяется. 




