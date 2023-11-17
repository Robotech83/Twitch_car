#include <RH_NRF24.h>
#include <SPI.h>

// Create an instance of the radio driver
RH_NRF24 nrf24(9, 10);  // CE, CSN

// Motor pins for the left and right motors
const int leftMotorPin1 = 5;
const int leftMotorPin2 = 6;
const int rightMotorPin1 = 7;
const int rightMotorPin2 = 8;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for the serial port to be ready

  Serial.println("Initializing NRF24 Receiver...");

  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  if (!nrf24.init()) {
    Serial.println("NRF24 Initialization failed");
    while (1)
      ;
  } else {
    Serial.println("NRF24 Initialization succeeded");
  }

  if (!nrf24.setChannel(1)) {
    Serial.println("Setting channel failed");
    while (1)
      ;
  } else {
    Serial.println("Channel set successfully");
  }

  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
    Serial.println("Setting RF configuration failed");
    while (1)
      ;
  } else {
    Serial.println("RF configuration successful");
  }

  const char *readyMessage = "Ready to Receive";
  nrf24.send((uint8_t *)readyMessage, strlen(readyMessage) + 1);  // +1 for null character
  nrf24.waitPacketSent();
  Serial.println("Sent ready message to transmitter.");
  Serial.println("Receiver setup complete");
  stopRobot();
  
}

void loop() {
  if (nrf24.available()) {
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (nrf24.recv(buf, &len)) {
      buf[len] = 0; // Null-terminate the string

      if (isValidCommand((char*)buf)) {
        Serial.print("Received valid command: ");
        Serial.println((char*)buf);

        if (strcmp((char*)buf, "!forward") == 0) {
          Serial.println("Moving forward");
          moveForward();
        } else if (strcmp((char*)buf, "!backward") == 0) {
          Serial.println("Moving backward");
          moveBackward();
        } else if (strcmp((char*)buf, "!left") == 0) {
          Serial.println("Turning left");
          turnLeft();
        } else if (strcmp((char*)buf, "!right") == 0) {
          Serial.println("Turning right");
          turnRight();
        } else if (strcmp((char*)buf, "!stop") == 0) {
          Serial.println("Stopping robot");
          stopRobot();
        }
      } else {
        Serial.println("Invalid command received");
      }
    } else {
      Serial.println("Failed to receive data");
    }
  }
}


    bool isValidCommand(const char *command) {
      // Check if command starts with '!'
      if (command[0] != '!') {
        Serial.println("Invalid command format.");
        return false;
      }
      return true;
    }

    void moveForward() {
      digitalWrite(leftMotorPin1, HIGH);
      digitalWrite(leftMotorPin2, LOW);
      digitalWrite(rightMotorPin1, HIGH);
      digitalWrite(rightMotorPin2, LOW);
    }

    void moveBackward() {
      digitalWrite(leftMotorPin1, LOW);
      digitalWrite(leftMotorPin2, HIGH);
      digitalWrite(rightMotorPin1, LOW);
      digitalWrite(rightMotorPin2, HIGH);
    }

    void turnLeft() {
      digitalWrite(leftMotorPin1, LOW);
      digitalWrite(leftMotorPin2, HIGH);
      digitalWrite(rightMotorPin1, HIGH);
      digitalWrite(rightMotorPin2, LOW);
    }

    void turnRight() {
      digitalWrite(leftMotorPin1, HIGH);
      digitalWrite(leftMotorPin2, LOW);
      digitalWrite(rightMotorPin1, LOW);
      digitalWrite(rightMotorPin2, HIGH);
    }

    void stopRobot() {
      digitalWrite(leftMotorPin1, LOW);
      digitalWrite(leftMotorPin2, LOW);
      digitalWrite(rightMotorPin1, LOW);
      digitalWrite(rightMotorPin2, LOW);
    }
// this need more testing
