# MC3419 Driver
![image](https://user-images.githubusercontent.com/50047346/222525058-e980e436-bfd0-4b9a-aadf-7afdd8e5a448.png)

## Chip
![image](https://user-images.githubusercontent.com/50047346/222525238-7327ca08-2665-4b27-8a10-ab8403c02b4a.png)

## Brief Description
MC3419 accelerometer driver for microcontrollers using the I2C protocol.

This driver supports the basic features of the accelerometer for quick prototyping and development.

### Configurations supported
User can configure the chip's:

```ODR``` (Output Data Rate)

```Resolution``` ( G Range)

```LPF``` (Low Pass Filter)

```Interrupts```

#### ODR

```c
/**
 * @brief Output data rates (ODR) in Hz
 */
typedef enum {
    odr_50 = 8,  // 50Hz
    odr_100,     // 100Hz
    odr_125,     // 125Hz
    odr_200,     // 200Hz
    odr_250,     // 250Hz
    odr_500,     // 500Hz
    odr_1000,    // 1000Hz
    odr_2000     // 2000Hz
} mc3419_odr;
```

#### Resolution
```c
enum {
    r2g = 0,
    r4g,
    r8g,
    r16g,
    r12g
} range;
```

#### LPF
```c
// Enable Low Pass Filter (with lpf set)
bool lpf = 0;

/**
 * @brief LPF bandwidth setting
 * 
 */
enum {
    idr_div_4 = 1, // Fc = IDR/4.255
    idr_div_6,     // Fc = IDR/6
    idr_div_12,    // Fc = IDR/12
    idr_div_16 = 5 // Fc = IDR/16
} filter;
```

#### Interrupts
```c
/**
 * @brief Interrupt sources
 */
typedef struct {
    bool tilt = 0;
    bool flip = 0;
    bool anym = 0;
    bool shake = 0;
    bool tilt_35 = 0;
    mc3419_int_settings settings;
} mc3419_int_source;
```
