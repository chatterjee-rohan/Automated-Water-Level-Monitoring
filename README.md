# Automated Water Level Monitoring and Control System with OLED Display

## Overview
This project automates water level monitoring and control using an Arduino Uno R3. It leverages an ultrasonic sensor to measure water levels, an OLED display for real-time feedback, and a relay-controlled water pump to manage water flow efficiently. The system is designed to prevent overflow and shortages, making it ideal for residential and industrial use.

## Features
- **Real-time Monitoring:** Displays water level on an OLED screen.
- **Automated Control:** Relay automatically manages the water pump based on thresholds.
- **Critical Alerts:** Buzzer alerts for low or high water levels, with a mute option.
- **Manual and Automatic Modes:** User flexibility for water pump control.
- **LED Indicators:** Visual representation of water levels.
- **Scalable and Efficient:** Designed for reliability in diverse environments.

## Components
- Arduino Uno R3
- Ultrasonic Sensor (HC-SR04)
- OLED Display (128x64)
- Single Channel Relay Module
- LEDs (Blue, Green, Yellow, Orange, Red)
- Buzzer
- Toggle and Slide Switches
- Water Pump with pipe
- Resistors and Jumper Wires

## Circuit and Simulation
- **Circuit Diagram:** Detailed connections provided in the project documentation.
- **Simulation:** [Wokwi Simulator Link](https://wokwi.com/projects/403604367575169025)

## Software
The system software is written in C++ using the Arduino IDE. It reads sensor inputs, processes data, and controls components. A debounce technique ensures reliable button functionality.

## How to Use
1. **Setup the Circuit:** Follow the provided circuit connections.
2. **Choose Mode:** Select between manual and automatic modes using the toggle switch.
3. **Monitor Levels:** Observe water levels on the OLED display and LEDs.
4. **Respond to Alerts:** Handle critical alerts with the buzzer mute functionality.

## Future Enhancements
- Wireless monitoring via IoT integration.
- Mobile app for remote control and notifications.

## Contributors
- Mohar Chatterjee
- Rohan Chatterjee

## License
This project is licensed under the MIT License.
