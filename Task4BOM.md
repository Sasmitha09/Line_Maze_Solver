# Task 4: Bill of Materials & Component Selection

## 1. Bill of Materials (BOM) — Line Maze Solver / Meshmerize Robot

| Component Category | Exact Model & Part No. | Qty | Unit Price (₹) | Total (₹) | Product Link | Key Specifications & Rationale |
| :--- | :--- | :---: | :---: | :---: | :--- | :--- |
| **Microcontroller** | **ESP32-WROOM-32 38Pin Dev Board** (CP2102) | 1 | ₹450 | ₹450 | [Robu.in Product Link](https://robu.in/product/esp32-38pin-development-board-wifibluetooth-ultra-low-power-consumption-dual-core/) | Dual-core 240MHz, Wi-Fi/BLE, 3.3V logic. Provides fast processing for maze algorithms and wireless telemetry (Task 8). |
| **Motors** | **N20 6V 600 RPM Micro Metal Gear Motor** | 2 | ₹350 | ₹700 | [Robu.in Product Link](https://robu.in/product/n20-6v-600-rpm-micro-metal-gear-motor/) | High RPM + metal gearbox. Extremely compact and lightweight, essential for rapid acceleration and tight turns. |
| **Motor Driver** | **TB6612FNG Dual Motor Driver Module** | 1 | ₹300 | ₹300 | [ElectronicsComp Product Link](https://www.electronicscomp.com/tb6612fng-dual-motor-driver-module-1a-for-arduino) | 1.2A continuous per channel, MOSFET-based. Low voltage drop and high efficiency compared to legacy drivers. |
| **Line Sensors** | **SmartElex 5-Channel Analog Tracker Sensor** | 1 | ₹350 | ₹350 | [Robu.in Product Link](https://robu.in/product/smartelex-tracker-sensor/) | 5 analog-output infrared reflectance sensors spaced optimally for weighted centroid line tracking (Task 1). |
| **Battery** | **Pro-Range 7.4V 850mAh 30C 2S LiPo Pack** | 1 | ₹850 | ₹850 | [Robu.in Product Link](https://robu.in/product/orange-850mah-2s-30c-60c-lithium-polymer-battery-pack-lipo/) | High discharge rate (30C continuous) prevents voltage sag during high-torque motor bursts, protecting against ESP32 brownouts. |
| **Chassis & Wheels** | **Mini Smart Robot Car Chassis + Wheels + N20 Brackets** | 1 | ₹500 | ₹500 | [Robu.in Product Link](https://robu.in/product/mini-smart-robot-car-chassis-kit/) | Lightweight dual-wheel drive with a rear caster ball, maintaining a low center of gravity. |
| **Power Regulation** | **LM2596 DC-DC Step-Down Buck Converter** | 1 | ₹120 | ₹120 | [Robu.in Product Link](https://robu.in/product/lm2596-step-down-module-power-supply-module/) | Steps down 7.4V LiPo voltage to a stable 5V/3.3V for logic components, preventing brownout resets. |
| **Wiring & Connectors** | Jumper Wires (Male-Female), Connectors, Headers | Set | ₹400 | ₹400 | [Robu.in Product Link](https://robu.in/product/40-pcs-10cm-female-to-female-jumper-wire/) | Ensures secure power and signal distribution across high-vibration runs. |
| **Shipping & Contingency** | Courier shipping + 10% spare components buffer | - | ₹530 | ₹530 | Standard Transit | Covers transit costs and backup sensors/wires for unexpected damage. |
| **TOTAL ESTIMATED COST** | | | | **₹4,200** | | *Well under the ₹10,000 ceiling.* |

---

## 2. Component Justification & Trade-off Analysis

### Why N20 Micro Metal Gear Motors over Standard BO Motors?
* **Selected:** N20 Micro Metal Gear Motors (600 RPM)
* **Alternative:** Standard Plastic BO Motors (1:48 or 1:120 gear ratio)
* **Justification:** Standard BO motors are bulky, heavy, have high backlash, and lack high-RPM options. In a competitive line-maze solver where every gram counts for agility, N20 motors offer an exceptional power-to-weight ratio, higher top speeds (crucial for straightaways), and direct compatibility with precision mounting brackets.

### Why TB6612FNG over L298N Motor Driver?
* **Selected:** TB6612FNG Dual Driver
* **Alternative:** L298N Dual H-Bridge Driver Module
* **Justification:** The L298N is an older bipolar junction transistor (BJT) design that suffers from a massive internal voltage drop (up to 2V), which wastes battery energy as heat and starves the motors of peak voltage. The MOSFET-based TB6612FNG has a very low voltage drop, runs significantly cooler, consumes less idle current, and occupies a fraction of the footprint on a compact PCB.

---

## 3. Power-Block Diagram

```text
  +--------------------------+
  |  7.4V 2S LiPo Battery    |
  +--------------------------+
        |                |
        | (7.4V)         | (7.4V)
        v                v
  +-----------+    +-----------------------+
  |  LM2596   |    |   TB6612FNG Driver    |
  |  Buck     |    |   (VM: Motor Power)   |
  +-----------+    +-----------------------+
        |                |          ^
        | (5V)           | (PWM/    | (Motor
        v                |  Dir)    | Outputs)
  +-------------------+  |          |
  |    ESP32 MCU      |--+          |
  |  (3.3V / 5V Logic)|             |
  +-------------------+             |
        |                           |
        | (Analog/Digital Signals)  |
        v                           v
  +---------------------------------------+
  |         5-Channel IR Sensor Array     |
  +---------------------------------------+
