## Table of Contents
- [Introduction](#Introduction)
- [Features](#Features)
- [Hardware & Components](#Hardware%20&%20Components)
- [System Architecture](#System%20Architecture)

# Introduction
Modern agriculture faces challenges such as unpredictable weather, soil degradation, and inefficient crop selection, leading to reduced yields and financial losses for farmers. To address this, our Crop Recommendation System leverages IoT sensors and an Deep Neural Network (DNN) to analyze real-time environmental and soil conditions, providing accurate crop recommendations.

This project utilizes a locally running DNN on the ESP32 to make predictions.

# Features
✔ Real-time soil monitoring (NPK levels, moisture, temperature, resistance)

✔ Deep Learning-based crop recommendation

✔ User-friendly dashboard for farmers

✔ Sustainable and data-driven farming approach



# Hardware & Components

#### This project utilizes:-

  ● ESP-32S microcontroller

  ● TTL to RS485 converter

  ● JXCT Soil NPK sensor

  ● Soil resistance measuring probe

  ● I²C OLED Screen


# System Architecture 
1) The IoT sensors collect and analyze data, which is then communicated to the ESP32 using the RS485 to TTL converter.
2) ESP32 processes the collected data to be fed to the DNN.
3) The DNN is locally run on the ESP32, and the best recommended crop displayed on the OLED screen. Additionally, all soil values such as N, P, pH, etc., are also displayed for analysis

![image](https://github.com/user-attachments/assets/439240eb-37cf-482e-8370-7d0d98310841)
