# STM32 LED Control & Anti-Bounce Button Driver

A highly responsive, non-blocking embedded C application developed for STM32 microcontrollers using the STM32Cube HAL framework. This project demonstrates how to orchestrate hardware Timers, Pulse Width Modulation (PWM), External Interrupts (EXTI), and Nested Vectored Interrupt Controller (NVIC) management to control LEDs dynamically via a physical 5-button keypad (Up, Down, Left, Right, Middle).

## 🚀 Key Features

* **Dynamic PWM Control (TIM4):** Independent 4-channel PWM generation with synchronized on-the-fly updates for both frequency (blinking speed) and duty cycle (brightness).
* **Zero-Lag Responsiveness:** Custom hardware update routines bypass standard timer cycle completions, updating the LED pulse parameters instantly when requested by the user.
* **Hybrid Debouncing Logic (TIM3 + EXTI):** A reliable anti-bounce mechanism that instantly locks out noise at the NVIC layer upon an edge trigger, eliminating CPU-heavy pooling or delays (`HAL_Delay`).
* **Strict Clean Code Architecture:** Fully documented with industry-standard Doxygen-style formatting for seamless IDE tooltips and automated documentation generation.

---

## 🛠️ Hardware & Peripheral Architecture

The architecture relies entirely on hardware peripherals and low-overhead interrupt routines to keep the main CPU execution thread free.

### Peripherals Utilization Table

| Peripheral | Mode / Configuration | Role in System |
| :--- | :--- | :--- |
| **TIM4 (CH1 - CH4)** | PWM Generation | Generates variable frequency and duty cycle signals to control 4 independent LEDs. |
| **TIM3** | Base Timer (Interrupt Enabled) | Acts as a hardware delay window for button debouncing (One-shot style execution). |
| **EXTI (5-9, 10-15)**| Falling/Rising Edge Triggers | Captures physical push-button edge transitions instantly via dedicated NVIC lines. |

---

## 📈 System Workflow & Core Logic

### 1. Advanced Debouncing Sequence
Traditional debouncing often uses blocking methods like `HAL_Delay()`, which stalls execution. This driver uses a robust **State-Locking Interrupt Method**:

1.  A physical button press triggers an external interrupt (`HAL_GPIO_EXTI_Callback`).
2.  The active button port and pin are immediately cached in a globally volatile `Button_pressed` context structure.
3.  **The Shielding Phase:** `Interrupts_GPIO_disable()` is invoked, shutting down `EXTI9_5_IRQn` and `EXTI15_10_IRQn` vector processing in the NVIC. This completely isolates the system from subsequent mechanical contact-bounce noise.
4.  Timer 3 (`TIM3`) is started to introduce a stable, non-blocking hardware noise-settling window.
5.  Upon `TIM3` period elapse, `HAL_TIM_PeriodElapsedCallback` confirms the button's physical voltage level. If validated, the appropriate LED modifier function is executed.
6.  Interrupt lines are safely restored via `Interrupts_GPIO_enable()`.

### 2. On-The-Fly PWM Modifications (Instant Sync)
When a user alters the blinking frequency, waiting for a slow timer period (e.g., $1\text{ Hz} = 1\text{ second}$) to expire before applying the update results in sluggish UI feedback. 

To solve this, the driver uses an instant-hardware-force methodology inside `LED_Hardware_Update()`:
* New register values for the Auto-Reload Register (`ARR`) and Capture/Compare Registers (`CCR`) are recalculated proportionally to preserve consistent perceived brightness.
* The hardware registers are updated using standard HAL abstraction macros.
* **Instant Software Forced Update:** By setting the Update Generation bit (`TIM_EGR_UG`) directly inside the Event Generation Register (`EGR`), the counter register (`CNT`) resets to zero and forces the new parameters to take effect *immediately*, eliminating sluggish response times.

---

## 📁 File Structure

* `led_functions.c` / `.h` — Houses the calculation math for frequency, brightness limits, and raw register manipulations for `TIM4`.
* `push_buttons_functions.c` / `.h` — Manages the physical button ISR configurations, EXTI handlers, and NVIC masking utilities.
* `push_buttons_callbacks.c` — Implements the `HAL_TIM_PeriodElapsedCallback` debounce processing and feature dispatcher state machine.

---

## 🔧 Driver Parameters Configuration

You can customize operational boundaries and stepping bounds directly within `led_functions.c`:

```c
#define MAX_BRIGHTNESS             100  // Maximum brightness %
#define MIN_BRIGHTNESS               0  // Minimum brightness %
#define MAX_FREQUENCY              100  // Maximum blinking speed (Hz)
#define MIN_FREQUENCY                1  // Minimum blinking speed (Hz)
#define CHANGE_FREQUENCY_VALUE      10  // Hz adjustment step size
#define CHANGE_BRIGHTNESS_VALUE     10  // Brightness % adjustment step size