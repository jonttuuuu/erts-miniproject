#include "lvgl.h"
#include "gui/gui.h"

#include "hardware/hardware.h"
#include "hardware/powermgm.h"

#include "app/gps_status/gps_status.h"
#include "app/hiking_tour_assistant/hta.h"

#if defined( NATIVE_64BIT )
    /**
     * for non arduino
     */                 
    void setup( void );
    void loop( void );

    int main( void ) {
        setup();
        while( 1 ) { loop(); };
        return( 0 );
    }
#endif // NATIVE_64BIT

void setup() {
    /**
     * hardware setup
     */
    hardware_setup();
    /**
     * gui setup
     */
    gui_setup();
    
    /**
     * apps here
     */
    hta_app_setup();
    
    /**
     * post hardware setup
     */
    hardware_post_setup();
}

void loop(){
    powermgm_loop();
}
