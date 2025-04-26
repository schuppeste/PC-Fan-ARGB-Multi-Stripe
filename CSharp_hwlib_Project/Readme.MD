# LibreHardwareMonitor RGB Fan Controller

A C# application that connects LibreHardwareMonitor with an ESP32-based RGB fan controller. This application enables temperature-based RGB lighting effects for PC fans by monitoring real-time hardware data.

## 🚀 Features

- Real-time hardware monitoring using LibreHardwareMonitor
- Serial communication with ESP32 RGB fan controller
- JSON-based data exchange
- Support for multiple hardware sensors:
  - CPU temperature
  - GPU temperature
  - Motherboard sensors
- Queue-based message handling for reliable communication
- Debug output support

## 📋 Requirements

- .NET 6.0 or higher
- Visual Studio 2022 or Visual Studio Code
- Libraries:
  ```xml
  <PackageReference Include="LibreHardwareMonitorLib" Version="0.9.3" />
  <PackageReference Include="Newtonsoft.Json" Version="13.0.3" />
  <PackageReference Include="SerialPortLib" Version="1.0.23" />
  ```

## 🔧 Setup

1. Clone the repository
2. Install required NuGet packages
3. Configure your COM port in the code:
   ```csharp
   serialPort.SetPort("COM7", 115200);  // Adjust COM port as needed
   ```

## 💻 Usage

The application accepts the following commands via serial connection:

- `liste`: Returns a complete list of available sensors
- `DEBUG`: Prints debug information
- JSON format for specific sensor requests:
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

## 🔄 Data Flow

1. ESP32 sends sensor configuration
2. Application monitors hardware using LibreHardwareMonitor
3. Filtered sensor data is sent back to ESP32
4. ESP32 updates RGB fans based on temperature values

## 📊 Example Response

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

## ⚠️ Notes

- Requires administrative privileges for hardware monitoring
- Make sure to close other applications using the COM port
- Hardware names must match exactly with LibreHardwareMonitor's naming

## 🤝 Contributing

Feel free to submit issues and enhancement requests!

## 📜 License

This project is licensed under the MIT License - see the LICENSE file for details.

---

Made with ❤️ for the PC modding community
