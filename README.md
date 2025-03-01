## Table of Contents
- [Introduction](#Introduction)
- [Features](#Features)
- [Hardware & Components](#Hardware_&_Components)
- [System Architecture](#System_Architecture)

# Introduction
Modern agriculture faces challenges such as unpredictable weather, soil degradation, and inefficient crop selection, leading to reduced yields and financial losses for farmers. To address this, our Crop Recommendation System leverages IoT sensors and an Deep Neural Network (DNN) to analyze real-time environmental and soil conditions, providing accurate crop recommendations.

This project utilizes a locally running DNN on the ESP32 to make predictions.

# Features
✔ Real-time soil monitoring (NPK levels, moisture, temperature, resistance)

✔ Deep Learning-based crop recommendation

✔ User-friendly dashboard for farmers

✔ Sustainable and data-driven farming approach



# Hardware_&_Components

#### This project utilizes:-

  ● ESP-32S microcontroller

  ● TTL to RS485 converter

  ● JXCT Soil NPK sensor

  ● Soil resistance measuring probe

  ● I²C OLED Screen


# System_Architecture 
1) The IoT sensors collect and analyze data, which is then communicated to the ESP32 using the RS485 to TTL converter.
2) ESP32 processes the collected data to be fed to the DNN.
3) The DNN is locally run on the ESP32, and the best recommended crop displayed on the OLED screen. Additionally, all soil values such as N, P, pH, etc., are also displayed for analysis

## Example circuit diagram

![circuit_dia](https://github.com/user-attachments/assets/0c793094-a2b7-43db-a9c6-ae1475a61f75)

