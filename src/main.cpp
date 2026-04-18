#include <Arduino.h>
#include "ros2arduino.h"
#include "Ultrasom.hpp"
#include "Bussola.hpp"
#include <vector>

std::vector<char> commands;

void subscribeCommand(std_msgs::String* msg, void* args) {
  (void)(args);

  for (char command : msg->data) {
    commands.push_back(command);
  }
}

class CommandSub : public ros2::Node 
{
  public:
    CommandSub() : Node("command_sub")
    {
      ros2::Subscriber<std_msgs::String>* subscriber_ = 
      this->createSubscriber<std_msgs::String>("commands", (ros2::CallbackFunc) subscribeCommand, nullptr);
    }

  private:
};

Bussola bussola;
Ultrasom ultrasom(14, 12); // Pino 14 - Trigger, Pino 12 - Echo

void setup() {
    Serial.begin(9600);
}

void loop() {
    static CommandSub command;

    int distance = ultrasom.collectUltrasonicData();
    int angle = bussola.collectCompassData();
    
    Serial.printf("Distance: %i, Angle: %.2f\n", distance, angle);
    ros2::spin(&command);
}