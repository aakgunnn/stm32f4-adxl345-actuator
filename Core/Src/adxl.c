/*
 * adxl.c
 *
 *  Created on: Apr 28, 2025
 *      Author: 90506
 */

#include "adxl.h"
#include <math.h>


// ADXL345 I2C 8-bit adresi (0x53 << 1)
#define ADXL345_I2C_ADDR (0x53 << 1)


static I2C_HandleTypeDef *adxl_i2c_handle = NULL;

static int16_t x_offset = 0;
static int16_t y_offset = 0;
static int16_t z_offset = 0;


uint8_t adxl345_init(I2C_HandleTypeDef *hi2c)
{
    uint8_t data;
    uint8_t id;

    adxl_i2c_handle = hi2c;

    // Device ID kontrolü
    if (HAL_I2C_Mem_Read(adxl_i2c_handle, ADXL345_I2C_ADDR, 0x00, 1, &id, 1, 100) != HAL_OK)
        return 0;

    if (id != 0xE5)
        return 0; // Sensör bulunamadı

    // Ölçüm modunu aç (Power_CTL register = 0x08)
    data = 0x08;
    HAL_I2C_Mem_Write(adxl_i2c_handle, ADXL345_I2C_ADDR, 0x2D, 1, &data, 1, 100);

    // Full Resolution + ±2g ayarı (Data_Format register = 0x08)
    data = 0x08;
    HAL_I2C_Mem_Write(adxl_i2c_handle, ADXL345_I2C_ADDR, 0x31, 1, &data, 1, 100);

    // Data Rate = 100Hz (BW_RATE register = 0x0A)
    data = 0x0A;
    HAL_I2C_Mem_Write(adxl_i2c_handle, ADXL345_I2C_ADDR, 0x2C, 1, &data, 1, 100);

    return 1;
}

void adxl345_calibrate(void)
{
    int32_t sum_x = 0, sum_y = 0, sum_z = 0;
    int16_t x, y, z;

    for (int i = 0; i < 100; i++)
    {
        adxl345_read_xyz(&x, &y, &z);
        sum_x += x;
        sum_y += y;
        sum_z += z;
        HAL_Delay(10);
    }
    x_offset = sum_x / 100;
    y_offset = sum_y / 100;
    z_offset = (sum_z / 100) - 256;  // 1g yerçekimi düzeltmesi
}


void adxl345_read_xyz(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t buf[6];

    HAL_I2C_Mem_Read(adxl_i2c_handle, ADXL345_I2C_ADDR, 0x32, 1, buf, 6, 100);

    *x = (int16_t)((buf[1] << 8) | buf[0]) - x_offset;
    *y = (int16_t)((buf[3] << 8) | buf[2]) - y_offset;
    *z = (int16_t)((buf[5] << 8) | buf[4]) - z_offset;

}

void adxl345_get_angles(float *roll, float *pitch)
{
    int16_t x, y, z;

    // Kalibre edilmiş ivme verilerini al
    adxl345_read_xyz(&x, &y, &z);

    // float'a çevir (1 LSB ≈ 0.004g @ ±2g full res)
    float xf = x * 0.004f;
    float yf = y * 0.004f;
    float zf = z * 0.004f;

    // Roll: Y-Z düzlemi etrafında eğim
    *roll  = atan2f(yf, zf) * 180.0f / M_PI;

    // Pitch: X-Z düzlemi etrafında eğim
    *pitch = atan2f(-xf, sqrtf(yf * yf + zf * zf)) * 180.0f / M_PI;
}
