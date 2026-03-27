#include "f405esc.h"

static TIM_HandleTypeDef *escTimHandler;

void f405esc_init(TIM_HandleTypeDef *handler){
    escTimHandler = handler;

    HAL_TIM_Base_Start_IT(escTimHandler);
    HAL_TIM_PWM_Start(escTimHandler, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(escTimHandler, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(escTimHandler, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(escTimHandler, TIM_CHANNEL_4);
    f405esc_setPwmValues(1000, 1000, 1000, 1000);
}

void f405esc_setPwmValues(uint16_t cntCh1, uint16_t cntCh2, uint16_t cntCh3, uint16_t cntCh4){
    if(escTimHandler == NULL)
        return;
    __HAL_TIM_SET_COMPARE(escTimHandler, TIM_CHANNEL_1, cntCh1);
    __HAL_TIM_SET_COMPARE(escTimHandler, TIM_CHANNEL_2, cntCh2);
    __HAL_TIM_SET_COMPARE(escTimHandler, TIM_CHANNEL_3, cntCh3);
    __HAL_TIM_SET_COMPARE(escTimHandler, TIM_CHANNEL_4, cntCh4);
}