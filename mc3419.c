#include <stdint.h>

#include "mc3419.h"


/**
 * @brief Writes data in specified MC3419 register
 * 
 * @param reg   Register
 * @param val  Value
 * @return uint8_t 0: OK, 1: Error 
 */
uint8_t mc3419_reg_wr(uint8_t reg, uint8_t val) {
    // Platform specific I2C function (adjust accordingly).
    // This driver simply writes one uint8_t value
    // to a single register for easier portability.
    if (!I2C_Write(MC3419_I2C_ADDRESS, reg, 0, 1, val))
	    return 1;
    
    return 0;
}


/**
 * @brief Reads data in specified MC3419 register
 * 
 * @param reg    Register
 * @param val    Value read
 * @param nBytes Number of bytes to be read
 * @return uint8_t 0: OK, 1: Error 
 */
uint8_t mc3419_reg_r(uint8_t reg, uint8_t* val, uint8_t nBytes) {
    // Platform specific I2C function (adjust accordingly).
    // This driver simply writes one uint8_t value
    // to a single register for easier portability.
    if (!I2C_Read(MC3419_I2C_ADDRESS, reg, 0, nBytes, &val))
	    return 1;
    
    return 0;
}


/**
 * @brief Sets the chip's operating state. STANDBY mode 
 *      allows writing to all registers whereas WAKE mode 
 *      only allows writing to the MODE register. 
 *      Chip configuration is performed in STANDBY mode.
 *      There is some confusion in the datasheet
 *      but WAKE state is 1 and STANDBY is 0.
 * 
 * @param mode 1: WAKE, 0: STANDBY
 * @return uint8_t 0: OK, 1: Error 
 */
uint8_t mc3419_set_mode(mc3419_mode mode) {
    if (mc3419_reg_wr(MC3419_REG_MODE, mode))
        return 1;
    
    return 0;
}


/**
 * @brief Sets the output data rate (ODR) for the chip.
 * 
 * @param odr mc3419_odr (enum) Output Data Rate
 * @return uint8_t 
 */
uint8_t mc3419_set_odr(mc3419_odr odr) {
    if (mc3419_reg_wr(MC3419_REG_SR, odr))
        return 1;

    return 0;
}


/**
 * @brief Sets the chip's range and filtering options.
 * 
 * @param rs mc3419_range_scale structure
 * @return uint8_t 0: OK, 1: Error 
 */
uint8_t mc3419_set_range(mc3419_range_scale rs) {
    uint8_t regVal = 0;

    regVal = rs.range | rs.lpf << 3 | rs.filter << 4;
    
    if (mc3419_reg_wr(MC3419_REG_RANGE, regVal))
        return 1;

    return 0;
}


/**
 * @brief Enables interrupt sources for the chip.
 *       Registers INTR_CTRL and MOTION_CTRL need to
 *       be adjusted. 
 * 
 * @param source Interrupt sources structure
 * @return uint8_t 0: OK, 1: Error
 */
uint8_t mc3419_set_int(mc3419_int_source source) {
    uint8_t regVal = 0;


    regVal = source.tilt >> TILT_INT_EN | source.flip >> FLIP_INT_EN | source.anym >> ANYM_INT_EN 
            | source.shake >> SHAKE_INT_EN | source.tilt_35 >> TILT_35_INT_EN;

    if (mc3419_reg_wr(MC3419_REG_INTR_CTRL, regVal))
        return 1;
    
    // AnyMotion feature must also be enabled for shake and tilt_35 features
    if (source.shake || source.tilt_35)
        regVal |=  source.anym >> ANYM_INT_EN;

    if (mc3419_reg_wr(MC3419_REG_MOTION_CTRL, regVal & 0x1D)) // Value ANDed with 0x1D to keep some default (and rarely used) settings intact (p.46 of datasheet)
        return 1;

    if (source.settings.t_f_threshold) { // Avoid unnecessary I2C writing
        regVal = (uint8_t)source.settings.t_f_threshold;
        if (mc3419_reg_wr(MC3419_REG_TF_THRESH_LSB, regVal))
            return 1;
        
        regVal = (uint8_t)(source.settings.t_f_threshold >> 8);
        if (mc3419_reg_wr(MC3419_REG_TF_THRESH_MSB, regVal & 0x7F)) // Reserved 15th (8th in this function) bit
            return 1;
    }

    if (source.settings.t_f_debounce) {
        regVal = source.settings.t_f_debounce;
        if (mc3419_reg_wr(MC3419_REG_TF_DB, regVal))
            return 1;
    }

    if (source.settings.anym_threshold) {
        regVal = (uint8_t)source.settings.anym_threshold;
        if (mc3419_reg_wr(MC3419_REG_AM_THRESH_LSB, regVal))
            return 1;
        
        regVal = (uint8_t)(source.settings.anym_threshold >> 8);
        if (mc3419_reg_wr(MC3419_REG_AM_THRESH_MSB, regVal & 0x7F)) // Reserved 15th (8th in this function) bit
            return 1;
    }

    if (source.settings.anym_debounce) {
        regVal = source.settings.anym_debounce;
        if (mc3419_reg_wr(MC3419_REG_AM_DB, regVal))
            return 1;
    }

    if (source.settings.shake_threshold) {
        regVal = (uint8_t)source.settings.shake_threshold;
        if (mc3419_reg_wr(MC3419_REG_SHK_THRESH_LSB, regVal))
            return 1;    
    
        regVal = (uint8_t)(source.settings.shake_threshold >> 8);
        if (mc3419_reg_wr(MC3419_REG_SHK_THRESH_MSB, regVal))
            return 1;
    }
        
    if (source.settings.shake_p2p_dur) {
        regVal = (uint8_t)source.settings.shake_p2p_dur;
        if (mc3419_reg_wr(MC3419_REG_PK_P2P_DUR_THRESH_LSB, regVal))
            return 1;
        
        regVal = (uint8_t)(source.settings.shake_p2p_dur >> 8);
        if (mc3419_reg_wr(MC3419_REG_PK_P2P_DUR_THRESH_MSB, regVal & 0x7F))
            return 1;
    }

    if (source.timer) {
        regVal = source.timer;
        if (mc3419_reg_wr(MC3419_REG_TIMER_CTRL, regVal & 0x07)) // no temporary latch support in this driver
            return 1;
    }
    
    return 0;
}


/**
 * @brief Checks for new XYZ data.
 * 
 * @return uint8_t 0: No new data, 1: New data available, 2: Error
 */
uint8_t mc3419_check_new_data(void) {
    uint8_t* newData;


    mc3419_reg_r(MC3419_REG_STATUS, &newData, 1);
    
    return (*newData & 0x80);
}


/**
 * @brief Reports the status of any pending interrupt sources.  
 * 
 * @param stat Pointer to mc3419_int_stat
 * @return uint8_t 0: OK, 1: Error 
 */
uint8_t mc3419_check_int(mc3419_int_stat* stat) {
    uint8_t* regVal;


    mc3419_reg_r(MC3419_REG_STATUS, &regVal, 1);

    stat->tilt_int = *regVal & MC3419_TILT_INT;
    stat->flip_int = *regVal & MC3419_FLIP_INT;
    stat->anym_int = *regVal & MC3419_ANYM_INT;
    stat->shake_int = *regVal & MC3419_SHAKE_INT;
    stat->tilt_35_int = *regVal & MC3419_TILT_35_INT;
    return 0;
}


/**
 * @brief Retrieves current G values.
 * 
 * @param data Pointer to mc3419_xyz structure
 * @return uint8_t OK, 1: Error
 */
uint8_t mc3419_get_xyz(mc3419_xyz* data) {
    uint8_t regVal[6];


    mc3419_reg_r(MC3419_XOUT_EX_L, &regVal, 6); // Reads all 6 data registers (starting from 0x0D)

    data->xG = (regVal[1] << 8) | regVal[0];
    data->yG = (regVal[3] << 8) | regVal[2];
    data->zG = (regVal[5] << 8) | regVal[4];
    return 0;
}
