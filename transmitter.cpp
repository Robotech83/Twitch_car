#include <RH_NRF24.h>
#include <SPI.h>

RH_NRF24 nrf24(9, 10);  // CE, CSN pins

void setup() {
  Serial.begin(9600);
  while (!Serial);  // Wait for the serial port to be ready

  if (!nrf24.init()) {
    Serial.println("Initialization failed");
    while (1);
  }
  // Set channel and RF settings here if needed
}

void loop() {
  if (Serial.available()) {
    char command[20] = "";
    Serial.readBytesUntil('\n', command, sizeof(command) - 1);  // Leave space for null terminator

    Serial.print("Sending command: ");
    Serial.println(command);
    
    if (nrf24.send((uint8_t *)command, strlen(command) + 1)) {  // +1 for null character
      nrf24.waitPacketSent();
      Serial.println("Command sent successfully");
    } else {
      Serial.println("Failed to send command");
    }
  }
}
// this is an test
