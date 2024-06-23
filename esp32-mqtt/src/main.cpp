#include <Arduino.h>
#include<WiFi.h>
#include<WiFiManager.h>
#include<WiFiClientSecure.h>
#include<PubSubClient.h>
#include<ArduinoJson.h>
#include <MQUnifiedsensor.h>
const int left_trig=33;
const int left_echo=25;
const int right_trig=18;
const int right_echo=19;
#define board "ESP-32"
#define Voltage_Resolution 3.3 
#define ADC_Bit_Resolution 12
#define pin 35
#define type "MQ-135"
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm
MQUnifiedsensor MQ135(board, Voltage_Resolution,ADC_Bit_Resolution,pin, type);
void setup_mq135()
{
    //CO2      | 110.47 | -2.862
    MQ135.setRegressionMethod(1);
    MQ135.init();
    Serial.print("Calibrating please wait.");
    float calcR0 = 0;
    for(int i = 1; i<=10; i ++)
    {
        MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
        calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
        Serial.print(".");
    }
    MQ135.setR0(calcR0/10);
}
float read_C02_concertration()
{
    MQ135.update();
    MQ135.setA(110.47); 
    MQ135.setB(-2.862);
    return MQ135.readSensor();
}
// ultrasonic sensor
double trash_amount(int trig,int echo)
{
    unsigned long duration;
    digitalWrite(trig, LOW); 
    delayMicroseconds(2); 
    digitalWrite(trig, HIGH);
    delayMicroseconds(5); 
    digitalWrite(trig, LOW);
    duration = pulseIn(echo, HIGH);
    double distance = ((double)(duration/2) / 29.412);
    return distance;
}
const char* mqtt_server ="oe9219e1.ala.eu-central-1.emqxsl.com";
const uint16_t mqtt_port = 8883;
const char *mqtt_username = "thinh";
const char *mqtt_password = "6wbF6MbDLF8fEFT"; 
const char *ca_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=
-----END CERTIFICATE-----
)EOF";
WiFiClientSecure espClient;
PubSubClient client(espClient);
WiFiManager wm;
String sensor_topic,client_id,sensor_output;
JsonDocument sensor_doc;
void setup()
{
    Serial.begin(9600);
    if(!wm.autoConnect("ESP32-WiFi-Manager","123456789"))
    {
        Serial.println("Failed to connect to wifi");
        ESP.restart();
    }
    Serial.println("Connected to wifi "); 
    client_id="5ff3a1ba-e8f5-43d0-8f7a-ff38d9cb6e30";
    sensor_topic="client/"+client_id+"/sensor";
    espClient.setCACert(ca_cert);
    client.setServer(mqtt_server,mqtt_port);
    client.connect(client_id.c_str(),mqtt_username,mqtt_password);
    pinMode(left_trig, OUTPUT);
    pinMode(left_echo, INPUT);
    pinMode(right_trig, OUTPUT);
    pinMode(right_echo, INPUT);
    setup_mq135();
}
void reconnect() 
{
  while (!client.connected()) 
  {
      Serial.print("Attempting MQTT connection...");
      if(client.connect(client_id.c_str(),mqtt_username, mqtt_password))
      {
        Serial.println("connected");
      } 
      else 
      {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
  }
}

void loop() 
{
    if(!client.connected())
    {
        reconnect();
    }
    Serial.println("Publishing message");
    sensor_doc["id"]=client_id;
    sensor_doc["ip"]=WiFi.localIP();
    sensor_doc["air_val"]=read_C02_concertration();
    sensor_doc["left_status"]=trash_amount(left_trig,left_echo);
    sensor_doc["right_status"]=trash_amount(right_trig,right_echo);
    sensor_doc["status"]="connected";
    Serial.print("CO2 ");
    Serial.println(read_C02_concertration());
    Serial.print("left ");
    Serial.println(trash_amount(left_trig,left_echo));
    Serial.print("right ");
    Serial.println(trash_amount(right_trig,right_echo));
    serializeJson(sensor_doc,sensor_output);
    client.publish(sensor_topic.c_str(),sensor_output.c_str());
    delay(5000);
}


