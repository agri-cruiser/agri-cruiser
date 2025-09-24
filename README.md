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
- [Troubleshooting](#troubleshooting)
   - [Manufacture and Assembly](#manufacture-and-assembly)
   - [Electronics and Hardware](#electronics-and-hardware)


## Overview of the AgriCruiser
**AgriCruiser** is a custom-built wheeled robotic platform featuring a flexible chassis design that can adapt to different crop types and row layouts. The platform is designed for integration with various agricultural subsystems; in this experiment, a precision spraying system was implemented to evaluate its effectiveness in weed management. The robot is operated via joystick input using a RadioMaster controller, which communicates wirelessly with the onboard receiver through the CSRF protocol. In its current iteration, the robot is capable of handling tasks such as navigation and solenoid control.

## Open-source AgriCruiser 
By offering open-source access to the AgriCruiser, we promote innovation through community collaboration, allowing other researchers, developers, and farmers to adapt and build upon the robot for a variety of use cases. In the MechanicalDesign folder of this repository, we've linked the SolidWorks models that we shared through the [GrabCAD Community](https://github.com/StructuresComp/agri-cruiser/tree/cd803cd31bd14a8776e03051d3f2b84bb0032ff6/MechanicalDesign). Most of the parts used are off-the-shelf components, as we aimed to keep the build as simple as possible. A few parts required some basic machining, which we handled using tools such as a water-jet cutter, bandsaw, or drill press. For parts that require this level of precision, numerous companies offer affordable custom fabrication services, including water-jet cutting, laser cutting, and CNC machining. Since all our parts were already designed in SolidWorks, it was simply a matter of exporting files in standard formats, such as STEP or DXF. In our case, all the water-jet parts were fabricated by an outside company, and some were manufactured in-house at UCLA. 

## Mechanical Design
**Note** The BOM is for ONE driven wheel and ONE caster wheel. If you want to build the entire platform, double every component
 
The Bill of Materials BOM, provided in the [BOM](https://github.com/StructuresComp/agri-cruiser/tree/2c079f16a274f6ae52bb82f286a7c02fbf9e2792/BOM) folders, lists the details of each component. For off-the-shelf parts, we’ve included either a McMaster-Carr or Amazon link. While these parts can also be sourced from other suppliers for cheaper prices, the provided links are helpful because they contain the specifications for each component. If you are in the United States, it is often more convenient to order directly through the linked stores. **Note**: You should do a price comparison from different stores before purchasing any parts. 

For water-jet–cut parts, we used SendCutSend, a company based in Reno, Nevada. They accept STL files, which can be exported directly from the provided SolidWorks files. If you prefer to use a different manufacturer, provide them with the CAD files of the components.

For manually machined parts, most are made from basic stock materials, such as square tubing or steel rods. We also provided links to buy these items. In most cases, these need to be cut to size and drilled according to the specifications. All drilling and tapping details are included in the SolidWorks files for reference.

### Chassis 
Begin by constructing the chassis using a T-slot framing system, which enables quick assembly and allows for later adjustments without welding. Use bolts and brackets instead of permanent welds, allowing each joint to be loosened, repositioned within millimeters, and tightened again with a simple hex key. This reconfigurable setup is essential for adapting the chassis to different crop sizes and field requirements.

Build the frame using aluminum extrusions to achieve both strength and lightweight, making it ideal for agricultural applications. Since only standard cutting and assembly tools are needed, the design remains practical and does not depend on specialized equipment. Utilize widely available components, such as angle brackets, gussets, and hinges, to reduce costs and ensure the platform can be built and modified anywhere.

Finally, design the chassis to allow track width adjustment between 1.42 m and 1.57 m (measured at the driven wheel centers). This feature enables the robot to adapt to various crop layouts in the field without requiring complete disassembly, ensuring the system remains versatile and efficient.

 ![Image Alt](https://github.com/StructuresComp/agri-cruiser/blob/97178a4c4580bc165dab21d5563dfaf69bd2e226/images/CAD_Chassis.png)
### Transmission System
In agricultural environments, the transmission system plays a key role in helping the robot handle uneven terrain. Once in the field, it needs to adapt to surfaces like concrete, grass, gravel, or soil. The AgriCruiser uses a two-wheel front-drive system powered by low-RPM, high-torque PMDC brush motors. Specifically, it’s equipped with ElectroCraft’s MP26 motor, paired with a 32:1 gear ratio. This setup delivers continuous torque of up to 31.42 Nm for 15 minutes.

We chose a two-wheel drive instead of a four-wheel drive to keep costs and mechanical complexity down, while still providing enough torque for typical field conditions. Motor selection was guided by torque requirement calculations, which were based on a worst-case scenario: higher operating speeds, a rolling resistance coefficient of up to 0.2, and localized inclines of 10 degrees. For context, soft soil usually has a rolling resistance coefficient between 0.04 and 0.08, so our assumptions were intentionally conservative to ensure the robot performs reliably across a variety of soil types.

**SolidWorks CAD**
 ![Image Alt](https://github.com/StructuresComp/agri-cruiser/blob/6ce7dcf9fd918c649a655e24bfd37614669e59a6/images/CAD_Transmission_System.png)

**Actual System**
 ![Image Alt](https://github.com/StructuresComp/agri-cruiser/blob/6ce7dcf9fd918c649a655e24bfd37614669e59a6/images/IRL_Transmission_System.png)

### Caster Wheel 
To the best of our knowledge, the AgriCruiser introduces a novel configuration in agricultural robotics at this scale by combining a two-wheel front differential drive with two passive rear caster wheels. This setup offers excellent maneuverability while also improving energy efficiency by avoiding the wheel skidding that often occurs in four-wheel drive systems during turns.

In this design, the two powered front wheels generate motion, while the rear caster wheels passively follow. The casters used are 12-inch swivel pneumatic casters with a bolt-on swivel lock. These can be locked or left free to swivel, giving control over their motion and preventing unwanted caster rotations. Locking the rear casters reduced minor heading deviations on uneven terrain during long, straight passes, helping the robot stay aligned within narrow crop rows. On rougher patches or potholes, the locked setting further improved directional stability.

**Actual + CAD**
![Image Alt](https://github.com/StructuresComp/agri-cruiser/blob/2f9c5f66baa831453833d5bae7f16ee296aa54e4/images/Caster_Wheel.png)

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
- **Note:** The most recent version of the RoboClaw did not work in our testing with UART communications, we had to adjust the Firmware Version to 4.1 using [Basic Micro Motion Studio](https://www.basicmicro.com/motor-controller-downloads)

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
      - **Note:** The libary for RoboClaw is also within the GitHub
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

## Troubleshooting 

### Manufacture and Assembly 
1. Since the wheels are connected through a long steel rod, it is common for them to become slightly misaligned, causing the wheels not to sit straight. If you want to set the wheels correctly, there are a few things to pay attention to:

   * The spacer between the rod and the motor needs to be a specific length. While SolidWorks provides a total spacer length, this is primarily theoretical. In practice, if your steel rod is cut slightly longer or shorter than the one shown in SolidWorks, the spacer length will also need to be adjusted accordingly. Since the steel rod was manufactured in-house (we purchased a 6-foot steel rod and cut it down to the desired length), the final length was not exact. That is why you see some shorter spacers (often less than an inch, shown in the blue box) — these account for the difference in rod length. Thus, when rebuilding the transmission system, always check the steel rod length and adjust the spacer accordingly. The advice is to buy extra spacers; the product link is also provided in the BOM. 
   
   * Another spacer detail to watch for is orientation. As you can see in the figure below (or in SolidWorks), one side of the spacer is “longer” than the other. Be sure to place the steel rod on the longer spacer side (red box) outside, while the rod on the shorter spacer side (green box) is on the inside, closer to the wheel.

   * Lastly, make sure the clamp on the wheel axle is fully seated and all components are secured. In one of our experiments, the clamp was not fully inserted (yellow box), resulting in misalignment between the steel rod and wheel, which caused the wheel to accidentally hit the steel rod (as shown in the figure below).

   **Misalignment Example**
   ![Image Alt](https://github.com/StructuresComp/agri-cruiser/blob/ee85f0ba36dc7ce276de6ec9ceb5f835d5c610e5/images/Troubleshooting_Misalignment.png)

2. Make sure to include all the support components — such as the support plate, diagonal brace, and especially the L-brackets at every corner — when connecting the transmission system to the chassis:
   * In our initial build, we left out some of these supports, which made the transmission system less secure and caused the wheels to become misaligned.

3. When manufacturing adapters for the transmission system, make sure to account for Geometric Dimensioning and Tolerancing (GD&T). Adapters are critical because they connect and transfer motion from the motor shaft to the wheel. If tolerances are ignored and the hole size on the adapter is exactly equal to the shaft diameter, the adapter will not fit onto the shaft:
   * In our initial build, I did not include proper tolerances on the adapters. As a result, I had to manually sand down the adapter holes to make them slightly larger, so the motor shaft could fit through. However, if the holes are made too large (either by oversanding or by applying too much tolerance), the adapter becomes loose on the shaft. In that case, the motor’s power and motion will not be fully transferred to the wheel, resulting in power loss.

4. For the timing belt, it connects the pulley on the motor to the pulley on the wheel. We’ve included the McMaster link for the timing belt we used, which worked for our build. However, when rebuilding the transmission and purchasing a new belt, the key detail to pay attention to is the Outer Circle (as noted on McMaster-Carr):
   * The Outer Circle is simply the total length of the timing belt, and this length will vary if the steel rod length is different. As noted in Troubleshooting Item 1, the steel rod is manufactured in-house, so its final length might not exactly match the design. For this reason, we recommend purchasing the belt only after you finish building the transmission system. At that point, measure the distance between the two pulleys directly on the physical build and use that measurement to calculate the exact belt length. Relying only on SolidWorks measurements may not be accurate enough. If the rod ends up longer, you’ll need a slightly larger belt; if shorter, a smaller belt.
   * All other belt specifications — such as pitch, width, and tooth profile — will remain the same. The provided McMaster link contains all of those fixed specifications for reference; only the Outer Circle may change depending on the rod length.

### Electronics and Hardware
1. During the process, the communication between the RoboClaw and the ESP32 was not working. In order to ensure this works, there are a few things that need to be done:
   - Download and open Basic Micro Studio
   - When opened, click on Help -> Download Legacy Motion Studio which will give you a zip file
   - Unzip the file and open up the file that says `Basicmico Motion Studio`
   - Once opened, connect your RoboClaw to the Basicmicro Motion Studio
   - Once connected, you should be able to see the current firmware version and the available firmware version. Update the firmware to the version listed above
