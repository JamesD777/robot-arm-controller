#include "arm.h"

// Define step constraints
#define STEP_SIZE 1.8
#define MICROSTEP_MODE 32

// Arm lengths in meters
#define ARM_LENGTH 0.2794
#define ARM_EXTENSION_LENGTH 0.2794

const double ARM_GEAR_RATIO = 111.0 / 32.0;
const double EXTENSION_GEAR_RATIO = 10.0 / 40.0;

/**
 * Input degrees to move the stepper
 */
static int degreesToSteps(double deg)
{
    return (deg / STEP_SIZE) * MICROSTEP_MODE;
}

static double stepsToDegrees(int steps) {
  return (steps / MICROSTEP_MODE) * STEP_SIZE;
}

/**
 * @param x: Meters to move the arm in the x-axis
 * @param y: Meters to move the arm in the y-axis
 */
static double getArmAngle(double x, double y)
{
  return (ARM_EXTENSION_LENGTH + atan2(y, x) * 180 / M_PI) * ARM_GEAR_RATIO;
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

  return (acos(numerator / denominator) * 180 / M_PI) * EXTENSION_GEAR_RATIO;
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
