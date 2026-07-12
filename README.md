# Motorized Smart Trolley with Bluetooth Follow-Me System

ESP32-based smart trolley that autonomously follows a user via Bluetooth Low Energy (BLE). A companion mobile app broadcasts a BLE UUID; the ESP32 scans and detects it via UUID match. IR sensors handle direction and obstacle sensing — turning, stopping, or halting the trolley if the signal is lost. Prototype built for real-world use cases such as airport luggage handling and retail smart carts.

## Problem Statement

Airports handle a large number of passengers every day, many of whom carry heavy luggage. Traditional trolleys require passengers to manually push and steer them across long distances, which can be inconvenient — especially for elderly passengers, families with children, and individuals carrying multiple bags. In large airports, navigating through terminals while handling luggage can also cause delays, congestion, and passenger fatigue.

This project addresses that gap with an automated luggage cart that follows the passenger with minimal effort, using sensors and control systems to improve passenger comfort and reduce physical strain.

## Components Used

* **Microcontroller:** ESP32 (NodeMCU or equivalent development board)
* **Actuators:** 2x DC Geared Motors
* **Motor Driver:** L298N Dual H-Bridge Motor Driver Module
* **Sensors:** 2x Infrared (IR) Obstacle Avoidance Sensors
* **Power Supply:** 2x 3.7V Li-ion Batteries (for motors) + 5V Power Bank (for ESP32)
* **Chassis:** 3-Wheel Configuration (1x Front Caster Wheel + 2x Rear Drive Wheels)
* **Control:** 1x SPST Toggle Switch (Main Power)

### Pin Mapping Table

| ESP32 Pin | Component | Pin Type | Function |
| :--- | :--- | :--- | :--- |
| **GPIO 32** | L298N IN1 | Output | Left Motor Forward |
| **GPIO 33** | L298N IN2 | Output | Left Motor Backward |
| **GPIO 25** | L298N IN3 | Output | Right Motor Forward |
| **GPIO 26** | L298N IN4 | Output | Right Motor Backward |
| **GPIO 14** | L298N ENA | PWM Output | Left Motor Speed Control |
| **GPIO 27** | L298N ENB | PWM Output | Right Motor Speed Control |
| **GPIO 23** | Left IR Sensor (`IR_LEFT`) | Digital Input | Left Obstacle / Edge Detection |
| **GPIO 22** | Right IR Sensor (`IR_RIGHT`) | Digital Input | Right Obstacle / Edge Detection |
| **GND** | Common Ground | Power | System Ground Reference |

### Prerequisites
* **Arduino IDE v1.8.19** installed.
* **Espressif ESP32 Arduino Core v2.x/3.x** installed via the Boards Manager.
* Node.js (v18+) and npm installed for the mobile application.

### Firmware Installation

1. Clone this repository:
```bash
   git clone https://github.com/Amulya1007/Autocart-ESP32-BLE-Trolley.git
   cd Autocart-ESP32-BLE-Trolley/code
```
2. Launch **Arduino IDE 1.8.19** and open the project file (`.ino`).
3. Install the ESP32 board package (skip if already installed):
   - Go to **File → Preferences**, and add this URL under "Additional Board Manager URLs":
     `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Go to **Tools → Board → Board Manager**, search "ESP32", and install it.
4. Go to **Tools → Board → ESP32 Arduino** and select **NodeMCU-32S** (or your specific ESP32 module).
5. Update the `TARGET_UUID` string in the code to match your mobile app's broadcasting UUID.
6. Connect your ESP32 board via USB.
7. Select the matching serial port under **Tools → Port**.
8. Click the **Upload** arrow icon to compile and flash the code.
> **Note:** No separate library installation is needed for BLE — `BLEDevice.h`, `BLEScan.h`, and `BLEAdvertisedDevice.h` come bundled with the ESP32 board package.

## Working Principle & Firmware Logic

The system utilizes an event-driven state machine based on BLE proximity and real-time sensor fusion:

1. **Target Broadcasting:** The mobile app continuously broadcasts a BLE advertisement payload containing a hardcoded, unique UUID.
2. **Proximity Scanning:** 2. **Proximity Scanning:** The ESP32 continuously scans for BLE advertisement packets. If a packet's service UUID matches the target UUID, `deviceFound` is set to `true` and the timestamp is recorded. If no matching packet is seen for more than 2 seconds, the trolley performs a safety stop.
3. **IR-Based Steering & Obstacle Avoidance:**
   * **No Obstacles Detected:** Trolley moves straight toward the user.
   * **Left IR Active:** Trolley spins/curves to the left to realign.
   * **Right IR Active:** Trolley spins/curves to the right to realign.
   * **Both IRs Active:** Immediate emergency brake (Obstacle directly ahead).

*Note: Differential steering is used to change direction. Turning is achieved by reducing the PWM duty cycle on one wheel relative to the other for smooth, curved adjustments.*

## App (Mobile Interface)

**Frontend:**
- React Native (Expo Framework) — cross-platform mobile app
- TypeScript (.tsx) — structured, type-safe code
- Expo Router — screen navigation
- Custom UI components

**Development tools:** Node.js & npm, Babel, ESLint, VS Code

## Applications
- **Airport/hotel luggage trolleys** — follows the passenger hands-free through terminals or lobbies, reducing the physical effort of pushing/steering
-**Hospital/warehouse carrier carts** — follows staff carrying equipment or supplies between fixed points, avoiding the need to manually pull a cart
-**Elderly or mobility-assistance carts** — trails a person at walking pace, useful for carrying bags/groceries/medical equipment without requiring them to pull weight
- **Education/research** — demonstrates core embedded systems concepts (BLE proximity sensing, real-time sensor fusion, motor control) useful for coursework or as a base for more advanced student projects

## Future Enhancements

- IoT/cloud integration for real-time data sharing
- AI-powered recommendations and voice-assisted guidance
- RFID or image-recognition-based automatic billing
- Extended-range BLE with fleet management for multiple trolleys
- Indoor positioning using BLE beacons for aisle/terminal-level navigation

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

*Built as a first-year engineering project at VIT Chennai.*
