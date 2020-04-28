#include "delay.h"

extern void xPortSysTickHandler(void);

uint32_t fac_us,fac_ms;

void SysTick_Handler(void){
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED){
		xPortSysTickHandler();
	}
}

void Delay_Init(){
	uint32_t reload;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	fac_us=SystemCoreClock/1000000;
	reload=SystemCoreClock/1000000;
	reload*=1000000/configTICK_RATE_HZ;
	
	fac_ms=1000/configTICK_RATE_HZ;
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;
	SysTick->LOAD=reload;
	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
}

void delay_us(uint32_t nus){
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;
	ticks=nus*fac_us;
	told=SysTick->VAL;
	while(1){
		tnow=SysTick->VAL;
		if(tnow!=told){
			if(tnow<told){
				tcnt+=told-tnow;
			}else{
				tcnt+=reload-tnow+told;
			}
			told=tnow;
			if(tcnt>=ticks){
				break;
			}
		}
	}
}

void delay_ms(uint32_t nms){
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED){
		if(nms>=fac_ms){
			vTaskDelay(nms/fac_ms);
		}
		nms%=fac_ms;
	}
	delay_us((uint32_t)(nms*1000));
}
