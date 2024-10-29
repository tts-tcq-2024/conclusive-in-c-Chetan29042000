#include <gtest/gtest.h>
#include "typewise-alert.h"

TEST(TypeWiseAlertTestSuite, InfersBreachAccordingToLimits) {
  EXPECT_EQ(inferBreach(25, 20, 30), NORMAL);
  EXPECT_EQ(inferBreach(15, 20, 30), TOO_LOW);
  EXPECT_EQ(inferBreach(35, 20, 30), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreachWithPassiveCooling) {
  EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 20), NORMAL);
  EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 36), TOO_HIGH);
  EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, -1), TOO_LOW);
}

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreachWithHiActiveCooling) {
  EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 20), NORMAL);
  EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46), TOO_HIGH);
  EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, -1), TOO_LOW);
}

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreachWithMedActiveCooling) {
  EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 20), NORMAL);
  EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41), TOO_HIGH);
  EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1), TOO_LOW);
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToController) {
  BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
  testing::internal::CaptureStdout();
  checkAndAlert(TO_CONTROLLER, batteryChar, 36);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "feed : 2\n"); // 2 is TOO_HIGH
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToEmailTooLow) {
  BatteryCharacter batteryChar = {HI_ACTIVE_COOLING, "BrandY"};
  testing::internal::CaptureStdout();
  checkAndAlert(TO_EMAIL, batteryChar, -5);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too low\n");
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToEmailTooHigh) {
  BatteryCharacter batteryChar = {MED_ACTIVE_COOLING, "BrandZ"};
  testing::internal::CaptureStdout();
  checkAndAlert(TO_EMAIL, batteryChar, 50);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToEmailNormal) {
  BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
  testing::internal::CaptureStdout();
  checkAndAlert(TO_EMAIL, batteryChar, 25);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "");
}
