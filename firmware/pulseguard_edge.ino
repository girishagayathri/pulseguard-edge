
/*
 * PulseGuard Edge
 * ESP32 + MAX30102 + MPU6050
 * Starter: PPG sampling, motion screening, basic tri-state output
 *
 * Libraries:
 * - SparkFun MAX3010x Sensor Library
 * - I2Cdevlib MPU6050 library
 */

#include <Wire.h>
#include <MPU6050.h>
#include "MAX30105.h"

#define SDA_PIN       21
#define SCL_PIN       22
#define BUZZER_PIN    26
#define LED_PIN       25

MPU6050 mpu;
MAX30105 ppg;

const uint32_t SAMPLE_INTERVAL_MS = 20;  // 50 Hz polling target
const uint32_t ALERT_CONFIRM_MS = 5000;  // demo only; tune during validation

uint32_t lastSample = 0;
uint32_t candidateSince = 0;

enum State {
  NORMAL,
  SUSPECTED_ANOMALY,
  UNCERTAIN
};

State state = UNCERTAIN;

void setState(State newState) {
  state = newState;

  digitalWrite(LED_PIN,
               state == SUSPECTED_ANOMALY ? HIGH : LOW);

  // Keep the buzzer off during this starter test.
  // Add a non-blocking alert pattern after validation.
  digitalWrite(BUZZER_PIN, LOW);

  switch (state) {
    case NORMAL:
      Serial.println("State: NORMAL");
      break;
    case SUSPECTED_ANOMALY:
      Serial.println("State: SUSPECTED ANOMALY");
      break;
    case UNCERTAIN:
      Serial.println("State: UNCERTAIN - check signal quality");
      break;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Wire.begin(SDA_PIN, SCL_PIN);

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("ERROR: MPU6050 connection failed");
    while (true) delay(100);
  }

  if (!ppg.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("ERROR: MAX30102 connection failed");
    while (true) delay(100);
  }

  // Starting configuration; adjust after checking signal quality.
  ppg.setup();
  ppg.setPulseAmplitudeRed(0x0A);
  ppg.setPulseAmplitudeIR(0x0A);

  Serial.println("time_ms,ir,ax,ay,az,motion_magnitude");
  setState(UNCERTAIN);
}

void loop() {
  uint32_t now = millis();
  if (now - lastSample < SAMPLE_INTERVAL_MS) return;
  lastSample = now;

  // PPG sample
  uint32_t ir = ppg.getIR();

  // Accelerometer sample
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Raw accelerometer magnitude; stationary gravity is included.
  float motionMagnitude = sqrt(
      (float)ax * ax +
      (float)ay * ay +
      (float)az * az
  );

  // Basic contact check only. This is NOT a complete SQI.
  // Threshold depends on sensor, placement, and ambient conditions.
  bool ppgPresent = ir > 10000;

  // Motion magnitude alone cannot reliably classify motion:
  // gravity and sensor orientation affect this value.
  // Use this output for data collection before setting a threshold.
  if (!ppgPresent) {
    candidateSince = 0;
    setState(UNCERTAIN);
  }

  Serial.print(now);
  Serial.print(",");
  Serial.print(ir);
  Serial.print(",");
  Serial.print(ax);
  Serial.print(",");
  Serial.print(ay);
  Serial.print(",");
  Serial.print(az);
  Serial.print(",");
  Serial.println(motionMagnitude, 1);

  // TODO 1: Estimate heart rate from detected PPG beats.
  // TODO 2: Calculate SQI over a rolling window.
  // TODO 3: Run a trained TinyML model.
  // TODO 4: Implement personalized baseline and confirmation rules.
  // TODO 5: Trigger alerts only after validated anomaly confirmation.
}
