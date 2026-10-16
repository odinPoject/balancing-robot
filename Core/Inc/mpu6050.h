/*
 * mpu6050.h
 *
 *  Created on: Aug 21, 2026
 *      Author: camdenkeller
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

/* MPU-6050 register addresses */
typedef enum
{
    PWR_MGMT_1_REG     = 0x6B,
    PWR_MGMT_2_REG     = 0x6C,
    SMPLRT_DIV_REG     = 0x19,
    ACCEL_CONFIG_REG   = 0x1C,
    ACCEL_XOUT_H_REG   = 0x3B,
    TEMP_OUT_H_REG     = 0x41,
    GYRO_CONFIG_REG    = 0x1B,
    GYRO_XOUT_H_REG    = 0x43,
    WHO_AM_I_REG       = 0x75
} mpu6050_reg_add;

/* MPU-6050 measurement data */
typedef struct
{
    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;

    double Ax;
    double Ay;
    double Az;

    int16_t Gyro_X_RAW;
    int16_t Gyro_Y_RAW;
    int16_t Gyro_Z_RAW;

    double Gx;
    double Gy;
    double Gz;

    float Temperature;

} MPU6050_t;

/* MPU-6050 I2C address */
#define MPU6050_ADDR        0x68
#define MPU6050_WRITE_ADDR 	0xD0
#define MPU6050_READ_ADDR 	0xD1
#define MPU6050_DEVICE_ID 	0x68
#define HAL_I2C_TIMEOUT 	100

/* Accelerometer configuration */
#define ACCEL_SENS_2G      16384.0f
#define ACCEL_SCALE_2G     0x00

/* Gyroscope configuration */
#define GYRO_SENS_250_DEG		131.0f
#define GYRO_SCALE_250_DEG		0x00

/* Power management */
#define PWR_WAKE_UP        0x01
#define ENABLE_SLEEP       0x40
#define ENABLE_CYCLE       0x20
#define TEMP_DIS           0x08

/*WHO_AM_I*/
#define MPU6050_SUCCESS  	0
#define MPU6050_FAILURE  	1

/* Driver functions */
uint8_t MPU6050_Init(uint8_t sample_rate);
void MPU6050_Read_Accel(MPU6050_t *DataStruct);
void MPU6050_Read_Gyro(MPU6050_t *DataStruct);


#endif /* INC_MPU6050_H_ */
