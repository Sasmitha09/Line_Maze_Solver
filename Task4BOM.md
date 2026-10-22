# Task 4: Bill of Materials & Component Selection

## 1. Bill of Materials (BOM) — Line Maze Solver / Meshmerize Robot

| Component Category | Exact Model & Part No. | Qty | Unit Price (₹) | Total (₹) | Supplier & Link | Key Specifications & Rationale |
| :--- | :--- | :---: | :---: | :---: | :--- | :--- |
| **Microcontroller** | **ESP32 NodeMCU Module** (CP2102) | 1 | ₹450 | ₹450 | [Robu.in](https://robu.in) | Dual-core 240MHz, Wi-Fi/BLE, 3.3V logic. Provides fast processing for maze algorithms and wireless telemetry (Task 8). |
| **Motors** | **N20 Micro Metal Gear Motor** (6V, 600 RPM) | 2 | ₹350 | ₹700 | [Robu.in](https://robu.in) | High RPM + metal gearbox. Extremely compact and lightweight, essential for rapid acceleration and tight turns. |
| **Motor Driver** | **TB6612FNG Dual Motor Driver Breakout** | 1 | ₹300 | ₹300 | [ElectronicsComp](https://www.electronicscomp.com) | 1.2A continuous per channel, MOSFET-based. Low voltage drop and high efficiency compared to legacy drivers. |
| **Line Sensors** | **Custom 5-Channel TCRT5000 IR Array** | 1 | ₹350 | ₹350 | [Robu.in](https://robu.in) | 5 analog-output infrared reflectance sensors spaced optimally for weighted centroid line tracking (Task 1). |
| **Battery** | **7.4V 2S 800mAh 25C LiPo Battery** (JST/XT30) | 1 | ₹850 | ₹850 | [RC Bazaar](https://www.rcbazaar.com) | High discharge rate (25C) prevents voltage sag during high-torque motor bursts, protecting against ESP32 brownouts. |
| **Chassis & Wheels** | **Custom Acrylic/FR4 Chassis + 34mm Wheels + N20 Brackets** | 1 | ₹500 | ₹500 | Local / Robu | Lightweight dual-wheel drive with a rear caster ball, maintaining a low center of gravity. |
| **Power Regulation** | **LM2596 DC-DC Step-Down Buck Converter** | 1 | ₹120 | ₹120 | [Robu.in](https://robu.in) | Steps down 7.4V LiPo voltage to a stable 5V/3.3V for logic components, preventing brownout resets. |
| **Wiring & Connectors** | Jumper Wires (Male-Male/Female), JST Connectors, Header Pins | Set | ₹400 | ₹400 | Local Component Store | Ensures secure power and signal distribution across high-vibration runs. |
| **Shipping & Contingency** | Estimated courier shipping + 10% spare components buffer | - | ₹530 | ₹530 | Various | Covers transit costs and backup sensors/wires for unexpected damage. |
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
