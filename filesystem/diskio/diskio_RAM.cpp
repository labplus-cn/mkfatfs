// Copyright 2015-2017 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string.h>
#include "diskio.h"
#include "ffconf.h"
#include "ff.h"
#include "esp_log.h"
#include "diskio_RAM.h"
#include "FatPartition.h"

static const char* TAG = "ff_diskio_RAM";

FatPartition *part[_VOLUMES] = {NULL, NULL};

RAM_handle_t ff_RAM_handles[_VOLUMES] = {  //支持两个RAM驱动器
        RAM_INVALID_HANDLE,
        RAM_INVALID_HANDLE,
};

DSTATUS ff_RAM_initialize (BYTE pdrv)
{
    return 0;
}

DSTATUS ff_RAM_status (BYTE pdrv)
{
    return 0;
}

DRESULT ff_RAM_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    ESP_LOGV(TAG, "ff_RAM_read - pdrv=%i, sector=%i, count=%i\n", (unsigned int)pdrv, (unsigned int)sector, (unsigned int)count);
    RAM_handle_t RAM_handle = ff_RAM_handles[pdrv];
    assert(RAM_handle + 1);
    esp_err_t err = part[RAM_handle]->read(sector * part[RAM_handle]->sector_size(), buff, count * part[RAM_handle]->sector_size());
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "RAM_read failed (%d)", err);
        return RES_ERROR;
    }
    return RES_OK;
}

DRESULT ff_RAM_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    ESP_LOGV(TAG, "ff_RAM_write - pdrv=%i, sector=%i, count=%i\n", (unsigned int)pdrv, (unsigned int)sector, (unsigned int)count);
    RAM_handle_t RAM_handle = ff_RAM_handles[pdrv];
    assert(RAM_handle + 1);
    esp_err_t err = part[RAM_handle]->erase_range(sector * part[RAM_handle]->sector_size(), count * part[RAM_handle]->sector_size());
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "RAM_erase_range failed (%d)", err);
        return RES_ERROR;
    }
    err = part[RAM_handle]->write(sector * part[RAM_handle]->sector_size(), buff, count * part[RAM_handle] -> sector_size());
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "RAM_write failed (%d)", err);
        return RES_ERROR;
    }
    return RES_OK;
}

DRESULT ff_RAM_ioctl (BYTE pdrv, BYTE cmd, void *buff)
{
    RAM_handle_t RAM_handle = ff_RAM_handles[pdrv];
    ESP_LOGV(TAG, "ff_RAM_ioctl: cmd=%i\n", cmd);
    assert(RAM_handle + 1);
    switch (cmd) {
    case CTRL_SYNC:
        return RES_OK;
    case GET_SECTOR_COUNT:
        *((uint32_t *) buff) = part[RAM_handle]->chip_size() / part[RAM_handle]->sector_size();
        return RES_OK;
    case GET_SECTOR_SIZE:
        *((uint32_t *) buff) = part[RAM_handle]->sector_size();
        return RES_OK;
    case GET_BLOCK_SIZE:
        return RES_ERROR;
    }
    return RES_ERROR;
}


esp_err_t ff_diskio_register_RAM_partition(BYTE pdrv, RAM_handle_t RAM_handle, esp_partition_t *data_partition)
{
    void *part_ptr = NULL;
    if (pdrv >= _VOLUMES) {
        return ESP_ERR_INVALID_ARG;
    }
    static const ff_diskio_impl_t RAM_impl = {
        .init = &ff_RAM_initialize,
        .status = &ff_RAM_status,
        .read = &ff_RAM_read,
        .write = &ff_RAM_write,
        .ioctl = &ff_RAM_ioctl
    };
    ff_RAM_handles[pdrv] = RAM_handle;
    ff_diskio_register(pdrv, &RAM_impl);

    // Allocate memory for a Partition object, and then initialize the object
    // using placement new operator. This way we can recover from out of
    // memory condition.
    part_ptr = malloc(sizeof(FatPartition));
    if (part_ptr == NULL) {
        ESP_LOGE(TAG, "%s: can't allocate FatPartition", __func__);
        return ESP_FAIL;
    }
    part[pdrv] = new (part_ptr) FatPartition(data_partition);
    return ESP_OK;
}

esp_err_t ff_diskio_unregister_RAM_partition(BYTE pdrv, RAM_handle_t RAM_handle)
{
    // delete part[pdrv];
}

BYTE ff_diskio_get_pdrv_RAM(RAM_handle_t RAM_handle)
{
    for (int i = 0; i < _VOLUMES; i++) {
        if (RAM_handle == ff_RAM_handles[i]) {
            return i;
        }
    }
    return 0xff;
}
