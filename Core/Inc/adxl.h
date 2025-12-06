/*
 * adxl.h
 *
 *  Created on: Apr 28, 2025
 *      Author: 90506
 */

#ifndef INC_ADXL_H_
#define INC_ADXL_H_


#include "stm32f4xx_hal.h"
#include "main.h"


uint8_t adxl345_init(I2C_HandleTypeDef *hi2c);
void adxl345_read_xyz(int16_t *x, int16_t *y, int16_t *z);
void adxl345_calibrate(void);
void adxl345_get_angles(float *roll, float *pitch);




#endif /* INC_ADXL_H_ */
