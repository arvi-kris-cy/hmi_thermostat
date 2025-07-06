#ifndef QR_MANAGER_H
#define QR_MANAGER_H

#include "lvgl.h"

// QR Manager structure
typedef struct {
    lv_obj_t * qr_obj;      // QR code LVGL object
    lv_obj_t * parent;      // Parent container
    uint16_t size;          // QR code size
} qr_manager_t;

// Initialize QR Manager (create QR object)
void qr_manager_init(qr_manager_t * manager, lv_obj_t * parent_container, uint16_t size);

// Update QR Code data
void qr_manager_update(qr_manager_t * manager, const char * data);

// Deinitialize QR Manager (optional, only if you want to clean up)
void qr_manager_deinit(qr_manager_t * manager);

#endif
