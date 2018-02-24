#ifndef ARM_H
#define ARM_H

#include "lib/math.h"

static double rotate(double deg);
static double getArmAngle(double x, double y);
static double getExtensionAngle(double x, double y);
static void step(boolean dir, byte dirPin, byte stepPin, int steps, int pulseWidth);

#endif
