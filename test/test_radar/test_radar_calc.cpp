#include "unity.h"
#include <vector>
#include "../src/controll/Radar.hpp"

Radar radar;

void setUp(void) {}

void tearDown(void) {}

void test_obstacle_calc_one(void) {
    std::vector<float> coordenates = radar.getObstacle(2, 90);

    TEST_ASSERT_EQUAL_FLOAT(0, coordenates[0]);
    TEST_ASSERT_EQUAL_FLOAT(2, coordenates[1]);
}

void test_obstacle_calc_two(void) {
    std::vector<float> coordenates = radar.getObstacle(2, 45);

    TEST_ASSERT_EQUAL_FLOAT(1.41, coordenates[0]);
    TEST_ASSERT_EQUAL_FLOAT(1.41, coordenates[1]);
}

void test_obstacle_calc_three(void) {
    std::vector<float> coordenates = radar.getObstacle(3.4, 75);

    TEST_ASSERT_EQUAL_FLOAT(3.28, coordenates[0]);
    TEST_ASSERT_EQUAL_FLOAT(0.87, coordenates[1]);
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_obstacle_calc_one);
  RUN_TEST(test_obstacle_calc_two);
  RUN_TEST(test_obstacle_calc_three);
  return UNITY_END();
}

int main(void) {
  return runUnityTests();
}

void setup() {
  // Espera cerca de dois segundos para evitar erros
  // relacionados a sincronização do sistema
  delay(2000);

  runUnityTests();
}
void loop() {}