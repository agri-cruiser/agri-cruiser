#include <esp_now.h>
#include <WiFi.h>
#include "RoboClaw.h"
#include <HardwareSerial.h>

#define RELAY_PIN1  5  // Change this to the GPIO you're using
#define RELAY_PIN2  18  // Change this to the GPIO you're using
#define RELAY_PIN3  19  // Change this to the GPIO you're using
#define RELAY_PIN4  21  // Change this to the GPIO you're using

#define ROBOCLAW_ADDR 0x80

HardwareSerial serial2(2);  // RX=16, TX=17
RoboClaw roboclaw(&serial2, 10000);
uint8_t receiverAddress[] = { 0xAC, 0x15, 0x18, 0xD8, 0xD0, 0xD8 };


typedef struct struct_message {
  int speed1;
  int speed2;
  bool solenoid1;
  bool solenoid2;
  bool solenoid3;
  bool solenoid4;
} struct_message;

typedef struct struct_message1 {
  int enc1;
  int enc2;
  float voltage;
  float speedL;
  float speedR;
} struct_message1;

struct_message1 sendingData;

struct_message incomingData;
const int ledPin = 2;
#define CPR 4000  // 1000 lines * 4 (quadrature)

int MAX_POWER = 35000;
int MAX_TURN = 35000;

int MAX_PWM_F = 64;
int MAX_PWM_T = 32; 

// CRSF buffer
uint8_t crsfBuffer[64];
int crsfPos = 0;

void onDataRecv(const uint8_t *mac, const uint8_t *incomingDataRaw, int len) {
  memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));

  Serial.printf("M1: %d, M2: %d\n", incomingData.speed1, incomingData.speed2);
  // digitalWrite(ledPin, !digitalRead(ledPin));

  if(incomingData.speed1 < 0){
    // roboclaw.BackwardM1(ROBOCLAW_ADDR, -incomingData.speed1);
    roboclaw.SpeedM1(ROBOCLAW_ADDR, incomingData.speed1);
  }else{
    // roboclaw.ForwardM1(ROBOCLAW_ADDR, incomingData.speed1);
    roboclaw.SpeedM1(ROBOCLAW_ADDR, incomingData.speed1);
  }
  if(incomingData.speed2 < 0){
    // roboclaw.BackwardM2(ROBOCLAW_ADDR, -incomingData.speed2);
    roboclaw.SpeedM2(ROBOCLAW_ADDR, incomingData.speed2);
  }else{
    // roboclaw.ForwardM2(ROBOCLAW_ADDR, incomingData.speed2);
    roboclaw.SpeedM2(ROBOCLAW_ADDR, incomingData.speed2);
  }

  if(incomingData.speed1 == 0){
    roboclaw.ForwardM1(ROBOCLAW_ADDR, 0);
    // roboclaw.BackwardM1(ROBOCLAW_ADDR, -incomingData.speed1);
    // roboclaw.SpeedM1(ROBOCLAW_ADDR, incomingData.speed1);
  }
  if(incomingData.speed2 == 0){
    // roboclaw.BackwardM1(ROBOCLAW_ADDR, -incomingData.speed1);
    roboclaw.ForwardM2(ROBOCLAW_ADDR, 0);
  }
  // roboclaw.ForwardM2(ROBOCLAW_ADDR, incomingData.speed2);
  // Read encoder values
  uint32_t enc1, enc2;
  bool valid1, valid2;
  enc1 = roboclaw.ReadEncM1(ROBOCLAW_ADDR, NULL, &valid1);
  enc2 = roboclaw.ReadEncM2(ROBOCLAW_ADDR, NULL, &valid2);

  bool valid3;
  uint16_t volts = roboclaw.ReadMainBatteryVoltage(ROBOCLAW_ADDR);

  struct_message1 telemetry;
  telemetry.enc1 = valid1 ? enc1 : -1;
  telemetry.enc2 = valid2 ? enc2 : -1;

  // Serial.print("Battery Voltage: ");
  // Serial.println(volts / 10.0);

  float x = volts/10.0;

  telemetry.voltage = x;

  if(incomingData.solenoid1){
    digitalWrite(RELAY_PIN1, HIGH);  // Turn relay OFF (active-low relay)
  }
  else{
    digitalWrite(RELAY_PIN1, LOW);
  }
  if(incomingData.solenoid2){
    digitalWrite(RELAY_PIN2, HIGH);  // Turn relay OFF (active-low relay)
  }
  else{
    digitalWrite(RELAY_PIN2, LOW);
  }
  if(incomingData.solenoid3){
    digitalWrite(RELAY_PIN3, HIGH);  // Turn relay OFF (active-low relay)
  }
  else{
    digitalWrite(RELAY_PIN3, LOW);
  }
  if(incomingData.solenoid4){
    digitalWrite(RELAY_PIN4, HIGH);  // Turn relay OFF (active-low relay)
  }
  else{
    digitalWrite(RELAY_PIN4, LOW);
  }

  bool valid4, valid5;
  int32_t speed1 = roboclaw.ReadSpeedM1(ROBOCLAW_ADDR,NULL, &valid4);  // counts/sec
  int32_t speed2 = roboclaw.ReadSpeedM2(ROBOCLAW_ADDR,NULL, &valid5);  // counts/sec

  telemetry.speedL = speed1;
  telemetry.speedR = speed2;

  // Send back to sender
  esp_now_send(mac, (uint8_t *)&telemetry, sizeof(telemetry));
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(420000, SERIAL_8N1, 22, 23); // CRSF UART

  // pinMode(ledPin, OUTPUT);
  // digitalWrite(ledPin, LOW);

  pinMode(RELAY_PIN1, OUTPUT);     // Set the relay pin as output
  digitalWrite(RELAY_PIN1, LOW);  // Turn relay OFF (active-low relay)

  pinMode(RELAY_PIN2, OUTPUT);     // Set the relay pin as output
  digitalWrite(RELAY_PIN2, LOW);  // Turn relay OFF (active-low relay)

  pinMode(RELAY_PIN3, OUTPUT);     // Set the relay pin as output
  digitalWrite(RELAY_PIN3, LOW);  // Turn relay OFF (active-low relay)

  pinMode(RELAY_PIN4, OUTPUT);     // Set the relay pin as output
  digitalWrite(RELAY_PIN4, LOW);  // Turn relay OFF (active-low relay)

  serial2.begin(38400, SERIAL_8N1, 16, 17);
  roboclaw.begin(38400);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // esp_now_register_recv_cb(onDataRecv);

  uint16_t volts = roboclaw.ReadMainBatteryVoltage(ROBOCLAW_ADDR);
  Serial.print("Battery Voltage: ");
  Serial.println(volts / 10.0);
}

void loop() {
  while (Serial1.available()) {
    uint8_t byte = Serial1.read();

    if (crsfPos == 0 && byte != 0xC8) continue;

    crsfBuffer[crsfPos++] = byte;

    if (crsfPos > 2 && crsfPos >= crsfBuffer[1] + 2) {
      parseCRSFPacket(crsfBuffer, crsfPos);
      crsfPos = 0;
    }
  }
  delay(10);
}


void parseCRSFPacket(uint8_t *data, int len) {
  if (data[2] == 0x16) { // RC Channels frame
    uint16_t channels[16];
    uint8_t *p = &data[3];

    channels[0] = ((p[0] | p[1] << 8) & 0x07FF);
    channels[1] = ((p[1] >> 3 | p[2] << 5) & 0x07FF);
    channels[2] = ((p[2] >> 6 | p[3] << 2 | p[4] << 10) & 0x07FF);
    channels[3] = ((p[4] >> 1 | p[5] << 7) & 0x07FF);
    channels[4] = ((p[5] >> 4 | p[6] << 4) & 0x07FF);
    channels[5] = ((p[6] >> 7 | p[7] << 1 | p[8] << 9) & 0x07FF);
    channels[6] = ((p[8] >> 2 | p[9] << 6) & 0x07FF);
    channels[7] = ((p[9] >> 5 | p[10] << 3) & 0x07FF);
    channels[8]  = ((p[11] | p[12] << 8) & 0x07FF);
    channels[9]  = ((p[12] >> 3 | p[13] << 5) & 0x07FF);
    channels[10] = ((p[13] >> 6 | p[14] << 2 | p[15] << 10) & 0x07FF);


    float ch1 = ((channels[1] - 992.0f) / 820.0f); // Throttle
    float ch2 = ((channels[3] - 992.0f) / 820.0f); // Steering

    // Clamp values to -1.0 to 1.0
    ch1 = constrain(ch1, -1.0f, 1.0f);
    ch2 = constrain(ch2, -1.0f, 1.0f);

    // Optionally: Apply deadzone
    float deadzone = 0.05;
    if (fabs(ch1) < deadzone) ch1 = 0;
    if (fabs(ch2) < deadzone) ch2 = 0;

    float forward = ch1 * MAX_POWER;
    float turn = -ch2 * MAX_TURN;

    float forwardPWM = ch1 * MAX_PWM_F;
    float turnPWM = -ch2 * MAX_PWM_T;

    int leftPowerPWM = forwardPWM + turnPWM;
    int rightPowerPWM = forwardPWM - turnPWM;

    if(leftPowerPWM < 0){
      roboclaw.BackwardM1(ROBOCLAW_ADDR, -leftPowerPWM);
      // roboclaw.SpeedM1(ROBOCLAW_ADDR, incomingData.speed1);
    }else{
      roboclaw.ForwardM1(ROBOCLAW_ADDR, leftPowerPWM);
      // roboclaw.SpeedM1(ROBOCLAW_ADDR, incomingData.speed1);
    }
    if(rightPowerPWM < 0){
      roboclaw.BackwardM2(ROBOCLAW_ADDR, -rightPowerPWM);
      // roboclaw.SpeedM2(ROBOCLAW_ADDR, incomingData.speed2);
    }else{
      roboclaw.ForwardM2(ROBOCLAW_ADDR, rightPowerPWM);
      // roboclaw.SpeedM2(ROBOCLAW_ADDR, incomingData.speed2);
    }

    // int leftPower = forward + turn;
    // int rightPower = forward - turn;

    // Serial.printf("L: %d, R: %d\n", leftPower, rightPower);

    // roboclaw.SpeedM1(ROBOCLAW_ADDR, leftPower);
    // roboclaw.SpeedM2(ROBOCLAW_ADDR, rightPower);
    
    //  if(leftPower == 0){
    //    roboclaw.ForwardM1(ROBOCLAW_ADDR, 0);
    //  }
    //  if(rightPower == 0){
    //    roboclaw.ForwardM2(ROBOCLAW_ADDR, 0);
    //  }


    if (channels[6] > 1500) {  // Or adjust threshold as needed
      Serial.println("Active 1");
      // outgoingData.solenoid1 = true;
      digitalWrite(RELAY_PIN1, HIGH);  // Turn relay OFF (active-low relay)
    } else {
      Serial.println("NOT ACTIVE 1");
      // outgoingData.solenoid1 = false;
      digitalWrite(RELAY_PIN1, LOW);  // Turn relay OFF (active-low relay)
    }
    if (channels[7] > 1500) {  // Or adjust threshold as needed
      Serial.println("Active 2");
      // outgoingData.solenoid2 = true;
      digitalWrite(RELAY_PIN2, HIGH);  // Turn relay OFF (active-low relay)
    } else {
      Serial.println("NOT ACTIVE 2");
      // outgoingData.solenoid2 = false;
      digitalWrite(RELAY_PIN2, LOW);  // Turn relay OFF (active-low relay)
    }
    if (channels[8] > 1500) {  // Or adjust threshold as needed
      Serial.println("Active 3");
      // outgoingData.solenoid3 = true;
      digitalWrite(RELAY_PIN3, HIGH);  // Turn relay OFF (active-low relay)
    } else {
      Serial.println("NOT ACTIVE 3");
      // outgoingData.solenoid3 = false;
      digitalWrite(RELAY_PIN3, LOW);  // Turn relay OFF (active-low relay)
    }
    if (channels[9] > 1500) {  // Or adjust threshold as needed
      Serial.println("Active 4");
      // outgoingData.solenoid4 = true;
      digitalWrite(RELAY_PIN4, HIGH);  // Turn relay OFF (active-low relay)
    } else {
      Serial.println("NOT ACTIVE 4");
      // outgoingData.solenoid4 = false;
      digitalWrite(RELAY_PIN4, LOW);  // Turn relay OFF (active-low relay)
    }
  }
}
