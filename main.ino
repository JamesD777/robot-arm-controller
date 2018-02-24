// Installed libraries
#include <ArduinoHttpClient.h>

// Arm functions include
#include "arm.h"

// Standard library includes
#include <SPI.h>
#include <Ethernet.h>

// Define enable pin
#define EN 5

// Define direction pins
#define ARM_DIR 3
#define EXTENSION_DIR 8
#define CLAW_DIR 6

// Define step pins
#define ARM_STEP 4
#define EXTENSION_STEP 9
#define CLAW_STEP 7

// Global ethernet variables
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
char server[] = "echo.websocket.org";
int port = 80;

// Global position variables
int armSteps = 0;
int extensionSteps = 0;
double posX, posY = 0;
double lastX, lastY = 0;

// Websocket client
EthernetClient ethernet;
WebSocketClient client = WebSocketClient(ethernet, server, port);

void readSocket()
{
  if (client.connected())
  {
    // Check if a message is available to be received
    int messageSize = client.parseMessage();

    if (messageSize > 0)
    {
      Serial.println("Received a message:");
      Serial.println(client.readString());
      
      // Parse the message to set the new x and y positions
    }
  }
}

void stepArm() {
  if (armSteps < rotate(90)) {
    step(true, ARM_DIR, ARM_STEP, 100);
    armSteps++;
  } else {
    delay(100);
    armSteps = 0;
  }
}

void stepExtension() {
  if (extensionSteps < rotate(90)) {
    step(true, EXTENSION_DIR, EXTENSION_STEP, 100);
    extensionSteps++;
  } else {
    delay(100);
    extensionSteps = 0;
  }
}

void setup()
{
  Serial.begin(9600);

  // Initialize ip connection
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP!");
  }
  else
  {
    Serial.println(Ethernet.localIP());
  }

  pinMode(ARM_DIR, OUTPUT);
  pinMode(ARM_STEP, OUTPUT);
  pinMode(EXTENSION_DIR, OUTPUT);
  pinMode(EXTENSION_STEP, OUTPUT);
  pinMode(CLAW_DIR, OUTPUT);
  pinMode(CLAW_STEP, OUTPUT);

  pinMode(EN, OUTPUT);
  digitalWrite(EN, HIGH);

  // Start websocket client
  // client.begin();
}

void loop()
{
  //readSocket();
  stepArm();
  stepExtension();

  lastX = posX;
  lastY = posY;
}
