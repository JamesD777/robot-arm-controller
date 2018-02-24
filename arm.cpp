#include "arm.h"

// Define step constraints
#define STEP_SIZE 1.8
#define MICROSTEP_MODE 32

// Arm lengths in meters
#define ARM_LENGTH 0.3048
#define ARM_EXTENSION_LENGTH 0.2032

/**
 * Input degrees to move the stepper
 */
static int rotate(int deg)
{
  return (deg / STEP_SIZE) * MICROSTEP_MODE;
}

/**
 * @param x: Meters to move the arm in the x-axis
 * @param y: Meters to move the arm in the y-axis
 */
static double getArmAngle(double x, double y)
{
  return ARM_EXTENSION_LENGTH + atan2(y, x) * 180 / M_PI;
}

/**
 * @param x: Meters to move the arm in the x-axis
 * @param y: Meters to move the arm in the y-axis
 */
static double getExtensionAngle(double x, double y)
{
  double hypotenuse = sqrt(pow(x, 2) + pow(y, 2));
  double numerator = pow(ARM_LENGTH, 2) + pow(ARM_EXTENSION_LENGTH, 2) - pow(hypotenuse, 2);
  double denominator = 2 * ARM_LENGTH * ARM_EXTENSION_LENGTH;

  return acos(numerator / denominator) * 180 / M_PI;
}

/**
 * dir: true -> step right, false -> step left
 * @param pulseWidth: Time (us) between high/low write
 */
static void step(boolean dir, byte dirPin, byte stepPin, int pulseWidth)
{
  digitalWrite(dirPin, dir);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(pulseWidth);
}
