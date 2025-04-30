using System;
using System.IO.Ports;
using LibreHardwareMonitor.Hardware;
using Newtonsoft.Json;
using SerialPortLib;
using System.Threading.Tasks;
using System.CodeDom;
using System.Collections.Generic;

Queue<string> messageQueue = new Queue<string>();
var serialPort = new SerialPortInput();
Computer computer = new Computer
{
    IsCpuEnabled = true,
    IsGpuEnabled = true,
    IsMotherboardEnabled = true,
    IsControllerEnabled = true,
    IsNetworkEnabled = true
};
String lastjson = "";


// Listen to Serial Port events
static string FilterSensorsByJson(Computer computer, Dictionary<string, List<Dictionary<string, string>>> selection)
{
    var filteredData = new List<object>();

    foreach (IHardware hardware in computer.Hardware)
    {
        // Neue Liste für die Sensoren der aktuellen Hardware
        var selectedSensors = new List<object>();

        if (selection.ContainsKey(hardware.Name))
        {
            foreach (ISensor sensor in hardware.Sensors)
            {
                // Überprüfe, ob der Sensor im Filter enthalten ist
                if (selection[hardware.Name].Any(filter =>
                    filter["SensorName"] == sensor.Name &&
                    filter["SensorType"] == sensor.SensorType.ToString()))
                {
                    selectedSensors.Add(new
                    {
                        SensorName = sensor.Name,
                        SensorType = sensor.SensorType.ToString(),
                        Value = sensor.Value?.ToString()
                    });
                }
            }

            if (selectedSensors.Any())
            {
                filteredData.Add(new
                {
                    Hardware = hardware.Name,
                    Sensors = selectedSensors
                });
            }
        }

        // Suche in der Sub-Hardware
        foreach (IHardware subHardware in hardware.SubHardware)
        {
            var subSelectedSensors = new List<object>();

            if (selection.ContainsKey(subHardware.Name))
            {
                foreach (ISensor subSensor in subHardware.Sensors)
                {
                    // Überprüfe, ob der Sub-Sensor im Filter enthalten ist
                    if (selection[subHardware.Name].Any(filter =>
                        filter["SensorName"] == subSensor.Name &&
                        filter["SensorType"] == subSensor.SensorType.ToString()))
                    {
                        subSelectedSensors.Add(new
                        {
                            SensorName = subSensor.Name,
                            SensorType = subSensor.SensorType.ToString(),
                            Value = subSensor.Value?.ToString()
                        });
                    }
                }

                if (subSelectedSensors.Any())
                {
                    filteredData.Add(new
                    {
                        Hardware = subHardware.Name,
                        Sensors = subSelectedSensors
                    });
                }
            }
        }
    }

    return JsonConvert.SerializeObject(filteredData, Formatting.Indented);
}

static string GenerateSensorList(Computer computer)
{
    var sensorList = new List<object>();

    foreach (IHardware hardware in computer.Hardware)
    {
        foreach (ISensor sensor in hardware.Sensors)
        {
            sensorList.Add(new
            {
                Hardware = hardware.Name,
                SensorName = sensor.Name,
                SensorType = sensor.SensorType.ToString(),
                Value = sensor.Value?.ToString()
            });
    
        }

        foreach (IHardware subHardware in hardware.SubHardware)
        {
            foreach (ISensor subSensor in subHardware.Sensors)
            {
                sensorList.Add(new
                {
                    Hardware = subHardware.Name,
                    SensorName = subSensor.Name,
                    SensorType = subSensor.SensorType.ToString(),
                    Value = subSensor.Value?.ToString(),

                });
                
            }
        }
        
    }

    // Konvertiere die Liste in JSON
    return JsonConvert.SerializeObject(sensorList, Formatting.Indented);
}
serialPort.ConnectionStatusChanged += delegate (object sender, ConnectionStatusChangedEventArgs args)
{
    Console.WriteLine("Connected = {0}", args.Connected);
};


static Dictionary<string, List<Dictionary<string, string>>> ParseJsonInput(string jsonInput)
{
    // Deserialisiere den JSON-String in das gewünschte Dictionary-Format
    return JsonConvert.DeserializeObject<Dictionary<string, List<Dictionary<string, string>>>>(jsonInput);
}

String inputBuffer = "";
String receiveBuffer = "";
serialPort.MessageReceived += delegate (object sender, MessageReceivedEventArgs args)
{
    // Empfangene Daten in den Empfangspuffer hinzufügen
    receiveBuffer += System.Text.Encoding.UTF8.GetString(args.Data);

    // Überprüfe, ob der Empfangspuffer eine oder mehrere vollständige Zeilen enthält
    while (receiveBuffer.Contains("\r\n"))
    {
        // Extrahiere die erste vollständige Zeile
        int endOfLineIndex = receiveBuffer.IndexOf("\r\n");
        string completeMessage = receiveBuffer.Substring(0, endOfLineIndex);

        // Entferne die verarbeitete Zeile aus dem Empfangspuffer
        receiveBuffer = receiveBuffer.Substring(endOfLineIndex + 2);

        // Nachricht in die Queue schreiben
        if (completeMessage.Length > 2)
        messageQueue.Enqueue(completeMessage.Trim());
    }
};
String comport="";
if (args.Length == 1 && args[0].Equals("help", StringComparison.OrdinalIgnoreCase))
{
    Console.WriteLine("Usage:");
    Console.WriteLine("  [COM-Port] - Specify the COM port to use (e.g., COM3).");
    Console.WriteLine("  help       - Display this help message.");
    Environment.Exit(0); // Beendet das Programm nach der Anzeige der Hilfe
}
else if (args.Length == 1)
{
    // Wenn ein Argument übergeben wurde, verwenden Sie es als COM-Port
    comport = args[0];
}else
{
    // Wenn kein Argument übergeben wurde, verwenden Sie den Standardwert
    Console.WriteLine("The PortName cannot be empty. (Parameter 'PortName')"); // Ersetzen Sie dies durch den gewünschten Standardwert
    return;
}


serialPort.SetPort(comport, 115200);

// Connect the serial port
serialPort.Connect();
computer.Open();


computer.Accept(new UpdateVisitor());
while (true)
{

     if (messageQueue.Count > 0)
    {
        inputBuffer = messageQueue.Dequeue(); // Nachricht aus der Queue lesen
        if (inputBuffer.Equals("liste", StringComparison.OrdinalIgnoreCase))
        {
            inputBuffer = "";
         
            string sensorListJson = GenerateSensorList(computer);
            serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes(sensorListJson + "\r\n"));
              Console.Write(sensorListJson + "\r\n");
        }
        else
                if (inputBuffer.StartsWith("DEBUG", StringComparison.OrdinalIgnoreCase))
        {
            Console.Write(inputBuffer + "\r\n");
            inputBuffer = "";
           
            
            //serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes(sensorListJson + "\r\n"));
             
        }
        else
            if (inputBuffer.StartsWith("{") && inputBuffer.EndsWith("}"))
        {
              computer.Accept(new UpdateVisitor());
           inputBuffer = inputBuffer.Trim('\r', '\n');
          
            // Parse JSON input
            try
            {
               // Console.WriteLine($"Received Data: {receivedString}");
                var selection = ParseJsonInput(inputBuffer);
                string filteredSensorsJson = FilterSensorsByJson(computer, selection).Replace("\r\n", String.Empty);
                serialPort.SendMessage(System.Text.Encoding.UTF8.GetBytes(filteredSensorsJson + "\r\n"));
                //Console.WriteLine(filteredSensorsJson+"\r\n");
            }
            catch (JsonReaderException ex)
            {
                Console.WriteLine($"JSON Parsing Error: {ex.Message}");
                Console.WriteLine($"Invalid JSON: {inputBuffer}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error processing input: {ex.Message}");
            }
            lastjson= inputBuffer;
            inputBuffer = "";
        }
        else
        {
            // Sonstige Outputs
            Console.WriteLine($"Invalid input: {inputBuffer}");
            inputBuffer = "";
           
        }


    }

};

// Set port options


computer.Close();


// Move these declarations to the top of the file
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

