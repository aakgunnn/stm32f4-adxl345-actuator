# STM32 Linear Actuator Controller

This project aims to control a linear actuator based on Roll/Pitch angles using an STM32F407 microcontroller and FreeRTOS.

## Current Status

**Work in Progress:** The motor control code (for L298N) is **not added yet**.
Currently, only the sensor readings, LCD interface, and user inputs are implemented.

## Features

- **FreeRTOS:** Manages tasks for display and sensors.
- **ADXL345:** Calculates Roll and Pitch angles.
- **LCD Display:** Shows the current angle and the target angle.
- **Potentiometer:** Used to set the target position manually.

## Hardware

- STM32F4 Discovery Board
- ADXL345 Accelerometer
- 16x2 I2C LCD
- Potentiometer (10k)
- _(Planned)_ L298N Motor Driver & Linear Actuator

## Credits

The I2C LCD driver used in this project is adapted from **SharathN25**:

- [LDC16x02_I2C_Driver_STM32F407](https://github.com/SharathN25/LDC16x02_I2C_Driver_STM32F407)
