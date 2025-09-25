/*
 * app_radar.h
 *
 *  Created on: 06-Aug-2025
 *      Author: Tejas.Patel
 */

#ifndef APP_RADAR_H_
#define APP_RADAR_H_

#include "cybsp.h"
#include <inttypes.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cyabs_rtos.h"
#include "cyabs_rtos_impl.h"


int32_t init_radar_sensor(void);

void start_radar_accquisition_task(void);
void start_radar_processing_task(void);


#endif /* APP_RADAR_H_ */
