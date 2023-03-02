#include <stdint.h>

#include "mc3419.h"


int main(void) {
    
    // Check for every config function's return value

    // Chip needs to be in standby mode to receive configuration
    mc3419_set_mode(standby);
    mc3419_set_odr(odr_200);

    mc3419_range_scale rangeScale = {
        .range = r2g,
        .lpf = 1,
        .filter = idr_div_4
    };

    
    mc3419_set_range(rangeScale);

    mc3419_int_source intSource = {
        .tilt_35 = 1, // Enable TILT 35 interrupt source
        .settings.tilt_35_timer =  tilt35_2 // 2 second duration of valid tilt-35 angle detection 
    };

    mc3419_set_int(intSource);
    mc3419_set_mode(wake);

    while (!mc3419_check_new_data()) {
        Delay(10);
    }

    mc3419_xyz* data;
    mc3419_get_xyz(data);

    return 0;
}
