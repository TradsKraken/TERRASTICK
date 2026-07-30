# TERRASTICK: Portable Soil Analysis Device

TERRASTICK is an ESP32-based portable soil analysis device that provides quantitative measurements for agricultural soil monitoring. It communicates with industrial RS485 soil sensors via the Modbus RTU protocol and displays real-time data on a 20x4 I2C LCD screen.

## Features
- **Measures 7 Parameters:** Nitrogen, Phosphorus, Potassium, Moisture, Temperature, EC, and pH.
- **Auto-Diagnostics:** Built-in startup check to verify sensor connectivity.
- **Live Error Detection:** Flashes screen warnings if a sensor disconnects during operation.
- **Clean UI:** Custom-padded 20x4 LCD grid layout to prevent ghost characters.
- **Hardware Validated:** Uses robust `HardwareSerial` on the ESP32 for uninterrupted communication.
- **Calibration Support:** Built-in floating point offset variables for easy fine-tuning.

## Components Required
- ESP32 Development Board
- RS485 to TTL Module (e.g., MAX485 or MAX3485)
- Soil NPK Sensor (RS485)
- Soil 4-Parameter Sensor (Moisture, Temp, EC, pH) (RS485)
- 20x4 I2C LCD Display
- 12V DC Power Supply (for the sensors)

## Wiring Diagram

![TERRASTICK Wiring Diagram](Wiring_Terrastick.png)

### 1. MAX485 Module ➔ ESP32
| MAX485 Pin | ESP32 Pin | Notes |
| :--- | :--- | :--- |
| **VCC** | **3.3V** | ESP32 uses 3.3V logic. Powering the MAX485 from 3.3V is safer. |
| **GND** | **GND** | Common ground. |
| **RO** | **GPIO 16 (RX2)** | Receives data from the sensor. |
| **DI** | **GPIO 17 (TX2)** | Sends data to the sensor. |
| **DE & RE** | **GPIO 4** | Jumper DE and RE together. Switches module between Transmit/Receive. |

### 2. LCD Display ➔ ESP32
| LCD Pin | ESP32 Pin |
| :--- | :--- |
| **VCC** | **5V (VIN)** |
| **GND** | **GND** |
| **SDA** | **GPIO 21** |
| **SCL** | **GPIO 22** |

### 3. Sensors ➔ MAX485 & Power
| Sensor Wire | Connection |
| :--- | :--- |
| **A (Yellow/Green)** | MAX485 `A` Terminal |
| **B (Blue)** | MAX485 `B` Terminal |
| **VCC (Brown/Red)** | 12V Power Supply (+) |
| **GND (Black)** | 12V Power Supply (-) **AND ESP32 GND** |

> **⚠️ CRITICAL: COMMON GROUND**
> The 12V Power Supply Negative (-) MUST be tied to the ESP32 GND, otherwise RS485 communication will fail.

## Folder Structure
* `/terrastick_sensors/`: The main ESP32 code. Upload this to run the device.
* `/change_address/`: A one-time utility script to change the Modbus Address of the 4-Parameter sensor from `0x01` to `0x02`. 

## Setup & Installation Instructions
1. Download the `LiquidCrystal I2C` library by Frank de Brabander via the Arduino Library Manager.
2. Out of the box, both sensors likely have the default Modbus Address of `0x01`. 
3. Wire up **ONLY** the 4-Parameter Sensor to the MAX485 module.
4. Flash the `change_address.ino` script to the ESP32. Check the Serial Monitor (115200 baud) to verify it successfully changed the address to `0x02`.
5. Now, wire **BOTH** sensors to the MAX485 module in parallel (A to A, B to B).
6. Flash the `terrastick_sensors.ino` script to your ESP32.
7. The system will boot, check both sensors, and seamlessly display the data on the LCD!

## How to Calibrate

If you notice your readings are slightly off (e.g., pH reads 6.5 in neutral 7.0 distilled water), you can easily calibrate the device using the built-in software offsets.

1. **Test your sensors** against a known baseline:
   - **pH:** Use distilled water (should be 7.0) or commercial pH calibration buffers.
   - **Moisture:** Dry air (should be 0%) and fully saturated soil (should be ~100%).
   - **Temperature:** Compare with a reliable room thermometer.
   - **EC:** Use distilled water (should be 0) or EC calibration liquid.
   - **NPK:** Compare against a chemical soil test kit or laboratory results.
2. **Calculate the difference:** If your sensor reads `6.7` in `7.0` water, your offset is `+0.3`.
3. **Update the code:** Open `terrastick_sensors.ino` and find the `CALIBRATION OFFSETS` section at the top of the file.
4. **Apply the offset:** Change `float CALIBRATE_PH = 0.0;` to `float CALIBRATE_PH = 0.3;`. 
5. **Flash the updated code:** The ESP32 will automatically apply these offsets to all future readings (and automatically prevents impossible negative values like negative moisture).

## Project Showcase

### 1. Prototyping
Here is the initial prototype built on a breadboard:
![Prototype](assets/Protoype_of_the_device_in_breadboard.jfif)

### 2. PCB Development
Moving the prototype to a soldered perfboard:
![Perfboard Development 1](assets/Developing_of_the_PCB_perfboard.jfif)
![Perfboard Development 2](assets/Developing_of_the_PCB_perfboard1.jfif)

### 3. Final Assembly
The completed circuit board:
![PCB Front](assets/Actual_Photo_PCB_Front.jfif)
![PCB in Perfboard](assets/Actual_Photo_of_PCB_in_Perfboard.jfif)

### 4. Working Device
The device successfully reading sensor data!
![Sensors in Soil](assets/Photo_of_sesors_in_soil.jfif)
![Serial Monitor Readings](assets/Readings_in_Serial_Monitor.jfif)
![Device Working](assets/Photo_of_Device_Working.jfif)

### 5. Video Demonstration
🎥 [Click here to watch the LCD Working Demo video](assets/Video_of_LCD_working.mp4)
