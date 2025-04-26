Here's a GitHub description with setup instructions for the defines:

# ESP32 RGB Fan Controller

Control RGB PC fans with temperature-based animations using an ESP32 and FastLED. Connects to LibreHardwareMonitorLib in for real-time hardware monitoring.

## 🎯 Features
- Temperature-based RGB animations
- Multiple fan support (up to 8 channels)
- Real-time hardware monitoring
- JSON-based configuration
- Supports various RGB fan models

## 🔧 Hardware Setup

### Pin Configuration
```cpp
// ESP32 Output Enable Pin
#define TCB_OE_Pin 23  // Controls the signal output

// ESP32 Data Pins for LED strips
#define TXA_1 27       // Fan Channel 1
#define TXA_2 25       // Fan Channel 2
#define TXA_3 32       // Fan Channel 3
#define TXA_4 17       // Fan Channel 4
#define TXA_5 16       // Fan Channel 5
#define TXA_6 22       // Fan Channel 6 (unused)
#define TXA_7 21       // Fan Channel 7 (unused)
#define TXA_8 33       // Fan Channel 8
```

### Fan Configuration
Define your fan layout using these indices (0-based):
```cpp
// Fan Position Definitions
#define FAN_BACK 1     // Back exhaust fan
#define FAN_FRONT_1 0  // Front intake fan 1
#define FAN_FRONT_2 4  // Front intake fan 2
#define FAN_FRONT_3 5  // Front intake fan 3
#define FAN_CPU_1 2    // CPU fan 1
#define FAN_CPU_2 3    // CPU fan 2
#define FAN_CUSTOM_1 6 // Custom fan 1 (unused)
#define FAN_CUSTOM_2 7 // Custom fan 2 (unused)
```

### LED Configuration
```cpp
#define NUMm_LEDS 12    // Number of LEDs per fan
#define NUMm_Strings 8  // Number of fan channels
```

## 📌 Pin Mapping Example
```
ESP32 -> Fan Channel
GPIO27 -> Fan Channel 1 (FAN_FRONT_1)
GPIO25 -> Fan Channel 2 (FAN_BACK)
GPIO32 -> Fan Channel 3 (FAN_CPU_1)
GPIO17 -> Fan Channel 4 (FAN_FRONT_2)
GPIO16 -> Fan Channel 5 (FAN_FRONT_3)
```

## 🛠️ Setup Instructions

1. **Hardware Connection**
   - Connect the ESP32 pins to your RGB fan hub/controller
   - Ensure common ground between ESP32 and fan controller
   - Connect TCB_OE_Pin (GPIO23) to enable signal output

2. **Fan Configuration**
   - Modify the fan indices in the defines to match your setup
   - Update the `fanConfigs` array with your hardware information:
   ```cpp
   FanConfig fanConfigs[8] = {
     {FAN_BACK,    0, 75, "Nuvoton NCT6687D", "Temperature", "CPU Core", 0, setRGBRingoneColorFading},
     // ... additional fans
   };
   ```

3. **LED Strip Configuration**
   - Update NUMm_LEDS if your fans have a different number of LEDs
   - Modify NUMm_Strings based on number of channels you're using

## 📝 Notes
- Unused fan channels should be defined but not configured in fanConfigs
- Ensure proper power supply for all connected fans
- The TCB_OE_Pin must be HIGH for LED output

## ⚡ Power Requirements
- Each RGB fan can draw up to 60mA per LED
- Calculate total power requirements: `NUMm_LEDS * NUMm_Strings * 60mA`
- Use appropriate power supply based on calculations

## 🔗 Dependencies
- FastLED Library
- ArduinoJson Library

## 🛟 Troubleshooting
- If LEDs don't light up, check TCB_OE_Pin
- Verify pin connections match the defines
- Ensure fan indices in fanConfigs match physical connections

For more detailed information, check the code comments and documentation.
