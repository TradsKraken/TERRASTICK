#include <HardwareSerial.h>

HardwareSerial modbusSerial(2);

const int RX_PIN = 16;
const int TX_PIN = 17;
const int RE_DE_PIN = 4;

// --- Command to change the sensor's Modbus Address ---
// This command tells a sensor currently at Address 1 (0x01) 
// to change its address to Address 2 (0x02) by writing to register 0x07D0.
// Frame: [01] [06] [07 D0] [00 02] [CRC_L] [CRC_H]
const byte changeAddrCmd[] = {0x01, 0x06, 0x07, 0xD0, 0x00, 0x02, 0x08, 0x86};

byte response[8];

void setup() {
  Serial.begin(115200);
  
  // Try 4800 baud first (most common)
  modbusSerial.begin(4800, SERIAL_8N1, RX_PIN, TX_PIN);
  
  pinMode(RE_DE_PIN, OUTPUT);
  digitalWrite(RE_DE_PIN, LOW); // Start listening
  
  delay(3000); // Give you time to open the Serial Monitor
  
  Serial.println("\n==============================================");
  Serial.println("         MODBUS ADDRESS CHANGER TOOL          ");
  Serial.println("==============================================");
  Serial.println("CRITICAL: Make sure ONLY the 4-Parameter sensor is wired up!");
  Serial.println("If the NPK sensor is also connected, its address will change too!");
  Serial.println("...");
  delay(3000);
  
  Serial.println("Sending command to change address from 1 to 2...");
  
  // Switch to transmit mode
  digitalWrite(RE_DE_PIN, HIGH);
  delay(10);
  
  // Send the command
  modbusSerial.write(changeAddrCmd, sizeof(changeAddrCmd));
  modbusSerial.flush();
  
  // Switch to receive mode
  digitalWrite(RE_DE_PIN, LOW);
  delay(10);
  
  // Wait up to 1 second for a response
  int index = 0;
  unsigned long startTime = millis();
  while((millis() - startTime) < 1000) {
    if(modbusSerial.available()) {
      response[index++] = modbusSerial.read();
      if(index >= 8) break; // A successful Modbus write response is exactly 8 bytes
    }
  }
  
  Serial.println("...");
  
  if (index == 8) {
    Serial.println("✅ SUCCESS! The sensor acknowledged the command.");
    Serial.print("Raw sensor response: ");
    for(int i=0; i<8; i++) {
      Serial.print(response[i], HEX); Serial.print(" ");
    }
    Serial.println("\n");
    Serial.println("The 4-Parameter sensor is now permanently set to Address 2.");
    Serial.println("You can now wire BOTH sensors together and upload the 'terrastick_sensors.ino' file!");
  } else {
    Serial.print("❌ FAILED. The sensor did not respond properly. Bytes received: ");
    Serial.println(index);
    Serial.println("Troubleshooting:");
    Serial.println("1. Check your MAX485 wiring.");
    Serial.println("2. Make sure the sensor has 12V power and shares a ground with the ESP32.");
    Serial.println("3. The sensor might be running at 9600 baud instead of 4800. Try changing modbusSerial.begin(4800...) to 9600.");
  }
}

void loop() {
  // We only need to run this once, so the loop is empty.
}
