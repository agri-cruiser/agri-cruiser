# Agri-cruiser
AgriCruiser: An Open Source Agriculture Robot for Over-the-Row Navigation

## Overview of the AgriCruiser
**AgriCruiser** is a custom-built wheeled robotic platform featuring a flexible chassis design that can adapt to different crop types and row layouts. The platform is designed for integration with various agricultural subsystems; in this experiment, a precision spraying system was implemented to evaluate its effectiveness in weed management. The robot is operated via joystick input using a RadioMaster controller, which communicates wirelessly with the onboard receiver through the CSRF protocol. In its current iteration, the robot is capable of handling tasks such as navigation and solenoid control.

## Open-source AgriCruiser 
By offering open-source access to the AgriCruiser, we promote innovation through community collaboration and allow other researchers, developers, and farmers to adapt and build on the robot for a variety of use cases. In the MechanicalDesign folder of this repository, we've linked the SolidWorks models, which we shared through GrabCAD Community. Most of the parts we used are off-the-shelf components since we wanted to keep the build as simple as possible. A few parts did need some basic machining, which we handled with tools like a water-jet cutter, bandsaw, or drill press. For parts that do need that level of precision, there are plenty of companies that offer affordable custom fabrication services, including water-jet cutting, laser cutting, and CNC machining. Because all of our parts were already designed in SolidWorks, it was just a matter of exporting files in standard formats like STEP or DXF. In our case, all of the water-jet parts were made by an outside fabrication company, and some were manufactured in-house at UCLA. 

## Circuit for AgriCrusier:
The following diagram shows the wiring between the **ESP32**, **RoboClaw motor controller**, **RadioMaster receiver**, **relay module**, and the **power distribution system**.
<img width="1320" height="740" alt="circuit (1)" src="https://github.com/user-attachments/assets/de50e554-5e90-495d-9da1-14c720e1f77f" />

| ESP32 Pin | Connected To         | Purpose                                      |
|-----------|----------------------|----------------------------------------------|
| VIN       | 5V from Receiver     | ESP32 power supply                           |
| GND       | Common Ground        | Shared ground reference                      |
| RX1 (GPIO 22) | RadioMaster TX   | Receive CRSF data from RC receiver           |
| TX1 (GPIO 23) | RadioMaster RX   | Send data to RC receiver (if needed)         |
| RX2 (GPIO 16) | RoboClaw TX      | Receive data (encoders, telemetry) from RoboClaw |
| TX2 (GPIO 17) | RoboClaw RX      | Send motor commands to RoboClaw              |
| GPIO 5    | Relay Module IN1     | Control Solenoid 1                           |
| GPIO 18   | Relay Module IN2     | Control Solenoid 2                           |
| GPIO 19   | Relay Module IN3     | Control Solenoid 3                           |
| GPIO 21   | Relay Module IN4     | Control Solenoid 4                           |
| GPIO 2    | Status LED (optional)| Debugging / system heartbeat indicator       |
