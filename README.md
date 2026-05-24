# Predictive-Maintenance-via-Acoustic-Fingerprint-on-ESP32-S3

## Introduction

This project focuses on developing a high-speed data sampling system for predictive maintenance of hard-to-detect failures in industrial equipment, specifically targeting bearing failures at a sampling rate of 96kHz. The system utilizes the ESP32-S3 as a cost-effective central processor, combined with sensors such as the PCM1808 and TCT40-16R to capture full multi-band audio data. The entire project is designed end-to-end, from the physical PCB circuit to the accompanying software.

---

## Structure

- `docs/`: Documentation and design notes.
  - `notes/`: Development notes and considerations.
    - `analysis/`: Analysis of requirements and design decisions.
    - `peripherals/`: Details on peripheral usage and configuration.
    - `components/`: Information on hardware components used in the project.
  - `references/`: Reference materials and datasheets.
- `project-architecture/`: Overview of the project architecture, containing main driver code structure.
- `project-kicadspace/`: KiCad design files for the PCB layout and schematic.
- `project-obsidianspace/`: Obsidian vault for project notes and documentation.

---

## Features

- **Microcontroller**: ESP32-S3-DevKitC N16R8 for high-performance processing and connectivity.
- **Peripherals**:
  - SPI for communication with external microSD card for data storage
  - I2S for audio data acquisition from the PCM1808.
  - GPIO for status indicators and control signals.
  - RCC for reset & clock management.
  - I2C for LCD interfacing (optional).
  - DMA for efficient data transfer from ADC to memory.

---

## Hardware Components

Note that this sections can be expand with new requirements in the future.

- ESP32-S3-DevKitC N16R8 microcontroller
- MAX9812 microphone amplifier
- PCM1808 ADC
- TL072IDT operational amplifier
- TCT40-16R MEMS microphone
- JP-313B for 3.5mm Jack audio input
- MicroSD card module for data storage
- Optional: I2C LCD display for status monitoring

---

## References

- Do You Hear What I Hear?: Fingerprinting Smart Devices Through Embedded Acoustic Components by Anupam Das, Nikita Borisov, Matthew Caesar, 2014.
- Audio Data Acquisition System Design Based on ARM and DSP by Yongxin Wang, 2015.
- Predictive Maintenance with Sensor Data Analytics on a Raspberry Pi-Based Experimental Platform by Shang-Yi Chuang, Nilima Sahoo, Hung-Wei Lin and Yeong-Hwa Chang, 2019.
- Challenges in predictive maintenance – A review by P.Nunes, J.Santos, E.Rocha, 2022.
- Predictive Maintenance: An Embedded System Approach by Igor Tedeschi Franco, Rodrigo Marques de Figueiredo, 2022.
- Device Fingerprinting for Cyber-Physical Systems: A Survey by Vịay Kumar and Kolin Paul, 2023.
- Anomaly Detection In IoT Sensor Data Using Machine Learning Techniques For Predictive Maintenance In Smart Grids by Edwin Omol, Lucy Mburu, Paul Abuonji,  Dorcas Onyango, 2024.
- Application of Embedded Systems in Industrial IoT for Predictive Maintenance by Elena García-Ortega, 2024.
- Explainable Predictive Maintenance: A Survey of Current Methods, Challenges and Opportunities by Logan Cummins, Alex Sommers, Somayeh Bakhtiari Ramezani, Supid Mittal, Joseph Jabour, Maria Seale and Shahram Rahimi, 2024.
- Acoustic fingerprint in vehicle manufacturing as a basis for future applications by Jozsef Nagy, Istvan Lakatos, 2025.
- End-to-End Methodology for Predictive Maintenance Based on Fingerprint Routines and Anomaly Detection for Machine Tool Rotary Components by Amaia Arregi, Aitor Barrutia and Iñigo Bediaga, 2025.
