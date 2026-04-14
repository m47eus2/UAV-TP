#include "mpu6050.h"
#include <stdint.h>
#include <math.h>

typedef struct{
    I2C_HandleTypeDef *i2c;
    uint8_t addr;
} mpu6050Handler_t;

static mpu6050Handler_t mpu6050Handler;

void mpu6050_writeReg(uint8_t reg, uint8_t data){
    if(mpu6050Handler.i2c == NULL) return;

    HAL_I2C_Mem_Write(mpu6050Handler.i2c, mpu6050Handler.addr, reg, 1, &data, 1, 100);
}

uint8_t mpu6050_readReg(uint8_t reg){
    if(mpu6050Handler.i2c == NULL) return 0;

    uint8_t data;
    HAL_I2C_Mem_Read(mpu6050Handler.i2c, mpu6050Handler.addr, reg, 1, &data, 1, 100);
    return data;
}

void mpu6050_init(I2C_HandleTypeDef *i2cHandler, uint8_t chipAddr){
    mpu6050Handler.i2c = i2cHandler;
    mpu6050Handler.addr = chipAddr;

    uint8_t temp;
    // Reset
    temp = 1 << 7;
    mpu6050_writeReg(PWR_MGMT_1_REG, temp);
    HAL_Delay(100);
    
    // Disable sleep, clock source to PLL from X axis gyro
    mpu6050_writeReg(PWR_MGMT_1_REG, 0x01);

    // DLPF to 20Hz
    mpu6050_writeReg(CONFIG_REG, 0x04);

    // Gyro full range 
    mpu6050_writeReg(GYRO_CONFIG_REG, 0x18);

    // Accel full range 
    mpu6050_writeReg(ACCEL_CONFIG_REG, 0x18);
}

void mpu6050_readScaled(float *gyroScaled, float *accelScaled){
    int16_t gyroRaw[3];
    int16_t accelRaw[3];
    mpu6050_readRaw(gyroRaw, accelRaw);

    for(uint8_t i=0; i<3; i++){
        gyroScaled[i] = gyroRaw[i]/16.4f;
        accelScaled[i] = accelRaw[i]/2048.0f;
    }
}

uint8_t mpu6050_whoami(void){
    return mpu6050_readReg(WHOAMI_REG);
}

void mpu6050_readRaw(int16_t *gyro, int16_t *accel){
    if(mpu6050Handler.i2c == NULL) return;
    
    uint8_t buffer[14];
    HAL_I2C_Mem_Read(mpu6050Handler.i2c, mpu6050Handler.addr, 0x3B, 1, buffer, 14, 100);
    
    accel[0] = (int16_t)(buffer[0]<<8 | buffer[1]);
    accel[1] = (int16_t)(buffer[2]<<8 | buffer[3]);
    accel[2] = (int16_t)(buffer[4]<<8 | buffer[5]);
    
    gyro[0] = (int16_t)(buffer[8]<<8 | buffer[9]);
    gyro[1] = (int16_t)(buffer[10]<<8 | buffer[11]);
    gyro[2] = (int16_t)(buffer[12]<<8 | buffer[13]);
}

void mpu6050_readRollPitchYaw(float *rollAcc, float *pitchAcc, float *rollGyro, float *pitchGyro, float *yawGyro, float dt){
    float gyro[3];
    float accel[3];
    static float rollFromGyro, pitchFromGyro, yawFromGyro = 0.0f;

    mpu6050_readScaled(gyro, accel);

    // Based on accel
    *rollAcc = atan2(accel[1], accel[2]) * 57.3f;
    *pitchAcc = atan2(-accel[0], accel[2]) * 57.3f;

    // Based on gyro
    rollFromGyro += gyro[0] * dt;
    pitchFromGyro += gyro[1] * dt;
    yawFromGyro += gyro[2] * dt;

    *rollGyro = rollFromGyro;
    *pitchGyro = pitchFromGyro;
    *yawGyro = yawFromGyro;
}