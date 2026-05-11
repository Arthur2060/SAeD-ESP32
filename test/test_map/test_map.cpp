#include <Arduino.h>
#include "unity.h"
#include <vector>
#include "../src/controll/MapManager.hpp"

MapManager map(10, 10, 0.3);

void setUp(void) {
    map.setCurrentCell(5, 5);
}

void tearDown(void) {}

void test_obstacle_calc_one(void) {
    TEST_ASSERT_TRUE(map.addObstacle(0, 1));
}

void test_obstacle_calc_two(void) {
    TEST_ASSERT_TRUE(map.addObstacle(1, 1));
}

void test_obstacle_calc_three(void) {
    TEST_ASSERT_TRUE(map.addObstacle(1.2, 1.5));
}

void test_obstacle_calc_fuor(void) {
    TEST_ASSERT_FALSE(map.addObstacle(3, 2));
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_obstacle_calc_one);
  RUN_TEST(test_obstacle_calc_two);
  RUN_TEST(test_obstacle_calc_three);
  RUN_TEST(test_obstacle_calc_fuor);
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