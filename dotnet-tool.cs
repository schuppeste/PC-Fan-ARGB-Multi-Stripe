using System;
using System.IO.Ports;
using LibreHardwareMonitor.Hardware;
using Newtonsoft.Json;
using SerialPortLib;
using System.Threading.Tasks;
using System.CodeDom;
var serialPort = new SerialPortInput();

// Listen to Serial Port events

serialPort.ConnectionStatusChanged += delegate (object sender, ConnectionStatusChangedEventArgs args)
{
    Console.WriteLine("Connected = {0}", args.Connected);
};
String test=""; 
serialPort.MessageReceived += delegate (object sender, MessageReceivedEventArgs args)
{
    string receivedString = System.Text.Encoding.UTF8.GetString(args.Data);
    Console.Write("{0}", receivedString);
};

// Set port options
serialPort.SetPort("COM7", 115200);

// Connect the serial port
serialPort.Connect();

// Send a message
//var message = System.Text.Encoding.UTF8.GetBytes("Hello World!");
//serialPort.SendMessage(message);

//SerialPort serialPort = new SerialPort("COM4", 115200); // Replace "COM3" with your serial port
//serialPort.Open();
Computer computer = new Computer
{
    IsCpuEnabled = true,
    IsGpuEnabled = true,
    IsMotherboardEnabled = true,
    IsControllerEnabled = true,
    IsNetworkEnabled = true
};



while (true)
{

    computer.Open();
    computer.Accept(new UpdateVisitor());



    try
    {
        //serialPort.WriteLine($"\"{computer.Hardware[0].SubHardware[0].Sensors[22].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[22].Value}\",\r\n");

  serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"{{"));

    if (computer.Hardware[0].SubHardware.Length > 0 && computer.Hardware[0].SubHardware[0].Sensors.Length > 22)
        serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"\"{computer.Hardware[0].SubHardware[0].Sensors[22].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[22].Value}\","));
else throw new Exception("Sensor 22 not found");
    if (computer.Hardware[0].SubHardware.Length > 0 && computer.Hardware[0].SubHardware[0].Sensors.Length > 23)
        serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"\"{computer.Hardware[0].SubHardware[0].Sensors[23].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[23].Value}\","));
else throw new Exception("Sensor 23 not found");
    if (computer.Hardware[0].SubHardware.Length > 0 && computer.Hardware[0].SubHardware[0].Sensors.Length > 24)
        serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"\"{computer.Hardware[0].SubHardware[0].Sensors[24].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[24].Value}\","));
else throw new Exception("Sensor 24 not found");    
    if (computer.Hardware[0].SubHardware.Length > 0 && computer.Hardware[0].SubHardware[0].Sensors.Length > 25)
        serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"\"{computer.Hardware[0].SubHardware[0].Sensors[25].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[25].Value}\","));
else throw new Exception("Sensor 25 not found");
    if (computer.Hardware[0].SubHardware.Length > 0 && computer.Hardware[0].SubHardware[0].Sensors.Length > 26)
        serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"\"{computer.Hardware[0].SubHardware[0].Sensors[26].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[26].Value}\","));
else throw new Exception("Sensor 26 not found");
 //   if (computer.Hardware[2].Length > 2 && computer.Hardware[2].Sensors.Length > 22)
  //      serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"\"{computer.Hardware[2].Sensors[22].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[22].Value}\","));

   // if (computer.Hardware[2].Length > 2 && computer.Hardware[2].Sensors.Length > 24)
   //     serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"\"{computer.Hardware[2].Sensors[24].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[24].Value}\","));

    if (computer.Hardware[0].SubHardware.Length > 0 && computer.Hardware[0].SubHardware[0].Sensors.Length > 0)
        serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"\"{computer.Hardware[0].SubHardware[0].Sensors[0].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[0].Value}\","));
else throw new Exception("Sensor 0 not found");
    if (computer.Hardware[0].SubHardware.Length > 0 && computer.Hardware[0].SubHardware[0].Sensors.Length > 2)
        serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"\"{computer.Hardware[0].SubHardware[0].Sensors[2].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[2].Value}\","));
else throw new Exception("Sensor 2 not found");
    if (computer.Hardware[0].SubHardware.Length > 0 && computer.Hardware[0].SubHardware[0].Sensors.Length > 3)
        serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"\"{computer.Hardware[0].SubHardware[0].Sensors[3].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[3].Value}\""));
else throw new Exception("Sensor 3 not found");
    serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes($"}}\r\n"));
        //   serialPort.WriteLine($"\"{computer.Hardware[0].SubHardware[0].Sensors[22].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[22].Value}\",\r\n");
 //Console.WriteLine($"\"{computer.Hardware[0].SubHardware[0].Sensors[22].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[22].Value}\",");
        
        /* Console.WriteLine($"{{");
         Console.WriteLine($"\"{computer.Hardware[0].SubHardware[0].Sensors[22].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[22].Value}\",");
         Console.WriteLine($"\"{computer.Hardware[0].SubHardware[0].Sensors[23].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[23].Value}\",");
         Console.WriteLine($"\"{computer.Hardware[0].SubHardware[0].Sensors[24].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[24].Value}\",");
         Console.WriteLine($"\"{computer.Hardware[0].SubHardware[0].Sensors[25].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[25].Value}\",");
         Console.WriteLine($"\"{computer.Hardware[0].SubHardware[0].Sensors[26].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[26].Value}\",");
         Console.WriteLine($"\"{computer.Hardware[2].Sensors[22].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[22].Value}\",");
         Console.WriteLine($"\"{computer.Hardware[2].Sensors[24].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[24].Value}\",");
         Console.WriteLine($"\"{computer.Hardware[0].SubHardware[0].Sensors[0].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[0].Value}\",");
         Console.WriteLine($"\"{computer.Hardware[0].SubHardware[0].Sensors[2].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[2].Value}\",");
         Console.WriteLine($"\"{computer.Hardware[0].SubHardware[0].Sensors[3].Name}\": \"{computer.Hardware[0].SubHardware[0].Sensors[3].Value}\"");
         Console.WriteLine($"}}\r\n");*/
    }
    catch (Exception e)
    {
        Console.WriteLine($"Error writing to serial port: {e.Message}");
    }


    Task.Delay(100).Wait();
    computer.Close();
}



public class UpdateVisitor : IVisitor
{
    public void VisitComputer(IComputer computer)
    {
        computer.Traverse(this);
    }
    public void VisitHardware(IHardware hardware)
    {
        hardware.Update();
        foreach (IHardware subHardware in hardware.SubHardware) subHardware.Accept(this);
    }
    public void VisitSensor(ISensor sensor) { }
    public void VisitParameter(IParameter parameter) { }
}

