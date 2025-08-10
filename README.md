# esp32-servo
This project uses the ESP-IDF framework to control an SG90 servo motor based on analog input from a flex sensor.

## Overview
This ESP32 project reads the analog value from a flex sensor and converts it to a PWM signal that drives a servo motor. The servo position smoothly varies according to the sensor’s bend.

## Hardware
### Servo
- Model: SG90 microservo
- Signal pin connected to GPIO 25 on the ESP32

### Flex Sensor
- Connected to GPIO 35 (ADC1 Channel 7) for analog input
- Forms a voltage divider with a fixed 10 kΩ resistor to convert changes in resistance to changes in voltage

## Software
- ADC1 configured for 12-bit reading (values 0–4095)
- MCPWM used to generate 50 Hz PWM signal for servo control
- Servo pulse width varies between 1000 μs (0°) and 2000 μs (180°) depending on flex sensor reading
