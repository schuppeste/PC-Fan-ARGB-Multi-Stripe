# PC-Fan-ARGB-Multi-Stripe
Simple Workaround for Multiple 1 End ARGB Fans in PC-Cases. (without OUT Pin , 3Pin)

How it works:

The Dotnet Tool wait for 3 Commands in Serial Communication.

1. Show List "liste\r\n" -> Send a List with all available Hardware Sensors as JSON back to Serial and Console.
2. For debug , "DEBUG............\r\n" -> Redirect the Texts to System Console.
3. Input, parsed Json for Filtered sensors and sends a Json with all defined Sensors.
2.1 Example Json sending from Arduino to Console App: "

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







Example Video:
[![Example Video]()](https://www.youtube.com/shorts/zQxECo8_se8)

![platine](https://github.com/user-attachments/assets/48abebe9-8365-444c-a97c-e301bdfea8fd)
![fritzing_rgb](https://github.com/user-attachments/assets/fc334b93-069c-4e9f-a7b8-2eccac0a516a)


