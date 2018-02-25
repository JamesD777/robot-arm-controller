#include "arm.h"

// Define step constraints
#define STEP_SIZE 1.8
#define MICROSTEP_MODE 32.0

// Arm lengths in meters
#define ARM_LENGTH 0.25
#define EXTENSION_LENGTH 0.18

const double ARM_GEAR_RATIO = 111.0 / 32.0;
const double EXTENSION_GEAR_RATIO = 3.3;
const double CLAW_GEAR_RATIO = 3.3;

/**
 * Input degrees to move the stepper
 */
static int degreesToSteps(double deg)
{
  deg = fabs(deg);
  return (deg / STEP_SIZE) * MICROSTEP_MODE;
}

static double stepsToDegrees(int steps)
{
  return ((double)steps / MICROSTEP_MODE) * STEP_SIZE;
}

/**
 * @param x: Meters to move the arm in the x-axis
 * @param y: Meters to move the arm in the y-axis
 */
static double getArmAngle(double x, double y)
{

  // double hypotenuse = sqrt(x * x + y * y);
  // double numerator =  EXTENSION_LENGTH * EXTENSION_LENGTH + hypotenuse * hypotenuse - ARM_LENGTH * ARM_LENGTH;
  // double denominator = 2 * hypotenuse * EXTENSION_LENGTH;

  // return (acos(numerator / denominator) + atan2(y, x)) * 180 / M_PI;

  double hypotenuse = sqrt(x * x + y * y);
  return (atan2(EXTENSION_LENGTH, hypotenuse) + atan2(y, x)) * 180 / M_PI;


}

/**
 * @param x: Meters to move the arm in the x-axis
 * @param y: Meters to move the arm in the y-axis
 */
static double getExtensionAngle(double x, double y)
{
  double hypotenuse = sqrt(x * x + y * y);
  double numerator = ARM_LENGTH * ARM_LENGTH + EXTENSION_LENGTH * EXTENSION_LENGTH - hypotenuse * hypotenuse;
  double denominator = 2 * ARM_LENGTH * EXTENSION_LENGTH;

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
