# PC-Fan-ARGB-Multi-Stripe
Simple Workaround for Multiple 1 End ARGB Fans in PC-Cases. (without OUT Pin , 3Pin) 

Example Video: [![Example Video]()](https://www.youtube.com/shorts/zQxECo8_se8)

A Arduino creates Fan Color Animations controlled bei any available System,CPU/GPU Sensor Value.

How it works:

The Dotnet Tool wait for 3 Commands in Serial Communication.

1. Show List "liste\r\n" -> Send a List with all available Hardware Sensors as JSON back to Serial and Console.
2. For debug , "DEBUG............\r\n" -> Redirect theReceived Texts to System Console.
3. Input, parsed Json for Filtered sensors and sends a Json with all defined Sensors.
4. Example Json sending from Arduino to Console App: "

Question Json:
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

Answer to Arduino: [
  {
    "Hardware": "Nuvoton NCT6687D",
    "Sensors": [
      {
        "SensorName": "CPU",
        "SensorType": "Temperature",
        "Value": "35,5"
      },
      {
        "SensorName": "VRM MOS",
        "SensorType": "Temperature",
        "Value": "46"
      }
    ]
  },
  {
    "Hardware": "AMD Radeon RX 7600 XT",
    "Sensors": [
      {
        "SensorName": "GPU Core",
        "SensorType": "Temperature",
        "Value": "45"
      }
    ]
  }
]

Arduino Configuration:

1. Define used Pins in "TXB_" Constants
2. Define Pinorder for readable Constants (FAN_)
3. Define Fanconfigs:
4. ```FanConfig fanConfigs[8] = {  {FAN_BACK     ,0, 75,      "Nuvoton NCT6687D",       "Temperature",  "CPU Core",                    0,  setRGBRingoneColorFading}}```
5. ```FanConfig fanConfigs[8] = {  {Stripe-Index , min, max,  "Hardware Key"    ,       "Sensor-Type",  "Sensor Name",   Updated Value  ,  Function for Animation}}```
6. min/max is a Scaled and Mapped Value to Expand this Range to 0-255 for Animation
7. The Hardware Key, Sensor-Type ,and SensorName are defined in the CLI-Tool and was listed bei the "liste" Command 

![platine](https://github.com/user-attachments/assets/48abebe9-8365-444c-a97c-e301bdfea8fd)
![fritzing_rgb](https://github.com/user-attachments/assets/fc334b93-069c-4e9f-a7b8-2eccac0a516a)


