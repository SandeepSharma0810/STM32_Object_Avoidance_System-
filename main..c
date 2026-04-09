#include "stm32f1xx_hal.h"

// IR Sensor Pins
#define IR_LEFT   HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)
#define IR_MID    HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)
#define IR_RIGHT  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)

// Motor Control Pins (L293D)
#define IN1_PIN GPIO_PIN_3
#define IN2_PIN GPIO_PIN_4
#define IN3_PIN GPIO_PIN_5
#define IN4_PIN GPIO_PIN_6
#define MOTOR_PORT GPIOA

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

// Motor Functions
void move_forward() {
    HAL_GPIO_WritePin(MOTOR_PORT, IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN4_PIN, GPIO_PIN_RESET);
}

void move_backward() {
    HAL_GPIO_WritePin(MOTOR_PORT, IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN4_PIN, GPIO_PIN_SET);
}

void turn_left() {
    HAL_GPIO_WritePin(MOTOR_PORT, IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN4_PIN, GPIO_PIN_RESET);
}

void turn_right() {
    HAL_GPIO_WritePin(MOTOR_PORT, IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_PORT, IN4_PIN, GPIO_PIN_SET);
}

void stop() {
    HAL_GPIO_WritePin(MOTOR_PORT, IN1_PIN|IN2_PIN|IN3_PIN|IN4_PIN, GPIO_PIN_RESET);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    while (1)
    {
        if (IR_MID == GPIO_PIN_RESET)  // Obstacle in front
        {
            stop();
            HAL_Delay(1000);
            move_backward();
            HAL_Delay(1000);

            if (IR_LEFT == GPIO_PIN_SET)
            {
                turn_left();
            }
            else
            {
                turn_right();
            }
            HAL_Delay(1000);
        }
        else if (IR_LEFT == GPIO_PIN_RESET)
        {
            turn_right();
            HAL_Delay(500);
        }
        else if (IR_RIGHT == GPIO_PIN_RESET)
        {
            turn_left();
            HAL_Delay(500);
        }
        else
        {
            move_forward();
        }
    }
}

// GPIO Initialization
static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // IR Sensor Pins
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Motor Pins
    GPIO_InitStruct.Pin = IN1_PIN | IN2_PIN | IN3_PIN | IN4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(MOTOR_PORT, &GPIO_InitStruct);
}

// Dummy Clock Config (can be auto-generated in CubeIDE)
void SystemClock_Config(void)
{
    // Keep default or generate using CubeMX
}