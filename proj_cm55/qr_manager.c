#include "qr_manager.h"
#include <stdio.h>
#include <string.h>

void qr_manager_init(qr_manager_t * manager, lv_obj_t * parent_container, uint16_t size)
{
    if (!manager) return;

    manager->parent = parent_container;
    manager->size = size;

    manager->qr_obj = lv_qrcode_create(manager->parent);
    if (!manager->qr_obj)
    {
        printf("QR Manager: Failed to create QR object\n");
        return;
    }

    lv_qrcode_set_size(manager->qr_obj, size);
    lv_qrcode_set_dark_color(manager->qr_obj, lv_color_white());  // White modules
    lv_qrcode_set_light_color(manager->qr_obj, lv_color_black()); // Black background

    // Optional: center inside parent
    lv_obj_center(manager->qr_obj);
}

void qr_manager_update(qr_manager_t * manager, const char * data)
{
    if (!manager || !manager->qr_obj) return;

    lv_result_t res = lv_qrcode_update(manager->qr_obj, data, strlen(data));
    if (res != LV_RESULT_OK)
    {
        printf("QR Manager: Failed to update QR code\n");
    }

//    lv_obj_set_pos(manager->qr_obj, 6, -28);
}

void qr_manager_deinit(qr_manager_t * manager)
{
    if (!manager || !manager->qr_obj) return;

    lv_obj_del(manager->qr_obj);
    manager->qr_obj = NULL;
}
