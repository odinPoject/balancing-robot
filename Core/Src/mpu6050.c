/*
 * mpu6050.c
 *
 *  Created on: Aug 22, 2026
 *      Author: camdenkeller
 */

#include "mpu6050.h"
#include "i2c.h"

/* -- i2c_MPU6050_write_reg() --
 * Input: Command
 * Return: None
 * Description:
 *
 * Internal function of the MPU6050 driver, that uses I2C to write data to the I2C bus.
 *
 * */
static void i2c_MPU6050_write_reg(uint8_t *data, uint8_t num_of_data){
  // HAL Driver Usage //
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_WRITE_ADDR, data[0], 1, data + 1, 1, HAL_I2C_TIMEOUT);
}

/* -- i2c_MPU6050_read_reg() --
 * Input: Command
 * Return: None
 * Description:
 *
 * Internal function of the MPU6050 driver, that uses I2C to write a command to the I2C bus.
 * Follows Mem_Read format.
 *
 * */
static void i2c_MPU6050_read_reg(uint8_t command, uint8_t *data, uint8_t num_of_data){
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_WRITE_ADDR, command, 1, data, num_of_data, HAL_I2C_TIMEOUT);
}

uint8_t check;
uint8_t reg_trx[2];

/* -- MPU6050_peripheral_config() --
 * Input: None
 * Return: SUCCESS or FAILURE
 * Description:
 *
 * Initializes and configures the MPU6050 sensor.
 *
 * */
uint8_t MPU6050_Init(uint8_t sample_rate){
	i2c_MPU6050_read_reg(WHO_AM_I_REG, &check, 1);
	if (check == MPU6050_DEVICE_ID){
		// power management register 0X6B we should write all 0's to wake the sensor up //
		reg_trx[0] = PWR_MGMT_1_REG;
		reg_trx[1] = PWR_WAKE_UP;
		i2c_MPU6050_write_reg(reg_trx, 2);

		// Set Sample Rate by writing SMPLRT_DIV register //
		reg_trx[0] = SMPLRT_DIV_REG;
		reg_trx[1] = sample_rate;
		i2c_MPU6050_write_reg(reg_trx, 2);

		// Set accelerometer configuration in ACCEL_CONFIG Register //
		reg_trx[0] = ACCEL_CONFIG_REG;
		reg_trx[1] = ACCEL_SCALE_2G;
		i2c_MPU6050_write_reg(reg_trx, 2);

		// Set Gyro configuration in GYRO_CONFIG Register //
		reg_trx[0] = GYRO_CONFIG_REG;
		reg_trx[1] = GYRO_SCALE_250_DEG;
		i2c_MPU6050_write_reg(reg_trx, 2);
		return MPU6050_SUCCESS;
	  }
	else{
		return MPU6050_FAILURE;
	}
}

/* -- MPU6050_Read_Accel() --
 * Input: MPU6050_t *DataStruct
 * Return: None
 * Description:
 *
 * Reads the accelerometer raw values from the MPU6050
 * and converts them into g's.
 * The values are stored in the DataStruct corresponding fields.
 *
 * */
void MPU6050_Read_Accel(MPU6050_t *DataStruct){
  uint8_t Rx_Data[6];

  // Read 6 BYTES of data starting from ACCEL_XOUT_H register //
  i2c_MPU6050_read_reg(ACCEL_XOUT_H_REG, Rx_Data, 6);

  DataStruct->Accel_X_RAW = (int16_t) (Rx_Data[0] << 8 | Rx_Data[1]);
  DataStruct->Accel_Y_RAW = (int16_t) (Rx_Data[2] << 8 | Rx_Data[3]);
  DataStruct->Accel_Z_RAW = (int16_t) (Rx_Data[4] << 8 | Rx_Data[5]);

  // Convert the RAW values into acceleration in 'g' , so we have //
  // to divide according to the Full scale value set in FS_SEL    //
  DataStruct->Ax = DataStruct->Accel_X_RAW / (ACCEL_SENS_2G);
  DataStruct->Ay = DataStruct->Accel_Y_RAW / (ACCEL_SENS_2G);
  DataStruct->Az = DataStruct->Accel_Z_RAW / (ACCEL_SENS_2G);
}

/* -- MPU6050_Read_Gyro() --
 * Input: MPU6050_t *DataStruct
 * Return: None
 * Description:
 *
 * Reads the gyro's raw values from the MPU6050
 * and converts them into degrees.
 * The values are stored in the DataStruct corresponding fields.
 *
 * */
void MPU6050_Read_Gyro(MPU6050_t *DataStruct)
{
  uint8_t Rx_Data[6];

  // Read 6 BYTES of data starting from GYRO_XOUT_H register //
  i2c_MPU6050_read_reg(GYRO_XOUT_H_REG, Rx_Data, 6);

  DataStruct->Gyro_X_RAW = (int16_t) (Rx_Data[0] << 8 | Rx_Data[1]);
  DataStruct->Gyro_Y_RAW = (int16_t) (Rx_Data[2] << 8 | Rx_Data[3]);
  DataStruct->Gyro_Z_RAW = (int16_t) (Rx_Data[4] << 8 | Rx_Data[5]);

  // Convert the RAW values into into dps , so we have          //
  // to divide according to the Full scale value set in FS_SEL  //
  DataStruct->Gx = DataStruct->Gyro_X_RAW / (GYRO_SENS_250_DEG);
  DataStruct->Gy = DataStruct->Gyro_Y_RAW / (GYRO_SENS_250_DEG);
  DataStruct->Gz = DataStruct->Gyro_Z_RAW / (GYRO_SENS_250_DEG);
}

