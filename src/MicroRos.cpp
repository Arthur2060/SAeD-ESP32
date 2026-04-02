#include <micro_ros_arduino.h>
#include <Wire.h>
#include <WiFi.h>

#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rcl/error_handling.h>

#include <std_msgs/msg/float32.h>
#include <geometry_msgs/msg/vector3.h>

// WiFi credentials
char *ssid = "YOUR_WIFI_SSID";
char *password = "YOUR_WIFI_PASSWORD";

// micro-ROS agent IP and port
char *agent_ip_str = "192.168.1.100"; // Replace with your micro-ROS agent IP
uint agent_port = 8888;

rcl_publisher_t lidar_publisher;
rcl_publisher_t compass_publisher;
std_msgs__msg__Float32 lidar_msg;
std_msgs__msg__Float32 compass_msg;
rclc_executor_t executor;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;

void loop()
{
  delay(100);
}

class MicroRos
{
public:
  MicroRos()
  {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    set_microros_wifi_transports(ssid, password, agent_ip_str, agent_port);

    delay(2000);

    allocator = rcl_get_default_allocator();

    while (rclc_support_init(&support, 0, NULL, &allocator) != RCL_RET_OK)
    {
      Serial.println("Failed to init micro-ROS support");
      delay(1000);
    }

    while (rclc_node_init_default(&node, "esp32_sensor_node", "", &support) != RCL_RET_OK)
    {
      Serial.println("Failed to init micro-ROS node");
      delay(1000);
    }

    while (rclc_publisher_init_default(
               &lidar_publisher,
               &node,
               ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
               "radar/distance") != RCL_RET_OK)
    {
      Serial.println("Failed to create LiDAR publisher");
      delay(1000);
    }

    while (rclc_publisher_init_default(
               &compass_publisher,
               &node,
               ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Vector3),
               "radar/magnetic_field") != RCL_RET_OK)
    {
      Serial.println("Failed to create Compass publisher");
      delay(1000);
    }

    while (rclc_executor_init(&executor, &support.context, 0, &allocator) != RCL_RET_OK)
    {
      Serial.println("Failed to create micro-ROS executor");
      delay(1000);
    }

    Serial.println("micro-ROS initialized successfully!");
  }

  void sendData(float lidar, float bussola);

private:
};

void MicroRos::sendData(float lidar, float bussola)
{
  lidar_msg.data = lidar;
  compass_msg.data = bussola;
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
}