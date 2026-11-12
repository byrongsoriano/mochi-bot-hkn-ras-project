<img width="404" height="197" alt="image" src="https://github.com/user-attachments/assets/a401e376-43a4-45b5-b5b1-e76baff24110" />

# DIY Mochi Bot Interactive OLED Desk Companion workshop (HKN x IEEE RAS)

An interactive desktop pet and companion powered by the **Seeed Studio XIAO ESP32-C3**. It features animated facial expressions on a 128x64 OLED display, capacitive touch sensing, and synthesized PWM audio feedback.



---

## Features

* **Animated Expressions:** Renders smooth 25 FPS (`40ms` frame interval) XBM bitmap animations across four states: *Normal*, *Happy*, *Surprised*, and *Love*.
* **Touch-Triggered Reactions:** Uses a debounced (`50ms`) touch sensor to cycle sequentially through *Happy*, *Surprised*, and *Love* animations while the sensor is held.
* **Synthesized Audio Feedback:** Generates distinct multi-tone PWM melodies on a passive piezo buzzer corresponding to each emotional reaction.
* **Idle Typewriter Screen:** Automatically transitions to a centered `"HKN x RAS"` typewriter marquee after **5 seconds** of inactivity.

---

## Hardware Requirements

| Component | Specification / Notes |
| :--- | :--- |
| **Microcontroller** | Seeed Studio XIAO ESP32-C3 |
| **Display** | 0.96" SSD1306 128x64 I2C OLED Display |
| **Input Sensor** | Capacitive Touch Sensor (Active HIGH, e.g., TTP223) |
| **Audio Output** | Passive Piezo Buzzer |

---

## Pinout & Wiring

| Component | Component Pin | XIAO ESP32-C3 Pin | Notes |
| :--- | :--- | :--- | :--- |
| **SSD1306 OLED** | SDA | `D4` | Hardware I2C Data |
| **SSD1306 OLED** | SCL | `D5` | Hardware I2C Clock |
| **SSD1306 OLED** | VCC / GND | `3V3` / `GND` | Power |
| **Touch Sensor** | SIG / OUT | `D2` | Configured with `INPUT_PULLDOWN` (Active `HIGH`) |
| **Touch Sensor** | VCC / GND | `3V3` / `GND` | Power |
| **Piezo Buzzer** | Positive (`+`) | `D1` | Driven via ESP32 LEDC PWM (`2000 Hz`, 8-bit) |
| **Piezo Buzzer** | Negative (`-`) | `GND` | Ground |

---

## Software Dependencies

* **Framework:** Arduino (ESP32 Board Package v3.0+ required for `ledcAttach` / `ledcWriteTone` APIs)
* **Libraries:**
  * [`U8g2`](https://github.com/olikraus/u8g2) by olikraus (for SSD1306 XBM rendering and fonts)
  * `Wire.h` (Built-in I2C library)

---

## Project Structure

```text
├── main.ino         # Core state machine, touch debouncing, audio, and display logic
└── bitmaps.h        # XBM byte arrays, frame pointer arrays, and frame count constants
```
## Arduino IDE & Board Setup

### 1. Install Arduino IDE
1. Download the latest **Arduino IDE** (Windows, macOS, or Linux) from the [official Arduino Software page](https://www.arduino.cc/en/software).
2. Run the installer and launch the Arduino IDE.

### 2. Add ESP32 Board Support
1. Open **File > Preferences** (Windows/Linux) or **Arduino IDE > Settings** (macOS).
2. Locate the **Additional Boards Manager URLs** field and paste the official Espressif index URL:
   ```text
   [https://espressif.github.io/arduino-esp32/package_esp32_index.json](https://espressif.github.io/arduino-esp32/package_esp32_index.json)
   ```
