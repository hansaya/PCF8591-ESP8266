#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include "os_type.h"
#include <stdlib.h>
#include <string.h>
#include "osapi.h"
#include "c_types.h"
#include "gpio.h"
#include "at_gpio.h"
#include "user_interface.h"
#include "ets_sys.h"

void dht_timerfunc(void *arg);
void dht_sensor_init();
#endif