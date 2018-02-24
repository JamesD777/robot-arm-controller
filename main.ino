// Installed libraries
#include <Thread.h>
#include <StaticThreadController.h>
#include <ArduinoHttpClient.h>

// Arm functions include
#include "arm.h"

// Standard library includes
#include <SPI.h>
#include <Ethernet.h>

// Define enable pin
#define EN 5

// Define direction pins
#define ARM_DIR 6
#define EXTENSION_DIR 8
#define CLAW_DIR 3

// Define step pins
#define ARM_STEP 7
#define EXTENSION_STEP 9
#define CLAW_STEP 4

// Global ethernet variables
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
char server[] = "echo.websocket.org";
int port = 80;

// Global position variables
double posX, posY = 0;
double lastX, lastY = 0;

// Threads for controlling arm motors
Thread *armThread = new Thread();
Thread *extensionThread = new Thread();
Thread *clawThread = new Thread();

// Websocket thread
Thread *socketThread = new Thread();

// Thread controller
StaticThreadController<4> controller(armThread, extensionThread, clawThread, socketThread);

// Websocket client
EthernetClient ethernet;
WebSocketClient client = WebSocketClient(ethernet, server, port);

void armCallback()
{
}

void extensionCallback()
{
}

void clawCallback()
{
  // TODO
}

void socketCallback()
{
  if (client.connected())
  {
    // Check if a message is available to be received
    int messageSize = client.parseMessage();

    if (messageSize > 0)
    {
      Serial.println("Received a message:");
      Serial.println(client.readString());
      
      // Parse the message to get the new x and y positions
    }
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

  const int INTERVAL = 20;

  // Configure threads
  armThread->onRun(armCallback);
  armThread->setInterval(INTERVAL);

  extensionThread->onRun(extensionCallback);
  extensionThread->setInterval(INTERVAL);

  clawThread->onRun(clawCallback);
  clawThread->setInterval(INTERVAL);

  socketThread->onRun(socketCallback);
  socketThread->setInterval(INTERVAL);

  // Start websocket client
  client.begin();
}

void loop()
{
  controller.run();

  lastX = posX;
  lastY = posY;
}
