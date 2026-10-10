# pulseguard-edge
Tri-state personalized TinyML for offline heart-rate anomaly detection
# PulseGuard Edge

## Overview

PulseGuard Edge is a low-cost wearable prototype that detects sustained
tachycardia and bradycardia patterns without cloud connectivity. It uses a
tri-state classifier (normal / suspected anomaly / uncertain) to avoid
false alerts caused by wrist motion.

## Hardware

| Component | Purpose |
|---|---|
| ESP32 | Main microcontroller |
| MAX30102 | PPG sensor for heart-rate estimation |
| MPU6050 | Accelerometer for motion context |
| LiPo 1000 mAh | Battery |
| Buzzer + LED | Local alert |
| TP4056 | Charging module |

Total BOM cost: $[XX] (see BOM.csv)

## Wiring

| Sensor | Sensor Pin | ESP32 Pin |
| MAX30102 | VIN/VCC | 3.3V* |
| MAX30102 | GND | GND |
| MAX30102 | SDA | GPIO 21 |
| MAX30102 | SCL | GPIO 22 |
| MPU6050 | VCC | 3.3V* |
| MPU6050 | GND | GND |
| MPU6050 | SDA | GPIO 21 |
| MPU6050 | SCL | GPIO 22 |
| AD8232 ECG | 3.3V | 3.3V |
| AD8232 ECG | GND | GND |
| AD8232 ECG | OUTPUT | GPIO 34 |
| AD8232 ECG | LO+ | GPIO 32 (optional) |
| AD8232 ECG | LO− | GPIO 33 (optional) |
| Buzzer | Signal (+) | GPIO 26 |
| Buzzer | GND (−) | GND |
| LED | Anode (+), through 220 Ω resistor | GPIO 25 |
| LED | Cathode (−) | GND |
| Battery/power supply | Positive | Suitable regulated ESP32 power input* |
| Battery/power supply | Negative | GND |

## How to Build

1. Install Arduino IDE and ESP32 board support
2. Install libraries: [list them]
3. Open `firmware/pulseguard_edge.ino`
4. Select board: ESP32 Dev Module
5. Flash

## How to Reproduce the Test

1.## Test Setup

The PulseGuard Edge prototype will be developed using an ESP32 microcontroller mounted on a perfboard for compact wearable integration. A MAX30102 sensor will acquire photoplethysmography (PPG) signals for heart-rate estimation, while an MPU6050 accelerometer and gyroscope will capture motion data to identify potentially unreliable readings. An AD8232 ECG module will provide a secondary heart-rate reference for experimental comparison. A buzzer and LED will provide local alerts when sustained high- or low-heart-rate patterns are detected.

The prototype will be tested under stationary and controlled-motion conditions to evaluate signal quality, uncertainty handling, and false alerts. The proposed detection approach will be compared with a basic threshold-based method using recorded datasets and controlled signal replay. Key evaluation metrics will include heart-rate estimation error, sensitivity, false-alert rate, detection latency, uncertain-window frequency, and power consumption. Testing will be conducted using a battery-powered setup, with results documented to assess the feasibility of low-cost, offline wearable monitoring.

2. ## Anomaly Injection Method

Anomalies will be introduced through controlled signal replay and software-based test cases to evaluate PulseGuard Edge without deliberately inducing abnormal heart rates in a person. Recorded PPG signals will be replayed or processed to represent sustained high- and low-heart-rate patterns, while synthetic disturbances such as motion artifacts, signal-amplitude changes, and irregular inter-beat intervals will be added to test signal-quality assessment. Normal heart-rate recordings will serve as baseline cases. Each test case will be labelled as normal, suspected anomaly, or uncertain, depending on the heart-rate pattern and signal quality. The system's alerts, detection latency, false-alert rate, and uncertainty decisions will be recorded and compared against a basic threshold-based method. Simulated anomalies will be evaluated separately from clinically labelled events to avoid treating synthetic test cases as medical evidence.

3.## Latency Measurement

Detection latency will be measured as the time between the onset of a sustained, injected heart-rate anomaly in a recorded or replayed signal and the activation of the corresponding alert by the ESP32. The anomaly onset time and alert activation time will be recorded using timestamps or a logic analyzer/oscilloscope monitoring a dedicated GPIO output. Latency will be calculated as:

**Detection Latency = Alert Activation Time − Anomaly Onset Time**

Tests will be repeated across different anomaly magnitudes, signal-quality conditions, and motion levels. The average, median, and maximum observed latency will be reported, along with missed detections. Signal acquisition, processing, confirmation windows, and buzzer/LED activation will be considered when interpreting the results. The target is to achieve an alert latency below 60 seconds for sustained anomalies, subject to experimental validation.

## Results

| Metric | Target | Measured |
| Alert latency | < 60 s | [X] s |
| False alerts | < 1 per 8 h | [X] per 8 h |
| Battery life | 24 h | [X] h |

## Limitations

- Detects sustained HR deviations, not specific arrhythmias
- Not a clinical device
- ## Limitations
1. PPG Sensor Accuracy: The MAX30102 readings are susceptible to motion artifacts, inconsistent skin contact, sensor placement, and ambient light interference.

2. Limited Clinical Validation:The system detects sustained high or low heart-rate patterns and is not intended to diagnose arrhythmias or other medical conditions.

3. Dataset Constraints:Public wearable datasets may lack clinically labelled tachycardia and bradycardia events. Simulated anomalies cannot fully represent real-world clinical conditions.

4. False Positives and Missed Events:Signal-quality filtering and temporal confirmation may reduce false alerts but can also delay or miss genuine abnormalities.

5. Personalization Dependency:Detection performance depends on initial calibration and the reliability of the personalized baseline. Changes in resting heart rate may affect detection.

6. Processing Constraints:Signal-quality assessment, TinyML inference, and continuous sensor processing must operate within the ESP32's limited memory and computational resources.

7. Battery-Life Uncertainty:Continuous sensing, processing, buzzer alerts, and BLE communication consume power. The 24-hour operating target with a 1000 mAh battery requires practical validation.

8. Connectivity Dependency:Local detection and event buffering can operate offline, but remote caregiver notifications require a connected gateway and internet access.

9. **Security Implementation:** Metadata encryption and rotating device identifiers are planned privacy measures and require implementation and verification before they can be considered operational.

10. Long-Term Reliability:Short-duration tests cannot establish performance over extended use. Multi-hour and repeated trials are needed to evaluate false-alert frequency, detection latency, sensor reliability, and power consumption.

## Status

[Prototype in progress]
