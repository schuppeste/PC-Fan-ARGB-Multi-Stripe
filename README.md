# RGB Fan Controller - ESP32 & C# Integration

This project consists of two components that work together to control RGB fans based on real-time hardware sensor data. The integration is achieved through JSON-based communication over a serial connection.

---

## 📂 Components

### 1. **ESP32 Firmware**
The ESP32 acts as the RGB fan controller. It:
- Manages up to 8 RGB fan channels.
- Animates LEDs based on temperature values.
- Sends its fan configuration to the C# application.
- Receives real-time temperature data from the C# application.

### 2. **C# Application**
The C# application interfaces with LibreHardwareMonitor to:
- Monitor hardware sensors (CPU, GPU, motherboard, etc.).
- Filter sensor data based on the ESP32's configuration.
- Send temperature data to the ESP32 for RGB control.

---

## 🔄 How It Works

### 1. **Fan Configuration**
The ESP32 defines the fan configuration using the `FanConfig` structure:
```cpp
struct FanConfig {
  int index;          // Index of the LED strip
  int min;            // Minimum temperature value
  int max;            // Maximum temperature value
  String hwKey;       // Hardware key (e.g., "Nuvoton NCT6687D")
  String sensorType;  // Sensor type (e.g., "Temperature")
  String sensorName;  // Sensor name (e.g., "CPU Core")
  int value;          // Current mapped value (0-255)
  void (*animationFunction)(int, int); // Pointer to the animation function
};
```

The `fanConfigs` array defines the configuration for all connected fans:
```cpp
FanConfig fanConfigs[8] = {
  {FAN_BACK, 0, 75, "Nuvoton NCT6687D", "Temperature", "CPU Core", 0, setRGBRingoneColorFading},
  {FAN_FRONT_1, 50, 80, "Nuvoton NCT6687D", "Temperature", "CPU Core", 0, setRGBRingoneColorFading},
  {FAN_FRONT_2, 0, 95, "Nuvoton NCT6687D", "Temperature", "VRM MOS", 0, setRGBRingoneColorFading},
  {FAN_FRONT_3, 0, 70, "AMD Radeon RX 7600 XT", "Temperature", "GPU Core", 0, setRGBRingoneColorFading},
  {FAN_CPU_1, 35, 80, "", "", "", 0, setRGBRingoneColorFading},
  {FAN_CPU_2, 35, 80, "", "", "", 0, setRGBRingoneColorFading},
  {FAN_CUSTOM_1, 35, 80, "", "", "", 0, setRGBRingoneColorFading},
  {FAN_CUSTOM_2, 35, 80, "", "", "", 0, setRGBRingoneColorFading}
};
```

### 2. **Startup**
- The ESP32 sends its fan configuration as a JSON object to the C# application.
- Example JSON sent by the ESP32:
  ```json
  {
    "Nuvoton NCT6687D": [
      {
        "SensorName": "CPU Core",
        "SensorType": "Temperature"
      },
      {
        "SensorName": "VRM MOS",
        "SensorType": "Temperature"
      }
    ],
    "AMD Radeon RX 7600 XT": [
      {
        "SensorName": "GPU Core",
        "SensorType": "Temperature"
      }
    ]
  }
  ```

### 3. **Sensor Monitoring**
- The C# application uses LibreHardwareMonitor to retrieve real-time sensor data.
- Example sensor data:
  ```json
  [
    {
      "Hardware": "Nuvoton NCT6687D",
      "Sensors": [
        {
          "SensorName": "CPU Core",
          "SensorType": "Temperature",
          "Value": "45.5"
        },
        {
          "SensorName": "VRM MOS",
          "SensorType": "Temperature",
          "Value": "52.3"
        }
      ]
    },
    {
      "Hardware": "AMD Radeon RX 7600 XT",
      "Sensors": [
        {
          "SensorName": "GPU Core",
          "SensorType": "Temperature",
          "Value": "60.1"
        }
      ]
    }
  ]
  ```

### 4. **Data Exchange**
- The ESP32 requests specific sensor data by sending a JSON object.
- The C# application filters the sensor data based on the request and sends the relevant data back to the ESP32.

### 5. **RGB Control**
- The ESP32 maps the received temperature values to RGB animations.
- Example mapping:
  - Green: Low temperature
  - Yellow: Medium temperature
  - Red: High temperature

---

## 🛠️ Setup Instructions

### ESP32 Firmware
1. Flash the provided firmware to your ESP32.
2. Connect the RGB fans to the appropriate GPIO pins as defined in the firmware:
   ```cpp
   #define TXA_1 27  // Fan Channel 1
   #define TXA_2 25  // Fan Channel 2
   #define TXA_3 32  // Fan Channel 3
   #define TXA_4 17  // Fan Channel 4
   #define TXA_5 16  // Fan Channel 5
   #define TXA_6 22  // Fan Channel 6 (unused)
   #define TXA_7 21  // Fan Channel 7 (unused)
   #define TXA_8 33  // Fan Channel 8
   ```

### C# Application
1. Clone the repository and open the project in Visual Studio.
2. Install the required NuGet packages:
   - `LibreHardwareMonitorLib`
   - `Newtonsoft.Json`
   - `SerialPortLib`
3. Configure the serial port in the code:
   ```csharp
   serialPort.SetPort("COM7", 115200);  // Replace "COM7" with your ESP32's COM port
   ```
4. Run the application.

---

## 📡 Communication Protocol

### Commands
1. **`liste`**: The ESP32 requests a full list of available sensors.
2. **JSON Request**: The ESP32 requests specific sensor data by sending a JSON object.
3. **Debugging**: The ESP32 can send debug messages for troubleshooting.

### Message Format
- All messages are sent as JSON strings.
- Messages are terminated with `\r\n`.

---

## 🔗 Example Workflow

1. **ESP32 → C#**: Send fan configuration.
   ```json
   {
     "Nuvoton NCT6687D": [
       {
         "SensorName": "CPU Core",
         "SensorType": "Temperature"
       }
     ]
   }
   ```

2. **C# → ESP32**: Respond with filtered sensor data.
   ```json
   [
     {
       "Hardware": "Nuvoton NCT6687D",
       "Sensors": [
         {
           "SensorName": "CPU Core",
           "SensorType": "Temperature",
           "Value": "45.5"
         }
       ]
     }
   ]
   ```

3. **ESP32**: Update RGB animations based on the received temperature values.

---

## 🛠️ Debugging

### ESP32
- Use `Serial.print` to debug JSON messages and RGB animations.

### C# Application
- Use `Console.WriteLine` to debug received and sent JSON messages.
- Check the COM port connection and ensure no other application is using it.

---

## 📋 Notes
- Ensure the ESP32 and C# application use the same baud rate (115200).
- The hardware names in the ESP32's JSON must match exactly with LibreHardwareMonitor's names.
- Use a sufficient power supply for the RGB fans.

---

This project enables seamless integration between hardware monitoring and RGB fan control, providing a dynamic and visually appealing way to monitor your PC's performance.
