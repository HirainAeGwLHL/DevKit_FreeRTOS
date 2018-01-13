/*
 * bsp.h
 *
 *  Created on: Jan 13, 2018
 *      Author: li
 */

#ifndef BSP_BSP_H_
#define BSP_BSP_H_

#include "derivative.h"

extern void Led_On(int led);
extern void Led_Off(int led);
extern void Led_Toggle(int led);

extern void BSP_Init(void);

#endif /* BSP_BSP_H_ */
