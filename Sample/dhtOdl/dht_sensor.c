#include "os_type.h"
#include <stdlib.h>
#include <string.h>
#include "osapi.h"
#include "c_types.h"
#include "gpio.h"
#include "user_interface.h"
#include "ets_sys.h"

#define os_printf ets_uart_printf
#define ets_timer_arm_new ets_timer_arm
#define HIGHLEVEL 41
#define MAXTIMINGS 32000

#include "dht_sensor.h"

os_timer_t timer_dhtSensor;
unsigned char count;
uint8 gpio_id;
uint32 gpio_name;
uint8 gpio_func;
uint8_t data[6];
uint8_t counters[MAXTIMINGS];

void dht_sensor_write(bool);
bool dht_sensor_read(void);

void dht_timerfunc(void *arg)
{
    os_timer_disarm(&timer_dhtSensor);

    count++;
    
    bool high = (count&0x01);
    uint8_t laststate = 1;
    uint8_t counter = 0;
    uint8_t j = 0, i;

    // if(high) {
    //     os_delay_us(250 * 1000);
    // }
    // dht_sensor_write(high);
    // os_printf("Pulling GPIO to %s, was: 0x%02x\n", (high?"high":"low"), lastState);
    
    os_printf("Started temp reading!\n");

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    for(i = 0; i < MAXTIMINGS; i ++) {
        counters[i] = 0xFF;
    }

    // pull the pin high and wait 250 milliseconds
    dht_sensor_write(1);
    os_delay_us(250 * 1000);

    // now pull it low for ~20 milliseconds
    dht_sensor_write(0);
    os_delay_us(20 * 1000);
    dht_sensor_write(1);
    os_delay_us(40);
    
    // disable output
    PIN_PULLUP_DIS(gpio_name);
    GPIO_DIS_OUTPUT(GPIO_ID_PIN(gpio_id));
        
    // read in timings
    for(i = 0; i < MAXTIMINGS; i++) {
        counter = 0;
        while(dht_sensor_read() == laststate) {
            counter++;
            os_delay_us(1);
            if (counter == 255) {
                break;
            }
        }
        laststate = dht_sensor_read();

        if(counter == 255) {
            os_printf("Temp reading %d aborted due to counter!\n", i);
            break;
        }
        counters[i] = counter;
    }
    dht_sensor_write(0);

    for(i = 0; i < MAXTIMINGS; i ++) {
        if(counters[i] != 0xFF) {

            // ignore first 3 transitions
            if((i >= 4) && (i%2 == 0)) {
                // os_printf("read: %d | %d\n", i, counters[i]);
                // shove each bit into the storage bytes
                data[j/8] <<= 1;
                if (counters[i] > HIGHLEVEL) {
                    data[j/8] |= 1;
                }
                j++;
            }
        }
    }

    bool success = false;
    if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
        success = true;
    }

    os_printf("Temp reading finished! success: %d, data: 0x%02x%02x%02x%02x chk: %02x\n", success, data[0], data[1], data[2], data[3], data[4]);
    os_printf("Temp: %d°C, Hum: %d%%\n", data[2], data[0]);
}

void dht_sensor_init()
{
    os_printf("Initializing the sensor timer\n");
    gpio_id = 14;
     gpio_name = FUNC_GPIO14;
     //gpio_func = PERIPHS_IO_MUX_MTMS_U;
    
    // now we setup pin GPIO2 as GPIO
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
    dht_sensor_write(0);

    // set internal variable to decide on/off
    count = 0;
        
    // arm & start a timer that calls the blinky function
    os_timer_disarm(&timer_dhtSensor);
    os_timer_setfn(&timer_dhtSensor, (os_timer_func_t *)dht_timerfunc, NULL);
    os_timer_arm(&timer_dhtSensor, 3000, 1);

    os_printf("Initialized the sensor by pulling GPIO to low\n");
}

void dht_sensor_write(uint8_t bit)
{
    GPIO_OUTPUT_SET(GPIO_ID_PIN(gpio_id), bit);
}

uint8_t dht_sensor_read(void)
{
    return GPIO_INPUT_GET(GPIO_ID_PIN(gpio_id));
}