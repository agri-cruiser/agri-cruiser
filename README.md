# Agri-cruiser
AgriCruiser: An Open Source Agriculture Robot for Over-the-Row Navigation

## Table of Contents
- [Project Overview](#overview-of-the-agricruiser)
- [Open-Source ArgiCruiser](#open-source-agricruiser)
- [Mechanical Design](#mechanical-design)
    - [Chassis](#chassis)
    - [Transmission System](#transmission-system)
    - [Caster Wheel](#caster-wheel) 
- [Circuit Design](#circuit-design)
- [Software Design](#software-design)
    - [Software Setup](#software-setup)


## Overview of the AgriCruiser
**AgriCruiser** is a custom-built wheeled robotic platform featuring a flexible chassis design that can adapt to different crop types and row layouts. The platform is designed for integration with various agricultural subsystems; in this experiment, a precision spraying system was implemented to evaluate its effectiveness in weed management. The robot is operated via joystick input using a RadioMaster controller, which communicates wirelessly with the onboard receiver through the CSRF protocol. In its current iteration, the robot is capable of handling tasks such as navigation and solenoid control.

## Open-source AgriCruiser 
By offering open-source access to the AgriCruiser, we promote innovation through community collaboration, allowing other researchers, developers, and farmers to adapt and build upon the robot for a variety of use cases. In the MechanicalDesign folder of this repository, we've linked the SolidWorks models that we shared through the GrabCAD Community. Most of the parts we used are off-the-shelf components since we wanted to keep the build as simple as possible. A few parts required some basic machining, which we handled using tools such as a water-jet cutter, bandsaw, or drill press. For parts that require that level of precision, there are numerous companies offering affordable custom fabrication services, including water-jet cutting, laser cutting, and CNC machining. Since all our parts were already designed in SolidWorks, it was simply a matter of exporting files in standard formats, such as STEP or DXF. In our case, all the water-jet parts were fabricated by an outside company, and some were manufactured in-house at UCLA. 

## Mechanical Design
### Chassis 
Begin by constructing the chassis using a T-slot framing system, which enables quick assembly and allows for later adjustments without welding. Use bolts and brackets instead of permanent welds so that each joint can be loosened, repositioned within millimeters, and tightened again using a simple hex key. This reconfigurable setup is essential for adapting the chassis to different crop sizes and field requirements.

Build the frame using aluminum extrusions to achieve both strength and lightweight, making it ideal for agricultural applications. Since only standard cutting and assembly tools are needed, the design remains practical and does not depend on specialized equipment. Utilize widely available components, such as angle brackets, gussets, and hinges, to reduce costs and ensure the platform can be built and modified anywhere.

Finally, design the chassis to allow track width adjustment between 1.42 m and 1.57 m (measured at the driven wheel centers). This feature enables the robot to adapt to various crop layouts in the field without requiring complete disassembly, ensuring the system remains versatile and efficient.
 ![Image Alt](https://github.com/StructuresComp/agri-cruiser/blob/97178a4c4580bc165dab21d5563dfaf69bd2e226/images/CAD_Chassis.png)
### Transmission System
### Caster Wheel 

## Circuit Design
### Circuit Diagram for ArgiCrusier
The following diagram shows the wiring between the **ESP32**, **RoboClaw motor controller**, **RadioMaster receiver**, **relay module**, and the **power distribution system**.
<img width="1320" height="740" alt="circuit (1)" src="https://github.com/user-attachments/assets/de50e554-5e90-495d-9da1-14c720e1f77f" />

### ESP32 Pin Connections
| ESP32 Pin | Connected To         | Purpose                                      |
|-----------|----------------------|----------------------------------------------|
| VIN       | 5V from Receiver     | To power/complete circuit from ESP32 to Reciever                           |
| GND       | Common Ground        | Shared ground reference                      |
| RX1 (GPIO 22) | RadioMaster TX   | Receive CRSF data from RadioMaster receiver           |
| TX1 (GPIO 23) | RadioMaster RX   | Send data to RadioMaster receiver (if needed)         |
| RX2 (GPIO 16) | RoboClaw TX      | Receive data (encoders, telemetry) from RoboClaw |
| TX2 (GPIO 17) | RoboClaw RX      | Send motor commands to RoboClaw              |
| GPIO 5    | Relay Module IN1     | Control Solenoid 1                           |
| GPIO 18   | Relay Module IN2     | Control Solenoid 2                           |
| GPIO 19   | Relay Module IN3     | Control Solenoid 3                           |
| GPIO 21   | Relay Module IN4     | Control Solenoid 4                           |
| GPIO 2    | Status LED (optional)| Debugging / system heartbeat indicator       |

### Power System
- **Main Power:** 24 V LiFePO₄ battery pack.  
- **Power Distribution Board:** Splits 24 V supply to RoboClaw, motors, and relay module.  
- **LM317 + DC-DC Converter:** Regulates 24 V down to 5 V to be used for the ESP32 and receiver. 

### Motor Control
- **RoboClaw 2x60A** controls two MP26 MobilePower gearmotors.  
- Encoders are wired back to RoboClaw for closed-loop speed control.  
- RoboClaw communicates with ESP32 over **UART2**.

### Solenoid Control
- Relay module (4-channel) switches 24 V supply to four solenoid valves.  
- Relays are driven by **GPIO 5, 18, 19, 21** on the ESP32.  
- Each solenoid is powered directly from the 24 V battery rail through the relay. 

### Radio Receiver
- **RadioMaster Receiver** provides CRSF protocol data over UART1 (pins 22/23).  
- Channels are parsed by ESP32 firmware and mapped to throttle, steering, and solenoid switches.  

## Software Design
This section explains the software architecture that runs on the ESP32.  
It covers how to set up the development environment, install required libraries,  
and provides an overview of the main software components used in the system.  

---

### Software Setup

To get started, follow these steps:

1. **Install Arduino IDE (v2.x or later)**  
   - Download from [Arduino official site](https://www.arduino.cc/en/software).  

2. **Install ESP32 Board Support**  
   - Open Arduino IDE.  
   - Go to **File → Preferences → Additional Board Manager URLs**.  
   - Add:  
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```  
   - Then go to **Tools → Board → Board Manager**, search for **ESP32**, find **esp32** by Epxressif Systems, find version 2.09 and install.  

3. **Install Required Libraries**  
   - **RoboClaw**:
      - Download the library ZIP from the official GitHub repo: https://github.com/basicmicro/roboclaw_arduino_library
      - In Arduino IDE, go to **Sketch → Include Library → Add .ZIP Library…**. 
      - Select the downloaded ZIP file.
   - **Other Libraries**
      - `WiFi.h`, `esp_now.h`, `HardwareSerial.h` are built into the ESP32 board

4. **Open the Code**  
   - Load `MainRobotCode.ino` into Arduino IDE.  

5. **Select the Board & Port**  
   - Board: **DOIT ESP32 DEVKIT V1** (or your specific ESP32 board).  
   - Port: Select the correct COM port for your ESP32.  

6. **Upload the Firmware**  
   - Click the **Upload** button in Arduino IDE.  
   - Open **Serial Monitor** (115200 baud) to verify operation.  

---
### 🧩 Software Components

The firmware is structured into key modules that handle communication, motor control, and actuator management:  

- **ESP-NOW Communication (Optional)**  
  - Enables wireless peer-to-peer data transfer between multiple ESP32 boards.  
  - Used for sending motor commands and receiving telemetry.  

- **CRSF Receiver Parsing**  
  - Reads Crossfire (CRSF) packets from the RadioMaster receiver via UART1.  
  - Extracts channel values and parses them (throttle, steering, switches) to control robot functions.  

- **RoboClaw Motor Driver Interface**  
  - Handles bidirectional communication with the RoboClaw motor controller via UART2.  
  - Commands include speed control, encoder feedback, and battery voltage monitoring to all for movement as well as diagnostics.

- **Relay Control (Solenoids)**  
  - Four GPIO pins control a 4-channel relay board based on the data recieved by controller.  
  - Each relay switches 24 V power to a solenoid valve for actuation.  

- **Telemetry Feedback**  
  - Encoders, motor speeds, and battery voltage are read from RoboClaw.  
  - Data is sent back over ESP-NOW for monitoring and debugging.  

---
With this setup, the ESP32 can:  
- Receive control inputs from an RadioMaster transmitter via CRSF.  
- Drive motors with closed-loop feedback through RoboClaw.  
- Trigger solenoids using relays.  
- Send telemetry data back over ESP-NOW.  