/*
 * app_ui.c
 *
 *  Created on: 05-Jun-2025
 *      Author: Tejas.Patel
 */


/*******************************************************************************
 *                                INCLUDES
 ******************************************************************************/
#include "app_ui.h"

/*******************************************************************************
 *                              CONSTANTS
 ******************************************************************************/
#define UI_EVT_QUEUE_LENGTH   (10)

#define UI_TASK_NAME                       ("CM55 UI Task")
/* stack size in words */
#define UI_TASK_STACK_SIZE                 (configMINIMAL_STACK_SIZE * 32)

#define UI_TASK_PRIORITY                   (configMAX_PRIORITIES - 1)

/*******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************/
/* Queue handle for communication with the BLE task */
static QueueHandle_t app_ui_q;

TaskHandle_t rtos_cm55_ui_task_handle = NULL;

/*******************************************************************************
 *                             STATIC VARIABLES
 ******************************************************************************/


/****************************************************************************
 *                              FUNCTION DECLARATIONS
 ***************************************************************************/
static void ui_app_event_handler(void *evt);

/*******************************************************************************
 *                              FUNCTION DEFINITIONS
 ******************************************************************************/

static void ui_app_event_handler(void *evt) {
	app_event_t *q_evt = (app_event_t*) evt;

	switch (q_evt->data) {
	case UI_UPDATE_BLE_CONNECTED:
		printf("Received UI_UPDATE_BLE_CONNECTED\n");
		//TODO Call BLE advertising function
		break;

	default:
		printf("Received Unhandled UI event.\n");
		break;
	}

}

void ui_task(void *arg) {
	(void) arg;
	app_event_t event;

	/* Initialize UI task queue */
	app_ui_q = xQueueCreate(UI_EVT_QUEUE_LENGTH, sizeof(app_event_t));
	if (app_ui_q == NULL) {
		printf("UI app. event queue creation Error.\n");
	} else {
		printf("UI app. event queue creation OK.\n");
	}

	while (1) {
		/* Wait for events in task queue (block indefinitely) */
		if (xQueueReceive(app_ui_q, &event, portMAX_DELAY) == pdTRUE)
		{
			printf("Received UI event: %d\n", event.type);
			switch (event.type)
			{
				case EVT_UI_UPDATE:
					printf("Received BLE event : %d.\n", event.type);
					/* call ble event handler */
					event.Handler(&event);
					break;

				default:
					printf("Received BLE Event not handled : %d.\n", event.type);
					break;
			}

		}
	}
}

void put_ui_evt_task_q(event_type_t event, uint8_t data)
{
    app_event_t q_evt;

    /* Load event details */
    q_evt.type = event;
    q_evt.data = data;
    q_evt.Handler = ui_app_event_handler;  // Replace with appropriate handler if needed

    /* Send to queue with 100 ms timeout */
    if (xQueueSend(app_ui_q, &q_evt, pdMS_TO_TICKS(100)) != pdPASS)
    {
        printf("UI app. evt queue put Error\n");
    }
    else
    {
        printf("UI app. evt queue put OK (type: %d, data: %d)\n", event, data);
    }
}

void ui_thread_init(void) {
	BaseType_t task_return = pdFAIL;

	/* Create the FreeRTOS Task */
	task_return = xTaskCreate(ui_task, UI_TASK_NAME,
		UI_TASK_STACK_SIZE, NULL,
		UI_TASK_PRIORITY, &rtos_cm55_ui_task_handle);
	if (task_return != pdPASS) {
		printf("UI task create Error.\n");
	} else {
		printf("UI task create OK.\r\n");
	}
}




