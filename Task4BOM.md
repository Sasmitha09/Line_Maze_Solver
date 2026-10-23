# Task 4: Bill of Materials & Component Selection

## 1. Bill of Materials (BOM): Line Maze Solver / Meshmerize Robot

Prices checked on supplier pages on 22 Oct 2026 (INR, incl. GST). Prices and stock change, so re-check before ordering.
Rows marked **est.** are estimates with no supplier page behind them.

| Component Category | Exact Model & Part No. | Qty | Unit Price (₹) | Total (₹) | Product Link | Key Specifications & Rationale |
| :--- | :--- | :---: | :---: | :---: | :--- | :--- |
| **Microcontroller** | ESP32-WROOM-32 38-Pin Development Board | 1 | 449 | 449 | [Robu](https://robu.in/product/esp32-38pin-development-board-wifibluetooth-ultra-low-power-consumption-dual-core/) | Dual-core 240 MHz, Wi-Fi + BLE, 3.3 V logic, 12-bit ADC. Enough speed for the maze algorithm and wireless diagnostics (Task 7). |
| **Motors** | N20-6V-600 RPM Micro Metal Gear Motor | 2 | 215 | 430 | [Robu](https://robu.in/product/n20-6v-600-rpm-micro-metal-gear-motor/) | 6 V, 600 RPM, metal gearbox, stall current about 0.67 A. Small and light. Run from 7.4 V with PWM capped near 80 % (6/7.4) so the motors stay within their rating. |
| **Motor Driver** | TB6612FNG Dual Motor Driver Module | 1 | 235 | 235 | [ElectronicsComp](https://www.electronicscomp.com/tb6612fng-dual-motor-driver-module-1a-for-arduino) | Motor supply up to 15 V, logic 2.7–5.5 V, 1.2 A continuous per channel (3.2 A peak). MOSFET output stage, so a very low voltage drop. Covers the 0.67 A motor stall current with margin. |
| **Line Sensors** | SmartElex Tracker Sensor, 5-Channel Analog Output | 1 | 279 | 279 | [Robu](https://robu.in/product/smartelex-tracker-sensor/) | 5 IR reflectance sensors with analog outputs, for the weighted centroid (Task 1). Power it from the ESP32 3.3 V pin so outputs never exceed the ADC's 3.3 V limit. |
| **Battery** | Orange 7.4 V 850 mAh 30C 2S LiPo Pack | 1 | 999 | 999 | [Robu](https://robu.in/product/orange-850mah-2s-30c-60c-lithium-polymer-battery-pack-lipo/) | 7.4 V nominal, 850 mAh, 30C (about 25 A burst). Low sag under motor bursts, which helps avoid ESP32 brown-outs. |
| **Wheels** | 3Pi MiniQ Wheel Tyre 42 mm for N20 Motor | 2 | 59 | 118 | [Probots](https://probots.co.in/3pi-miniq-car-wheel-tyre-42mm-for-n20-motor.html) | Rubber tyre for grip on the maze surface. Sold per wheel, so two are needed. |
| **Motor Mounts** | Black N20 Plastic Motor Fixed Seat | 2 | 35 | 70 | [Probots](https://probots.co.in/black-n20-plastic-motor-fixed-seat.html) | Holds each N20 motor to the base plate. Sold singly. |
| **Caster** | Metal Ball Caster Wheel for Small Robotic Car | 1 | 41 | 41 | [Zbotic](https://zbotic.in/product/metal-ball-caster-wheel-for-small-robotic-car/) | Low-friction third point of contact at the rear. |
| **Chassis / Base Plate** | Custom base plate (acrylic, 3D-printed or perfboard) | 1 | 150 *(est.)* | 150 | None (est.) | Keeps the build light and low. No ready-made N20 chassis kit was both in stock and under budget (the one found was ₹3,890 and sold out). |
| **Power Regulation** | LM2596 DC-DC Step-Down Buck Module (adjustable) | 1 | 99 | 99 | [Probots](https://probots.co.in/lm2596-switching-type-dc-to-dc-step-down-regulator-buck-module.html) | Input 4.75–35 V, adjustable output, 2 A rated. Set to 5 V with a multimeter **before** connecting the ESP32. |
| **Wiring & Connectors** | Female-to-Female Jumper Wires, 40 pcs, 10 cm | 1 | 49 | 49 | [Probots](https://probots.co.in/female-to-female-jumper-wires-40-pcs-10cm.html) | Signal and power connections between modules. |
| **Power Switch & Misc.** | Slide switch, battery connector, heat-shrink | 1 | 60 *(est.)* | 60 | None (est.) | Allows the robot to be switched off without unplugging the battery. |
| **Charger** | 2S LiPo balance charger | 1 | 1,200 *(est.)* | 1,200 | None (est.) | A 2S LiPo must be charged on a balance charger. Check the exact model and price before ordering. |
| **Shipping** | Courier across 4 suppliers (Robu, ElectronicsComp, Probots, Zbotic) | - | 60 *(est.)* each | 240 | n/a | Some orders may ship free above a threshold (Robu showed ₹499). |
| **Contingency** | 10 % of parts cost | - | - | 418 | n/a | Spare wires, a spare sensor module or a damaged part. |
| **TOTAL ESTIMATED COST** | | | | **₹4,837** | | Well under the ₹10,000 limit. |

Verified part prices total ₹2,769. The ₹1,410 of estimated items (base plate, switch and misc., charger) is a rough figure.

### Stock to re-check before ordering
- **N20 600 RPM motor:** one listing for the same motor (Zbotic) showed "Out of Stock"; the Robu page didn't state stock. If it is unavailable, any in-stock N20 6 V motor of similar RPM works, but re-check the TB6612 current margin.
- **ESP32 board, SmartElex sensor, battery:** the pages did not show stock.
- **SmartElex sensor supply voltage:** the page did not list it. Confirm it works at 3.3 V.

---

## 2. Component Justification & Trade-off Analysis

### Why TB6612FNG over the L298N?
* **Selected:** TB6612FNG dual driver
* **Alternative:** L298N dual H-bridge module
* **Justification:** The L298N uses bipolar transistors and drops roughly 1.4–2 V across its output stage. On a 7.4 V battery that leaves the motors only about 5.5–6 V and turns the lost power into heat. The TB6612FNG uses MOSFETs, so its voltage drop is small, it runs cooler, and its 1.2 A continuous per channel comfortably covers the 0.67 A motor stall current. Its logic accepts 3.3 V, which suits the ESP32 directly.

### Why N20 micro metal gear motors over standard plastic BO motors?
* **Selected:** N20 6 V 600 RPM
* **Alternative:** Standard plastic BO motors
* **Justification:** N20 motors are much smaller and lighter, and a lighter robot accelerates and turns faster. They offer higher RPM options, giving a higher top speed on straight sections.

---

## 3. Power-Block Diagram

```text
                +---------------------------+
                |  7.4 V 2S LiPo (850 mAh)  |
                +---------------------------+
                      |                |
                      | 7.4 V          | 7.4 V
                      v                v
              +-------------+   +-------------------------+
              |   LM2596    |   |  TB6612FNG  (VM = 7.4 V)|
              |  buck, 5 V  |   |  VCC = 3.3 V from ESP32 |
              +-------------+   +-------------------------+
                      |             ^          |
                      | 5 V         | PWM/DIR  | motor outputs
                      v             |          v
              +-------------+-------+     +-----------+
              |   ESP32     |             | 2 x N20   |
              |  (Vin = 5 V)|             | motors    |
              +-------------+             +-----------+
                      |
                      | 3.3 V (ESP32 3V3 pin)
                      v
              +---------------------------+
              |  5-ch IR sensor array     |
              |  analog outputs -> ADC    |
              +---------------------------+

  All grounds joined: battery -, LM2596, TB6612FNG, ESP32, sensor array.
```
