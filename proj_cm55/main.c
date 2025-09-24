/*******************************************************************************
* File Name        : main.c
*
* Description      : This source file contains the main routine for 
*                    application running on CM55 CPU.
*
* Related Document : See README.md
*
********************************************************************************
* Copyright 2024-2025, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "retarget_io_init.h"
#include "vg_lite.h"
#include "vg_lite_platform.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cyabs_rtos.h"
#include "cyabs_rtos_impl.h"
#include "cy_time.h"

#include "lvgl.h"
#include "ui.h"
#if defined(MTB_DISPLAY_WS7P0DSI_RPI)
#include "mtb_disp_ws7p0dsi_drv.h"
#elif defined(MTB_DISPLAY_EK79007AD3)
#include "mtb_display_ek79007ad3.h"
#elif defined(MTB_DISPLAY_W4P3INCH_RPI)
#include "mtb_disp_dsi_waveshare_4p3.h"
#elif defined(MTB_DISPLAY_R4INCH_TFT)
#include "mtb_display_st7701s.h"
#endif

#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "demos/lv_demos.h"


/*******************************************************************************
* Macros
*******************************************************************************/
#define GPU_INT_PRIORITY                    (3U)
#define DC_INT_PRIORITY                     (3U)

#define GFX_TASK_NAME                       ("CM55 Gfx Task")
/* stack size in words */
#define GFX_TASK_STACK_SIZE                 (configMINIMAL_STACK_SIZE * 16)

#define GFX_TASK_PRIORITY                   (configMAX_PRIORITIES - 1)

#define APP_BUFFER_COUNT                    (2U)
/* 64 KB */
#define DEFAULT_GPU_CMD_BUFFER_SIZE         ((64U) * (1024U))

#if defined(MTB_DISPLAY_W4P3INCH_RPI)
#define DISP_H                              (480U)
#define DISP_W                              (832U)
#elif defined(MTB_DISPLAY_EK79007AD3) || defined(MTB_DISPLAY_WS7P0DSI_RPI)
#define DISP_H                              (600U)
#define DISP_W                              (1024U)
#elif defined(MTB_DISPLAY_R4INCH_TFT)
#define DISP_H                              (480U)
#define DISP_W                              (512U)
#define DISP_RESET_PORT      GPIO_PRT16
#define DISP_RESET_PIN       (6U)
#define PWM_GPIO
#define BACKLIGHT_PORT						 GPIO_PRT20
#define BACKLIGHT_PIN 						(6U)
#endif
#define GPU_TESSELLATION_BUFFER_SIZE        ((DISP_H) * 128U)

#define VGLITE_HEAP_SIZE                    (((DEFAULT_GPU_CMD_BUFFER_SIZE) * \
                                              (APP_BUFFER_COUNT)) + \
                                             ((GPU_TESSELLATION_BUFFER_SIZE) * \
                                              (APP_BUFFER_COUNT)))

#define GPU_MEM_BASE                        (0x0U)
#define I2C_CONTROLLER_IRQ_PRIORITY         (2UL)
#define VG_PARAMS_POS                       (0UL)

/* Enabling or disabling a MCWDT requires a wait time of upto 2 CLK_LF cycles
 * to come into effect. This wait time value will depend on the actual CLK_LF
 * frequency set by the BSP.
 */
#define LPTIMER_1_WAIT_TIME_USEC            (62U)

/* Define the LPTimer interrupt priority number. '1' implies highest priority.*/
#define APP_LPTIMER_INTERRUPT_PRIORITY      (1U)

#if ( configGENERATE_RUN_TIME_STATS == 1 )
#define TCPWM_TIMER_INT_PRIORITY            (1U)
#endif


/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Heap memory for VGLite to allocate memory for buffers, command, and
 * tessellation buffers 
 */
CY_SECTION(".cy_gpu_buf") uint8_t contiguous_mem[VGLITE_HEAP_SIZE] = { 0xFF };

volatile void *vglite_heap_base = &contiguous_mem;

TaskHandle_t rtos_cm55_gfx_task_handle = NULL;

/* DC IRQ Config */
cy_stc_sysint_t dc_irq_cfg =
{
    .intrSrc      = GFXSS_DC_IRQ,
    .intrPriority = DC_INT_PRIORITY
};

/* GPU IRQ Config */
cy_stc_sysint_t gpu_irq_cfg =
{
    .intrSrc      = GFXSS_GPU_IRQ,
    .intrPriority = GPU_INT_PRIORITY
};

cy_stc_scb_i2c_context_t disp_touch_i2c_controller_context;

#if defined(MTB_DISPLAY_R4INCH_TFT)
cy_stc_sysint_t disp_touch_i2c_controller_irq_cfg =
{
    .intrSrc      = CYBSP_I2C_CONTROLLER_2_IRQ,
    .intrPriority = I2C_CONTROLLER_IRQ_PRIORITY,
};
#else
cy_stc_sysint_t disp_touch_i2c_controller_irq_cfg =
{
    .intrSrc      = CYBSP_I2C_CONTROLLER_IRQ,
    .intrPriority = I2C_CONTROLLER_IRQ_PRIORITY,
};
#endif

#if defined(MTB_DISPLAY_EK79007AD3)
mtb_display_ek79007ad3_pin_config_t ek79007ad3_pin_cfg =
{
    .reset_port = CYBSP_DISP_RST_PORT,
    .reset_pin  = CYBSP_DISP_RST_PIN,
};
#elif defined(MTB_DISPLAY_R4INCH_TFT)
mtb_display_st7701s_pin_config_t st7701s_pin_cfg =
{
    .reset_port = CYBSP_DISP_RST_PORT,
    .reset_pin  = CYBSP_DISP_RST_PIN,
};

mtb_display_st7701s_backlight_config_t st7701s_pwm_cfg =
{
	.bl_port = 0 ,
	.bl_pin = 0 ,
	.pwm_hw = TCPWM0 ,
	.pwm_num = CYBSP_PWM_DISP_BACKLIGHT_NUM ,
	.pwm_config = &CYBSP_PWM_DISP_BACKLIGHT_config,
};
#endif

/* LPTimer HAL object */
static mtb_hal_lptimer_t lptimer_obj;
uint8_t brightness_level = 100;
/* RTC HAL object */
static mtb_hal_rtc_t rtc_obj;

#if ( configGENERATE_RUN_TIME_STATS == 1 )
/*******************************************************************************
* Function Name: setup_run_time_stats_timer
********************************************************************************
* Summary:
*  This function configuresTCPWM 0 GRP 0 Counter 0 as the timer source for  
*  FreeRTOS runtime statistics.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void setup_run_time_stats_timer(void)
{
    /* Initialze TCPWM block with required timer configuration */
    if (CY_TCPWM_SUCCESS != Cy_TCPWM_Counter_Init(CYBSP_GENERAL_PURPOSE_TIMER_HW, 
        CYBSP_GENERAL_PURPOSE_TIMER_NUM, 
        &CYBSP_GENERAL_PURPOSE_TIMER_config))
    {
        handle_app_error();
    }

    /* Enable the initialized counter */
    Cy_TCPWM_Counter_Enable(CYBSP_GENERAL_PURPOSE_TIMER_HW, 
                            CYBSP_GENERAL_PURPOSE_TIMER_NUM);

    /* Start the counter */
    Cy_TCPWM_TriggerStart_Single(CYBSP_GENERAL_PURPOSE_TIMER_HW, 
                                 CYBSP_GENERAL_PURPOSE_TIMER_NUM);
}


/*******************************************************************************
* Function Name: get_run_time_counter_value
********************************************************************************
* Summary:
*  Function to fetch run time counter value. This will be used by FreeRTOS for 
*  run time statistics calculation.
*
* Parameters:
*  void
*
* Return:
*  uint32_t: TCPWM 0 GRP 0 Counter 0 value
*
*******************************************************************************/
uint32_t get_run_time_counter_value(void)
{
   return (Cy_TCPWM_Counter_GetCounter(CYBSP_GENERAL_PURPOSE_TIMER_HW, 
                                       CYBSP_GENERAL_PURPOSE_TIMER_NUM));
}


/*******************************************************************************
* Function Name: calculate_idle_percentage
********************************************************************************
* Summary:
*  Function to calculate CPU idle percentage. This function is used by LVGL to  
*  showcase CPU usage.
*
* Parameters:
*  void
*
* Return:
*  uint32_t: CPU idle percentage
*
*******************************************************************************/
uint32_t calculate_idle_percentage(void)
{

    static uint32_t previousIdleTime = 0;
    static TickType_t previousTick = 0;
    uint32_t time_diff = 0;
    uint32_t idle_percent = 0;

    uint32_t currentIdleTime = ulTaskGetIdleRunTimeCounter();
    TickType_t currentTick = portGET_RUN_TIME_COUNTER_VALUE();

    time_diff = currentTick - previousTick;

    if((currentIdleTime >= previousIdleTime) && (currentTick > previousTick))
    {
        idle_percent = ((currentIdleTime - previousIdleTime) * 100)/time_diff;
    }

    previousIdleTime = ulTaskGetIdleRunTimeCounter();
    previousTick = portGET_RUN_TIME_COUNTER_VALUE();

    return idle_percent;
}
#endif

/*******************************************************************************
* Function Name: lptimer_interrupt_handler
********************************************************************************
* Summary:
*  Interrupt handler function for LPTimer instance.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void lptimer_interrupt_handler(void)
{
    mtb_hal_lptimer_process_interrupt(&lptimer_obj);
}


/*******************************************************************************
* Function Name: setup_tickless_idle_timer
********************************************************************************
* Summary:
*  1. This function first configures and initializes an interrupt for LPTimer.
*  2. Then it initializes the LPTimer HAL object to be used in the RTOS
*     tickless idle mode implementation to allow the device enter deep sleep
*     when idle task runs. LPTIMER_1 instance is configured for CM55 CPU.
*  3. It then passes the LPTimer object to abstraction RTOS library that
*     implements tickless idle mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void setup_tickless_idle_timer(void)
{
    /* Interrupt configuration structure for LPTimer */
    cy_stc_sysint_t lptimer_intr_cfg =
    {
        .intrSrc = CYBSP_CM55_LPTIMER_1_IRQ,
        .intrPriority = APP_LPTIMER_INTERRUPT_PRIORITY
    };

    /* Initialize the LPTimer interrupt and specify the interrupt handler. */
    cy_en_sysint_status_t interrupt_init_status =
                                    Cy_SysInt_Init(&lptimer_intr_cfg,
                                                    lptimer_interrupt_handler);

    /* LPTimer interrupt initialization failed. Stop program execution. */
    if(CY_SYSINT_SUCCESS != interrupt_init_status)
    {
        handle_app_error();
    }

    /* Enable NVIC interrupt. */
    NVIC_EnableIRQ(lptimer_intr_cfg.intrSrc);

    /* Initialize the MCWDT block */
    cy_en_mcwdt_status_t mcwdt_init_status =
                                    Cy_MCWDT_Init(CYBSP_CM55_LPTIMER_1_HW,
                                                &CYBSP_CM55_LPTIMER_1_config);

    /* MCWDT initialization failed. Stop program execution. */
    if(CY_MCWDT_SUCCESS != mcwdt_init_status)
    {
        handle_app_error();
    }

    /* Enable MCWDT instance */
    Cy_MCWDT_Enable(CYBSP_CM55_LPTIMER_1_HW,
                    CY_MCWDT_CTR_Msk,
                    LPTIMER_1_WAIT_TIME_USEC);

    /* Setup LPTimer using the HAL object and desired configuration as defined
     * in the device configurator. */
    cy_rslt_t result = mtb_hal_lptimer_setup(&lptimer_obj,
                                            &CYBSP_CM55_LPTIMER_1_hal_config);

    /* LPTimer setup failed. Stop program execution. */
    if(CY_RSLT_SUCCESS != result)
    {
        handle_app_error();
    }

    /* Pass the LPTimer object to abstraction RTOS library that implements
     * tickless idle mode
     */
    cyabs_rtos_set_lptimer(&lptimer_obj);
}


/*******************************************************************************
* Function Name: dc_irq_handler
********************************************************************************
* Summary:
*  Display Controller interrupt handler which gets invoked when the DC finishes
*  utilizing the current frame buffer.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void dc_irq_handler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    Cy_GFXSS_Clear_DC_Interrupt(GFXSS, &gfx_context);

    /* Notify the cm55_gfx_task */
    xTaskNotifyFromISR(rtos_cm55_gfx_task_handle, 1, eSetValueWithOverwrite, 
                       &xHigherPriorityTaskWoken);

    /* Perform a context switch if a higher-priority task was woken */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


/*******************************************************************************
* Function Name: gpu_irq_handler
********************************************************************************
* Summary:
*  GPU interrupt handler which gets invoked when the GPU finishes composing
*  a frame.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void gpu_irq_handler(void)
{
    Cy_GFXSS_Clear_GPU_Interrupt(GFXSS, &gfx_context);
    vg_lite_IRQHandler();
}


/*******************************************************************************
* Function Name: disp_touch_i2c_controller_interrupt
********************************************************************************
* Summary:
*  I2C controller ISR which invokes Cy_SCB_I2C_Interrupt to perform I2C transfer
*  as controller.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void disp_touch_i2c_controller_interrupt(void)
{
#if defined(MTB_DISPLAY_R4INCH_TFT)
    Cy_SCB_I2C_Interrupt(CYBSP_I2C_CONTROLLER_2_HW, &disp_touch_i2c_controller_context);
#else
	Cy_SCB_I2C_Interrupt(CYBSP_I2C_CONTROLLER_HW, &disp_touch_i2c_controller_context);
#endif
}


/*******************************************************************************
* Function Name: cm55_gfx_task
********************************************************************************
* Summary:
*   This is the FreeRTOS task callback function.
*   It initializes:  
*       - GFX subsystem.
*       - Configure the DC, GPU interrupts.
*       - Initialize I2C interface to be used for touch as well as 7-inch display
*         drivers. 
*       - Initializes the display panel selected through Makefile component and
*         vglite driver.
*       - Allocates vglite memory.
*       - Configures LVGL, low level display and touch driver.
*       - Finally invokes the UI application.
*
* Parameters:
*  void *arg: Pointer to the argument passed to the task (not used)
*
* Return:
*  void
*
*******************************************************************************/
static void cm55_gfx_task(void *arg)
{
    CY_UNUSED_PARAMETER(arg);

    uint32_t time_till_next = 0;

    cy_en_sysint_status_t sysint_status = CY_SYSINT_SUCCESS;
    cy_en_gfx_status_t gfx_status = CY_GFX_SUCCESS;
    vg_lite_error_t vglite_status = VG_LITE_SUCCESS;

#if defined(MTB_DISPLAY_WS7P0DSI_RPI)
    cy_rslt_t status = CY_RSLT_SUCCESS;
#elif defined(MTB_DISPLAY_EK79007AD3) || defined(MTB_DISPLAY_R4INCH_TFT)
    cy_en_mipidsi_status_t mipi_status = CY_MIPIDSI_SUCCESS;
#endif

    cy_en_scb_i2c_status_t i2c_result = CY_SCB_I2C_SUCCESS;

    /* GFXSS init */
    /* MIPI-DSI Display specific configs */
#if defined(MTB_DISPLAY_WS7P0DSI_RPI)
    GFXSS_config.mipi_dsi_cfg = &mtb_disp_ws7p0dsi_dsi_config;
#elif defined(MTB_DISPLAY_EK79007AD3)
    GFXSS_config.mipi_dsi_cfg = &mtb_display_ek79007ad3_mipidsi_config;
#elif defined(MTB_DISPLAY_W4P3INCH_RPI)
    GFXSS_config.mipi_dsi_cfg = &mtb_disp_waveshare_4p3_dsi_config;
#endif

    /* Set frame buffer address to the GFXSS configuration structure */
    GFXSS_config.dc_cfg->gfx_layer_config->buffer_address    = frame_buffer1;
    GFXSS_config.dc_cfg->gfx_layer_config->uv_buffer_address = frame_buffer1;

//    GFXSS_config.dc_cfg->gfx_layer_config->width = DISP_W;
//    GFXSS_config.dc_cfg->gfx_layer_config->height = DISP_H;

    /* Initialize Graphics subsystem as per the configuration */
    gfx_status = Cy_GFXSS_Init(GFXSS, &GFXSS_config, &gfx_context);

    if (CY_GFX_SUCCESS == gfx_status)
    {

        /* Initialize GFXSS DC interrupt */
        sysint_status = Cy_SysInt_Init(&dc_irq_cfg, dc_irq_handler);
 
        if (CY_SYSINT_SUCCESS != sysint_status)
        {
            printf("Error in registering DC interrupt: %d\r\n", sysint_status);
            handle_app_error();
        }

        /* Enable GFX DC interrupt in NVIC. */
        NVIC_EnableIRQ(GFXSS_DC_IRQ);

        /* Initialize GFX GPU interrupt */
        sysint_status = Cy_SysInt_Init(&gpu_irq_cfg, gpu_irq_handler);

        if (CY_SYSINT_SUCCESS != sysint_status)
        {
            printf("Error in registering GPU interrupt: %d\r\n", sysint_status);
            handle_app_error();
        }
 
        /* Enable GPU interrupt */
        Cy_GFXSS_Enable_GPU_Interrupt(GFXSS);

        /* Enable GFX GPU interrupt in NVIC. */
        NVIC_EnableIRQ(GFXSS_GPU_IRQ);
#if defined(MTB_DISPLAY_R4INCH_TFT)
        /* Initialize the I2C in controller mode. */
        i2c_result = Cy_SCB_I2C_Init(CYBSP_I2C_CONTROLLER_2_HW,
                    &CYBSP_I2C_CONTROLLER_2_config, &disp_touch_i2c_controller_context);
#else
        i2c_result = Cy_SCB_I2C_Init(CYBSP_I2C_CONTROLLER_HW,
                    &CYBSP_I2C_CONTROLLER_config, &disp_touch_i2c_controller_context);
#endif

        if (CY_SCB_I2C_SUCCESS != i2c_result)
        {
            printf("I2C controller initialization failed !!\n");
            handle_app_error();
        }

        /* Initialize the I2C interrupt */
        sysint_status = Cy_SysInt_Init(&disp_touch_i2c_controller_irq_cfg,
                                       &disp_touch_i2c_controller_interrupt);

        if (CY_SYSINT_SUCCESS != sysint_status)
        {
            printf("I2C controller interrupt initialization failed\r\n");
            handle_app_error();
        }

        /* Enable the I2C interrupts. */
        NVIC_EnableIRQ(disp_touch_i2c_controller_irq_cfg.intrSrc);

#if defined(MTB_DISPLAY_R4INCH_TFT)
        /* Enable the I2C */
        Cy_SCB_I2C_Enable(CYBSP_I2C_CONTROLLER_2_HW);
#else
        /* Enable the I2C */
        Cy_SCB_I2C_Enable(CYBSP_I2C_CONTROLLER_HW);
#endif

        vTaskDelay(pdMS_TO_TICKS(500));

#if defined(MTB_DISPLAY_R4INCH_TFT)
		/* Initialize the R4INCH display */
		mipi_status =  mtb_display_st7701s_init(GFXSS_GFXSS_MIPIDSI,&st7701s_pin_cfg);
        if(CY_MIPIDSI_SUCCESS != mipi_status)
        {
            printf("st7701s 4-inch display init failed with status = %d\r\n", mipi_status);
            CY_ASSERT(0);
        }

        if (CY_TCPWM_SUCCESS !=
                mtb_display_st7701s_backlight_init(&st7701s_pwm_cfg))
        {
            /* Handle possible errors */
        printf("failed pwm init");
            CY_ASSERT(0);
        }
        mtb_display_st7701s_set_brightness(brightness_level);
#endif
        /* Allocate memory for VGLite from the vglite_heap_base */
        vg_module_parameters_t vg_params;
        vg_params.register_mem_base = (uint32_t)GFXSS_GFXSS_GPU_GCNANO;
        vg_params.gpu_mem_base[VG_PARAMS_POS] = GPU_MEM_BASE;
        vg_params.contiguous_mem_base[VG_PARAMS_POS] = vglite_heap_base;
        vg_params.contiguous_mem_size[VG_PARAMS_POS] = VGLITE_HEAP_SIZE;

        /* Initialize VGlite memory. */
        vg_lite_init_mem(&vg_params);

        /* Initialize the memory and data structures needed for VGLite draw/blit
         * functions
         */
        vglite_status = vg_lite_init((MY_DISP_HOR_RES) / 4,
                             (MY_DISP_VER_RES) / 4);

        if (VG_LITE_SUCCESS == vglite_status)
        {
            /* Initialize LVGL library */
            lv_init();
            lv_port_disp_init();
            lv_port_indev_init();
            ui_demo_init();

        }
        else
        {
            printf("vg_lite_init failed, status: %d\r\n", vglite_status);

            /* Deallocate all the resources and free up all the memory */
            vg_lite_close();
            handle_app_error();
        }
    }
    else
    {
        printf("Graphics subsystem init failed, status: %d\r\n", gfx_status);
        handle_app_error();
    }

    for (;;)
    {
        /* LVGL's timer handler function, to be called periodically to handle
         * LVGL tasks.
         */
        time_till_next = lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(time_till_next));
    }
}

/*******************************************************************************
* Function Name: setup_clib_support
********************************************************************************
* Summary:
*    1. This function configures and initializes the Real-Time Clock (RTC)).
*    2. It then initializes the RTC HAL object to enable CLIB support library 
*       to work with the provided Real-Time Clock (RTC) module.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void setup_clib_support(void)
{
    /* RTC Initialization is done in CM33 non-secure project */

    /* Initialize the ModusToolbox CLIB support library */
    mtb_clib_support_init(&rtc_obj);
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  This is the main function for CM55 non-secure application.
*    1. It initializes the device and board peripherals.
*    2. It sets up the LPtimer instance for CM55 CPU and initializes debug UART. 
*    3. It creates the FreeRTOS application task 'cm55_gfx_task'.
*    4. It starts the RTOS task scheduler.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result       = CY_RSLT_SUCCESS;
    BaseType_t task_return = pdFAIL;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (CY_RSLT_SUCCESS != result)
    {
        handle_app_error();
    }

    /* Setup CLIB support library. */
    setup_clib_support();
    /* Setup the LPTimer instance for CM55 */
    setup_tickless_idle_timer();

    /* Initialize retarget-io middleware */
    init_retarget_io();

    /* Enable global interrupts */
    __enable_irq();

    /* Create the FreeRTOS Task */
    task_return = xTaskCreate(cm55_gfx_task, GFX_TASK_NAME,
                              GFX_TASK_STACK_SIZE, NULL,
                              GFX_TASK_PRIORITY, &rtos_cm55_gfx_task_handle);

    /* ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    if (pdPASS == task_return)
    {
        printf("****************** "
               "PSOC Edge MCU: Graphics LVGL Demo "
               "****************** \r\n\n");

        /* Start the RTOS Scheduler */
        vTaskStartScheduler();

        /* Should never get here! */
        handle_app_error();
    }
    else
    {
        printf("Error: Failed to create cm55_gfx_task.\r\n");
        handle_app_error();
    }
}


/* [] END OF FILE */
