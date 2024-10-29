#include "typewise-alert.h"
#include <stdio.h>

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if (value < lowerLimit) {
    return TOO_LOW;
  }
  if (value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

void getCoolingLimits(CoolingType coolingType, int& lowerLimit, int& upperLimit) {
  lowerLimit = 0;
  switch (coolingType) {
    case PASSIVE_COOLING: upperLimit = 35; break;
    case HI_ACTIVE_COOLING: upperLimit = 45; break;
    case MED_ACTIVE_COOLING: upperLimit = 40; break;
    default: upperLimit = 35; // Default for unrecognized types
  }
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
  int lowerLimit, upperLimit;
  getCoolingLimits(coolingType, lowerLimit, upperLimit);
  return inferBreach(temperatureInC, lowerLimit, upperLimit);
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);

  if (alertTarget == TO_CONTROLLER) {
    sendToController(breachType);
  } else if (alertTarget == TO_EMAIL) {
    sendToEmail(breachType);
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
  const char* recipient = "a.b@c.com";
  const char* breachMessages[] = {
    "Hi, the temperature is normal\n",
    "Hi, the temperature is too low\n",
    "Hi, the temperature is too high\n"
  };
  
  if (breachType != NORMAL) {
    printf("To: %s\n", recipient);
    printf("%s", breachMessages[breachType]);
  }
}
