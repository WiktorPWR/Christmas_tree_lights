# Embedded Event-Driven LED & Input Controller (STM32 HAL)

An enterprise-grade, non-blocking embedded C application architecture developed for STM32 microcontrollers. This production-ready system utilizes the **STM32Cube Hardware Abstraction Layer (HAL)** framework to orchestrate hardware Timers, Advanced PWM Generation, and Nested Vectored Interrupt Controller (NVIC) state masking. 

The core design philosophy is completely **Event-Driven and Non-Blocking**, eliminating primitive blocking loops (e.g., `HAL_Delay`) to achieve near-zero CPU overhead and maximum power efficiency.

---

## 🏛️ Architectural Overview & HAL Integration

This solution abstracts hardware interaction through the **STM32Cube HAL Layer**, ensuring high portability across the STM32 ecosystem (e.g., G4, F4, L4, or H7 series) while retaining the ability to perform raw register manipulation for ultra-low latency hardware updates.