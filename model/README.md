# TinyML Model

## Framework
- **Primary:** Edge Impulse
- **Alternative:** TensorFlow Lite for Microcontrollers (TFLite Micro)

## Input Features
The model is designed to use features extracted from:
- **PPG (MAX30102):** Signal-amplitude stability, inter-beat interval regularity, and PPG signal-quality indicators.
- **Motion (MPU6050):** Acceleration magnitude, motion variability, and motion-correlated signal disturbance.

## Output Classes
The model is intended to support three outcomes:

1. **Normal:** Reliable signal with no detected sustained heart-rate anomaly.
2. **Suspected Anomaly:** Reliable signal showing a sustained unusually high or low heart-rate pattern.
3. **Uncertain:** Poor-quality signal or insufficient evidence for a reliable decision.

## Training Data
- **PPG-DaLiA:** A public dataset containing wrist-worn PPG and motion recordings, suitable for investigating motion-related signal degradation.
- **Controlled Signal Replay:** Recorded signals and software-generated test cases representing normal patterns, sustained high/low heart-rate patterns, and motion-related artifacts.

Synthetic anomalies will be labelled separately from clinically annotated events. Training, validation, and testing data will be separated to reduce data leakage.

## Deployment
The trained model is intended to be exported as an Arduino library from Edge Impulse and integrated into the ESP32 firmware.

Firmware integration target: [`firmware/pulseguard_edge.ino`](firmware/pulseguard_edge.ino)

The deployed pipeline will combine signal-quality assessment, model inference, uncertainty handling, and confirmed local alerts.
