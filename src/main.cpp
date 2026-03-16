#include <micro_ros_arduino.h>
#include <Adafruit_QMC5883P.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>

#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rcl/error_handling.h>

#include <std_msgs/msg/float32.h>
#include <geometry_msgs/msg/vector3.h>

#define LiDAR_DATA_LENGTH 9

// WiFi credentials
char* ssid = "YOUR_WIFI_SSID";
char* password = "YOUR_WIFI_PASSWORD";

// micro-ROS agent IP and port
char* agent_ip_str = "192.168.1.100"; // Replace with your micro-ROS agent IP
uint agent_port = 8888;

Adafruit_QMC5883P compass;

rcl_publisher_t lidar_publisher;
rcl_publisher_t compass_publisher;
std_msgs__msg__Float32 lidar_msg;
geometry_msgs__msg__Vector3 compass_msg;
rclc_executor_t executor;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;

bool testCompass();
void configureCompass();
void collectCompassData();
void collectLiDARData();

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  configureCompass();
  Serial.println("Everything good to start!");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure micro-ROS transport
  set_microros_wifi_transports(ssid, password, agent_ip_str, agent_port);

  delay(2000);

  allocator = rcl_get_default_allocator();

  // create init_options
  while (rclc_support_init(&support, 0, NULL, &allocator) != RCL_RET_OK) {
    Serial.println("Failed to init micro-ROS support");
    delay(1000);
  }

  // create node
  while (rclc_node_init_default(&node, "esp32_sensor_node", "", &support) != RCL_RET_OK) {
    Serial.println("Failed to init micro-ROS node");
    delay(1000);
  }

  // create lidar publisher
  while (rclc_publisher_init_default(
    &lidar_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
    "radar/distance") != RCL_RET_OK) {
    Serial.println("Failed to create LiDAR publisher");
    delay(1000);
  }

  // create compass publisher
  while (rclc_publisher_init_default(
    &compass_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Vector3),
    "radar/magnetic_field") != RCL_RET_OK) {
    Serial.println("Failed to create Compass publisher");
    delay(1000);
  }

  // create executor (0 handles as publishers are not added to executor)
  while (rclc_executor_init(&executor, &support.context, 0, &allocator) != RCL_RET_OK) {
    Serial.println("Failed to create micro-ROS executor");
    delay(1000);
  }

  Serial.println("micro-ROS initialized successfully!");
}

void loop() {
  collectLiDARData();
  if (testCompass()) {
    collectCompassData();
  }
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
  delay(100);
}

void configureCompass() {
  if (!testCompass()) {
    Serial.println("Error, compass not found!");
  }

  compass.setMode(QMC5883P_MODE_NORMAL);
  
  compass.setODR(QMC5883P_ODR_50HZ);
  compass.setOSR(QMC5883P_OSR_4);
  compass.setDSR(QMC5883P_DSR_2);
  compass.setRange(QMC5883P_RANGE_8G);
  compass.setSetResetMode(QMC5883P_SETRESET_ON);
}

bool testCompass() {
  if (!compass.begin()) {
    return false;
  }
  return true;
}

void collectLiDARData() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX2=16, TX2=17
  //------------------------------------------------------//

  // Prepares variables for LiDAR`s data
  uint8_t data[LiDAR_DATA_LENGTH] = { 0 };

  uint16_t distance = 0;
  uint16_t strength = 0;
  uint16_t temperature = 0;

  int checksum = 0;
  int index = 0;

  // Calculate the brute data from
  // LiDAR for good use
  while(Serial2.available() && index < LiDAR_DATA_LENGTH) {
    data[index++] = Serial2.read();
  }

  if (index == LiDAR_DATA_LENGTH) {
    distance = data[2] + data[3] * 256;
    strength = data[4] + data[5] * 256;
    temperature = data[6] + data[7] * 256;

    // Publish LiDAR distance
    lidar_msg.data = (float)distance;
    rcl_publish(&lidar_publisher, &lidar_msg, NULL);
  }

  Serial.printf("Distance: %f cm\n", (float)distance);
}

void collectCompassData() {
  if (compass.isDataReady()) {
    int16_t x, y, z;
    compass.getRawMagnetic(&x, &y, &z);
    
    // Publish Compass data
    compass_msg.x = (double)x;
    compass_msg.y = (double)y;
    compass_msg.z = (double)z;
    rcl_publish(&compass_publisher, &compass_msg, NULL);

    Serial.printf("X: %f, Y: %f, Z: %f\n", (float)x, (float)y, (float)z);
  }
}