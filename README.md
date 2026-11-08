# mini-flight-computer
# Mini Flight Computer (Arduino + Wokwi)

A small flight-computer simulation built on an Arduino Uno. It reads acceleration from an MPU6050, tracks flight states, shows live data on an I2C LCD, logs data over Serial, and sounds a buzzer when acceleration exceeds a safety limit.

**Live simulation:** [YOUR_WOKWI_LINK](https://wokwi.com/projects/480135874689067009)



## Features

- **Three flight states:** `IDLE`, `ARMED`, `LAUNCHED`
- **Arming logic:** pressing the button moves IDLE to ARMED. Launch detection only works while ARMED, so a bump on the ground does not trigger a launch.
- **Automatic launch detection:** in ARMED state, acceleration above 15 m/s² (about 1.5 g) switches the state to LAUNCHED and turns on the LED.
- **Reset:** pressing the button again returns to IDLE.
- **Warning buzzer:** sounds when acceleration exceeds 18 m/s², and stops when it drops back.
- **Data logging:** prints `time_ms,state,acceleration` to the Serial Monitor every 200 ms.

## Hardware (simulated in Wokwi)

| Component | Connection |
|---|---|
| Arduino Uno | - |
| 16x2 I2C LCD (0x27) | SDA to A4, SCL to A5, VCC to 5V, GND to GND |
| MPU6050 | SDA to A4, SCL to A5, VCC to 5V, GND to GND |
| Push button | Pin 7 and GND (`INPUT_PULLUP`) |
| LED | Pin 13 and GND |
| Buzzer | Pin 8 and GND |

The LCD and MPU6050 share the same I2C bus.

## How it works

Total acceleration is computed from all three axes:

`acc = sqrt(ax² + ay² + az²)`

At rest this reads about 9.8 m/s² (1 g, gravity). The sketch checks this value every 200 ms against the launch threshold and the warning limit.

## How to run

1. Open the Wokwi link above, or create a new Arduino Uno project and paste in `sketch.ino`, `diagram.json` and `libraries.txt`.
2. Click **Start Simulation**.
3. Set the MPU6050 sliders to Y = 0 g, Z = 1 g (resting). The LCD shows `IDLE` and about 9.8 m/s².
4. Press the button. The state becomes `ARMED`.
5. Raise the Z slider to 2 g. The state becomes `LAUNCHED`, the LED turns on and the buzzer sounds.
6. Press the button to reset.

## Libraries

- LiquidCrystal_I2C
- Adafruit MPU6050
