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
#define EXTENSION_DIR 6
#define CLAW_DIR 8

// Define step pins
#define ARM_STEP 4
#define EXTENSION_STEP 7
#define CLAW_STEP 9

#define PULSE_WIDTH 20

// Global ethernet variables
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
char server[] = "192.168.0.102";
int port = 8080;

// Global position variables
int armSteps, extensionSteps, clawSteps = 0;

double posX, posY = 0;

double currentArmAngle = 0;
double currentExtensionAngle = 180;
double currentClawAngle = 180;

double targetArmAngle = currentArmAngle;
double targetExtensionAngle = currentExtensionAngle;
double targetClawAngle = currentClawAngle;

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
      String message = client.readString();

      // Parse the message to set the new x and y positions
      char *token = strtok(message.c_str(), ",");
      double x = (double)atof(token);

      token = strtok(NULL, ",");
      double y = (double)atof(token);

      posX = x;
      posY = y;
    }
  }
}

void stepArm()
{
  const double diff = fabs(targetArmAngle - currentArmAngle);
  if (diff > 0.1)
  {
    if (targetArmAngle > currentArmAngle)
    {
      step(false, ARM_DIR, ARM_STEP, PULSE_WIDTH);
      currentArmAngle += stepsToDegrees(1) / ARM_GEAR_RATIO;
      currentExtensionAngle -= stepsToDegrees(1) / (ARM_GEAR_RATIO * ARM_GEAR_RATIO);
    }
    else if (targetArmAngle < currentArmAngle)
    {
      step(true, ARM_DIR, ARM_STEP, PULSE_WIDTH);
      currentArmAngle -= stepsToDegrees(1) / ARM_GEAR_RATIO;
      currentExtensionAngle += stepsToDegrees(1) / (ARM_GEAR_RATIO * ARM_GEAR_RATIO);
    }
  }
}

void stepExtension()
{
  const double diff = fabs(targetExtensionAngle - currentExtensionAngle);
  if (diff > 0.1)
  {
    if (targetExtensionAngle > currentExtensionAngle)
    {
      step(false, EXTENSION_DIR, EXTENSION_STEP, PULSE_WIDTH);
      currentExtensionAngle += stepsToDegrees(1) / EXTENSION_GEAR_RATIO;
      currentClawAngle += stepsToDegrees(1) / (EXTENSION_GEAR_RATIO);
    }
    else if (targetExtensionAngle < currentExtensionAngle)
    {
      step(true, EXTENSION_DIR, EXTENSION_STEP, PULSE_WIDTH);
      currentExtensionAngle -= stepsToDegrees(1) / EXTENSION_GEAR_RATIO;
      currentClawAngle -= stepsToDegrees(1) / (EXTENSION_GEAR_RATIO);
    }
  }
}

void stepClaw()
{
  const double diff = fabs(targetClawAngle - currentClawAngle);
  if (diff > 0.1)
  {
    if (targetClawAngle > currentClawAngle)
    {
      step(false, CLAW_DIR, CLAW_STEP, PULSE_WIDTH);
      currentClawAngle += stepsToDegrees(1) / CLAW_GEAR_RATIO;
    }
    else if (targetClawAngle < currentClawAngle)
    {
      step(true, CLAW_DIR, CLAW_STEP, PULSE_WIDTH);
      currentClawAngle -= stepsToDegrees(1) / CLAW_GEAR_RATIO;
    }
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

  Serial.print("arm angle: ");
  Serial.println(targetArmAngle);
  Serial.print("extension angle: ");
  Serial.println(targetExtensionAngle);

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
  client.begin();
}

void loop()
{
  readSocket();
  stepArm();
  stepExtension();
  stepClaw();

  targetArmAngle = getArmAngle(posX, posY);
  targetExtensionAngle = getExtensionAngle(posX, posY);
  targetClawAngle = getClawAngle(posX, posY);
}
