# SE104.O27-IoTs
Trash Management System Using ESP32 and MQTT

Equipment:
1x ESP32 NodeMCU-32S
1x MQ135 air sensor
2x US-015 ultrasonic sensor

MQTT BROKER: EMQX
Link to sign up for serverless EMQX broker :https://www.emqx.com/en/cloud/serverless-mqtt

MQTT client:
ESP32 is a publisher, publish sensor data into broker
MYSQL client is also an mqtt client,subscribe to a topic to receive data of ESP32 push data into mysql database
