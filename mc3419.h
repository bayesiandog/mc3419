#ifndef __MC3419
#define __MC3419

#include <stdbool.h>
 

#define MC3419_I2C_ADDRESS                  (0x98)
#define MC3419_DISABLE_FEATURE              (0x00)
#define MC3419_ENABLE_FEATURE               (0x01)

// Interrupt status bit masking
#define MC3419_TILT_INT                     (1 >> 0)
#define MC3419_FLIP_INT                     (1 >> 1)
#define MC3419_ANYM_INT                     (1 >> 2)
#define MC3419_SHAKE_INT                    (1 >> 3)
#define MC3419_TILT_35_INT                  (1 >> 4)

// Register definitions (basically all supported features are related to the following)
#define MC3419_REG_INTR_CTRL                (0X06)
#define MC3419_REG_MODE                     (0x07)
#define MC3419_REG_SR                       (0x08)
#define MC3419_REG_MOTION_CTRL              (0X09)
#define MC3419_XOUT_EX_L                    (0x0D)
#define MC3419_XOUT_EX_H                    (0x0E)
#define MC3419_YOUT_EX_L                    (0x0F)
#define MC3419_YOUT_EX_H                    (0x10)
#define MC3419_ZOUT_EX_L                    (0x11)
#define MC3419_ZOUT_EX_H                    (0x12)
#define MC3419_REG_STATUS                   (0X13)
#define MC3419_REG_INTR_STAT                (0X14)
#define MC3419_REG_RANGE                    (0X20)

#define MC3419_REG_TF_THRESH_LSB            (0X40)
#define MC3419_REG_TF_THRESH_MSB            (0X41)
#define MC3419_REG_TF_DB                    (0X42)
#define MC3419_REG_AM_THRESH_LSB            (0X43)
#define MC3419_REG_AM_THRESH_MSB            (0X44)
#define MC3419_REG_AM_DB                    (0X45)
#define MC3419_REG_SHK_THRESH_LSB           (0X46)
#define MC3419_REG_SHK_THRESH_MSB           (0X47)
#define MC3419_REG_PK_P2P_DUR_THRESH_LSB    (0X48)
#define MC3419_REG_PK_P2P_DUR_THRESH_MSB    (0X49)
#define MC3419_REG_TIMER_CTRL               (0X4A)


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


/**
 * @brief Interrupt sources register mapping
 * 
 */
typedef enum {
    TILT_INT_EN = 0,
    FLIP_INT_EN,
    ANYM_INT_EN,
    SHAKE_INT_EN,
    TILT_35_INT_EN
} mc3419_int;


/**
 * @brief Chip mode
 */
typedef enum {
    standby = 0, // Clocks are not running, no acceleration data available 
    wake         // Clocks are running, full functionality available
} mc3419_mode;


/**
 * @brief Range and scale register settings
 * 
 */
typedef struct {
    /**
     * @brief Resolution range
     */
    enum {
        r2g = 0,
        r4g,
        r8g,
        r16g,
        r12g
    } range;

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

} mc3419_range_scale;


/**
 * @brief Timer control settings for TILT_35
 * 
 *
*/
typedef enum {
    tilt35_1_6 = 0,
    tilt35_1_8,
    tilt35_2,
    tilt35_2_2,
    tilt35_2_4,
    tilt35_2_6,
    tilt35_2_8,
    tilt35_3
} mc3419_tilt35_timer;


/**
 * @brief Secondary settings for all interrupt sources. Threshold and debounce
 *      settings are in this struct.
 * 
 */
typedef struct {
    uint16_t t_f_threshold = 0;
    uint8_t t_f_debounce = 0;
    mc3419_tilt35_timer tilt_35_timer = 0;
    uint16_t anym_threshold = 0;
    uint8_t anym_debounce = 0;
    uint16_t shake_threshold = 0;
    uint8_t shake_debounce = 0;    
    uint16_t shake_p2p_dur = 0;
} mc3419_int_settings;


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


/**
 * @brief Interrupt status of pending interrupt sources
 * 
 */
typedef struct {
    bool tilt_int;
    bool flip_int;
    bool anym_int;
    bool shake_int;
    bool tilt_35_int;    
} mc3419_int_stat;


/**
 * @brief Acceleration data for all 3 axis
 * 
 */
typedef struct {
    uint16_t xG;
    uint16_t yG;
    uint16_t zG;
} mc3419_xyz;



// Functions
uint8_t mc3419_reg_wr(uint8_t reg, uint8_t val);
uint8_t mc3419_reg_r(uint8_t reg, uint8_t* val, uint8_t nBytes);
uint8_t mc3419_set_mode(mc3419_mode mode);
uint8_t mc3419_set_odr(mc3419_odr odr);
uint8_t mc3419_set_range(mc3419_range_scale rs);
uint8_t mc3419_set_int(mc3419_int_source source);
uint8_t mc3419_check_new_data(void);
uint8_t mc3419_check_int(mc3419_int_stat* stat);
uint8_t mc3419_get_xyz(mc3419_xyz* data);