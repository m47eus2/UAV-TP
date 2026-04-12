#ifndef F405ESC_H
#define F405ESC_H

#include <stdint.h>
#include "tim.h"

void f405esc_init(TIM_HandleTypeDef *handler);
void f405esc_setPwmValues(uint16_t cntCh1, uint16_t cntCh2, uint16_t cntCh3, uint16_t cntCh4);
void f405esc_setChannelPwmValue(uint8_t Ch, uint16_t val);
void f405esc_printPwmValues(void);

#endif