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
int clawSteps = 0;

double currentArmAngle = 0;
double currentExtensionAngle = 0;
double currentClawAngle = 0;

double targetArmAngle = 45 * ARM_GEAR_RATIO;
double targetExtensionAngle = 45 * EXTENSION_GEAR_RATIO;
double targetClawAngle = 0;

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

void stepArm()
{
  if (armSteps < degreesToSteps(targetArmAngle))
  {
    boolean dir = checkAngle(currentArmAngle, targetArmAngle);
    step(dir, ARM_DIR, ARM_STEP, 100);
    armSteps++;
  }
}

void stepExtension()
{
  if (armSteps < degreesToSteps(targetExtensionAngle))
  {
    boolean dir = checkAngle(currentExtensionAngle, targetExtensionAngle);
    step(dir, EXTENSION_DIR, EXTENSION_STEP, 100);
    extensionSteps++;
  }
}

void stepClaw()
{
  if (clawSteps < degreesToSteps(targetClawAngle))
  {
    boolean dir = checkAngle(currentClawAngle, targetClawAngle);
    step(dir, CLAW_DIR, CLAW_STEP, 100);
    clawSteps++;
  }
}

/**
 * return true: rotate cw, return false: rotate ccw
 */
boolean checkAngle(double current, double target)
{
  if (current < target)
  {
    return false;
  }
  else
  {
    return true;
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
  stepClaw();

  lastX = posX;
  lastY = posY;
}
