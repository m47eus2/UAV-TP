#include "f405esc.h"
#include "stm32l4xx_hal_tim.h"
#include <stdio.h>

typedef struct{
    TIM_HandleTypeDef *escTimHandler;
    uint16_t values[4];
} f405escHandler_t;

static f405escHandler_t f405escHandler;

void f405esc_init(TIM_HandleTypeDef *handler){
    f405escHandler.escTimHandler = handler;

    HAL_TIM_Base_Start_IT(f405escHandler.escTimHandler);
    HAL_TIM_PWM_Start(f405escHandler.escTimHandler, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(f405escHandler.escTimHandler, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(f405escHandler.escTimHandler, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(f405escHandler.escTimHandler, TIM_CHANNEL_4);
    f405esc_setPwmValues(1000, 1000, 1000, 1000);
}

void f405esc_setPwmValues(uint16_t cntCh1, uint16_t cntCh2, uint16_t cntCh3, uint16_t cntCh4){
    if(f405escHandler.escTimHandler == NULL) return;

    if(cntCh1 < 1000) cntCh1=1000;
    if(cntCh2 < 1000) cntCh2=1000;
    if(cntCh3 < 1000) cntCh3=1000;
    if(cntCh4 < 1000) cntCh4=1000;

    __HAL_TIM_SET_COMPARE(f405escHandler.escTimHandler, TIM_CHANNEL_1, cntCh1);
    __HAL_TIM_SET_COMPARE(f405escHandler.escTimHandler, TIM_CHANNEL_2, cntCh2);
    __HAL_TIM_SET_COMPARE(f405escHandler.escTimHandler, TIM_CHANNEL_3, cntCh3);
    __HAL_TIM_SET_COMPARE(f405escHandler.escTimHandler, TIM_CHANNEL_4, cntCh4);

    f405escHandler.values[0] = cntCh1;
    f405escHandler.values[1] = cntCh2;
    f405escHandler.values[2] = cntCh3;
    f405escHandler.values[3] = cntCh4;
}

void f405esc_setChannelPwmValue(uint8_t Ch, uint16_t val){
    if(f405escHandler.escTimHandler == NULL) return;
    if(val < 1000) val = 1000;

    switch(Ch){
        case 0:
            __HAL_TIM_SET_COMPARE(f405escHandler.escTimHandler, TIM_CHANNEL_1, val);
            f405escHandler.values[0] = val;
            break;
        case 1:
            __HAL_TIM_SET_COMPARE(f405escHandler.escTimHandler, TIM_CHANNEL_2, val);
            f405escHandler.values[1] = val;
            break;
        case 2:
            __HAL_TIM_SET_COMPARE(f405escHandler.escTimHandler, TIM_CHANNEL_3, val);
            f405escHandler.values[2] = val;
            break;
        case 3:
            __HAL_TIM_SET_COMPARE(f405escHandler.escTimHandler, TIM_CHANNEL_4, val);
            f405escHandler.values[3] = val;
            break;
    }
}

void f405esc_printPwmValues(void){
    if(f405escHandler.escTimHandler == NULL)
        return; 
    for(uint8_t i = 0; i<4; i++)
        printf(">f405esc_ch%d:%d\n", i+1, f405escHandler.values[i]);
}