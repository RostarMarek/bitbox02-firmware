// Copyright 2023-2024 Shift Crypto AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "keyrotation.h"

#include "driver_init.h"
#include <flags.h>
#include <memory/memory.h>
#include <memory/memory_shared.h>
#include <memory/mpu.h>

#include <screen.h>
#include <system.h>

#include <string.h>

static const version_t _new_fw_version = 37;

// clang-format off
#if PRODUCT_BITBOX_BTCONLY == 1
static const uint8_t _old_root_pubkeys[3][64] = {
    {
        0xe7, 0x30, 0x92, 0x3f, 0x19, 0x04, 0xd3, 0xe8, 0x3e, 0x0a, 0xb0, 0x7f, 0x05, 0x0d, 0x8e, 0x4e,
        0x60, 0x4c, 0x46, 0xba, 0x36, 0xae, 0xbb, 0xdb, 0x9a, 0x4d, 0x3c, 0x96, 0x60, 0x5b, 0x9e, 0xc3,
        0x94, 0x71, 0x03, 0x5c, 0x0f, 0x87, 0x34, 0xbd, 0xcb, 0xe1, 0x8f, 0x0c, 0xaa, 0x6f, 0x6e, 0x3d,
        0x8b, 0x7f, 0xa0, 0x5e, 0x42, 0x5a, 0xc2, 0x21, 0x5e, 0x6e, 0x97, 0x97, 0x5f, 0x48, 0x03, 0x8b,
    },
    {
        0xfd, 0x11, 0xed, 0xc3, 0xb6, 0xd8, 0xe5, 0x0b, 0xe8, 0x6b, 0x2e, 0x33, 0xee, 0xb0, 0x3b, 0x32,
        0x23, 0x91, 0x4c, 0x12, 0xe4, 0x9d, 0x0e, 0x64, 0xd9, 0x74, 0x03, 0xa7, 0x31, 0xc4, 0x30, 0x6a,
        0x44, 0xab, 0x6a, 0x99, 0x3e, 0x9d, 0xaa, 0x44, 0xda, 0x0e, 0x5a, 0x0a, 0x8b, 0x3e, 0x02, 0xe9,
        0xa4, 0x6e, 0x2c, 0xb6, 0x14, 0x57, 0xce, 0x78, 0x81, 0xa8, 0x71, 0x55, 0xee, 0x23, 0x67, 0x44,
    },
    {
        0x75, 0x98, 0x7e, 0x69, 0xa5, 0xed, 0xa5, 0x3f, 0x79, 0x63, 0x16, 0xfa, 0x47, 0x00, 0xf9, 0x9a,
        0x86, 0x36, 0xb0, 0xa5, 0x6c, 0x57, 0x28, 0xee, 0x8a, 0xd3, 0xb3, 0xcc, 0x8f, 0x37, 0xe6, 0xac,
        0xfc, 0xa6, 0x08, 0x23, 0x14, 0x4a, 0xea, 0xb2, 0xe4, 0xa7, 0x62, 0x97, 0x89, 0xd0, 0x3e, 0xa4,
        0xd2, 0xd1, 0x8a, 0xbf, 0x0f, 0xd7, 0xa0, 0x07, 0xd7, 0x96, 0xa1, 0x65, 0x57, 0x28, 0x4b, 0x3f,
    },
};

static const uint8_t _new_root_pubkeys[3][64] = {
    {
        0x56, 0x82, 0xcc, 0xed, 0x54, 0x4e, 0xa6, 0xa1, 0x8f, 0x9e, 0x7c, 0x48, 0x40, 0xb8, 0x6d, 0x3d,
        0x51, 0x4e, 0x49, 0x4a, 0x9f, 0x20, 0xde, 0xe7, 0x6b, 0x5a, 0x99, 0x2c, 0xe1, 0x3e, 0x77, 0xa9,
        0x8a, 0x61, 0xe2, 0x34, 0x3e, 0x1f, 0x9e, 0xc7, 0x27, 0x7f, 0xf7, 0x50, 0xf2, 0x07, 0x09, 0x3a,
        0xa0, 0xba, 0x36, 0x31, 0xa4, 0x0f, 0xcd, 0x5a, 0xd6, 0xd0, 0xaf, 0x38, 0x44, 0x19, 0xc8, 0x86,
    },
    {
        0x3b, 0x13, 0x86, 0x55, 0x8f, 0xc8, 0x31, 0xd6, 0x3a, 0x30, 0x2b, 0x30, 0x84, 0xbf, 0x0a, 0xde,
        0x07, 0x8b, 0xf6, 0x08, 0xec, 0x15, 0x20, 0x8c, 0x0f, 0xb9, 0x51, 0x7d, 0xbc, 0xb4, 0x48, 0xe6,
        0x33, 0xb6, 0x40, 0xf3, 0xb6, 0x19, 0xbe, 0x9b, 0x94, 0x94, 0x4b, 0x80, 0x4f, 0xf6, 0x12, 0x1a,
        0xcd, 0x0a, 0x4d, 0xe7, 0x6b, 0x60, 0x12, 0x64, 0x2f, 0x7a, 0x62, 0x65, 0x2e, 0xc0, 0x44, 0x65,
    },
    {
        0x93, 0x13, 0x34, 0xe4, 0x43, 0x6e, 0x4d, 0x41, 0xfe, 0x2a, 0xee, 0xbd, 0xf1, 0x25, 0x1f, 0x08,
        0x35, 0xb2, 0xca, 0x5a, 0x9b, 0xc5, 0xca, 0x5b, 0x12, 0xcb, 0x72, 0xf9, 0xf7, 0xbf, 0xb4, 0x6f,
        0x73, 0xf5, 0xe2, 0x3e, 0x93, 0x45, 0x50, 0x2c, 0xe0, 0xaf, 0xce, 0x7b, 0xd4, 0x12, 0x56, 0xa2,
        0xde, 0x34, 0x43, 0x8e, 0x71, 0xdf, 0x99, 0xeb, 0x59, 0xb4, 0x1e, 0xb1, 0x32, 0x17, 0xda, 0x8a,
    },
};

#elif PRODUCT_BITBOX_MULTI == 1
static const uint8_t _old_root_pubkeys[3][64] = {
    {
        0x2b, 0x8a, 0x21, 0x9f, 0x01, 0x57, 0xf3, 0x12, 0x3e, 0x3c, 0x69, 0xd5, 0x40, 0x0e, 0xff, 0xfe,
        0xd9, 0x55, 0x3f, 0x64, 0xd2, 0x5e, 0xec, 0x10, 0x65, 0x7d, 0x0e, 0x77, 0x6a, 0x8e, 0x17, 0xe2,
        0xac, 0x2c, 0x3d, 0x19, 0xca, 0xfd, 0x18, 0x9b, 0x62, 0x36, 0x68, 0xc4, 0x10, 0x1b, 0x9b, 0x96,
        0x74, 0xf5, 0xe6, 0xe6, 0x43, 0xa3, 0x82, 0x8e, 0x6a, 0xb9, 0x3b, 0x46, 0xe9, 0xc3, 0x87, 0x73,
    },
    {
        0x1e, 0xb6, 0xe7, 0xad, 0x36, 0x1c, 0x40, 0x6b, 0xdd, 0xf4, 0x47, 0x17, 0x7b, 0x38, 0x21, 0x52,
        0x7a, 0x7a, 0x25, 0x75, 0x99, 0x5c, 0x6f, 0x49, 0xc2, 0xd8, 0xe0, 0x9b, 0x89, 0x7a, 0x70, 0xb7,
        0xab, 0x9c, 0x53, 0xa4, 0xcf, 0xbe, 0x29, 0x13, 0x33, 0x27, 0x1e, 0xd6, 0x75, 0x6b, 0xd3, 0xad,
        0x20, 0x77, 0x08, 0x2d, 0xe7, 0x1a, 0xcd, 0xc8, 0xb8, 0x7c, 0x39, 0x2d, 0x94, 0x64, 0xef, 0x51,
    },
    {
        0x9f, 0x39, 0x3d, 0x4a, 0xb6, 0xe8, 0xb0, 0xda, 0xc7, 0xc0, 0x43, 0x62, 0xc5, 0x20, 0xe1, 0xbc,
        0x1b, 0x32, 0x7d, 0x92, 0xee, 0x25, 0x6b, 0xc4, 0x07, 0xf6, 0x82, 0xff, 0xac, 0x12, 0x0d, 0xec,
        0x6b, 0x80, 0x23, 0x28, 0x62, 0xb9, 0x7d, 0xfd, 0x3d, 0xcf, 0x21, 0xdb, 0xb3, 0xf5, 0x4e, 0x4c,
        0x1b, 0xec, 0x2b, 0x99, 0x40, 0x61, 0x3e, 0xef, 0xe7, 0x9e, 0x6a, 0x2b, 0x10, 0xbe, 0xda, 0xc2,
    },
};

static const uint8_t _new_root_pubkeys[3][64] = {
    {
        0x08, 0xa6, 0xdc, 0x5f, 0x9b, 0x9e, 0x0c, 0x74, 0x25, 0x06, 0x3d, 0x00, 0x77, 0x66, 0xe1, 0x69,
        0x0a, 0x57, 0xe7, 0x2d, 0xdb, 0xab, 0xa6, 0x4e, 0x3d, 0x88, 0x75, 0x41, 0x6d, 0xd1, 0x86, 0x37,
        0x9e, 0x01, 0x8c, 0x2a, 0xd1, 0xcf, 0x01, 0xf7, 0x0f, 0x92, 0x5c, 0x18, 0x4f, 0x64, 0x36, 0xa9,
        0xc3, 0xf8, 0x9a, 0x9c, 0x75, 0x9c, 0x92, 0xdb, 0x6a, 0x1a, 0x75, 0xcb, 0x00, 0xb0, 0x26, 0x88,
    },
    {
        0xf5, 0xb9, 0xd3, 0xa8, 0x43, 0x99, 0x2c, 0xb2, 0x5a, 0xcc, 0xd4, 0x20, 0xb8, 0x24, 0x65, 0x46,
        0x77, 0xa2, 0x03, 0xb0, 0x11, 0x68, 0xdb, 0x97, 0x26, 0x8d, 0xe4, 0xd5, 0xd1, 0x94, 0x28, 0x95,
        0x09, 0x3d, 0x22, 0x7e, 0x57, 0x8f, 0x19, 0x4f, 0x2c, 0xd8, 0x45, 0x05, 0x83, 0xdf, 0xe8, 0xfe,
        0xfd, 0x41, 0xdd, 0xb6, 0x7b, 0x05, 0xfe, 0xc1, 0x32, 0xfa, 0xc1, 0x51, 0xe1, 0xbb, 0x44, 0xc7,
    },
    {
        0xa9, 0x1a, 0x8e, 0xc6, 0x46, 0xfc, 0x37, 0x41, 0x64, 0xb5, 0xdc, 0xbf, 0x29, 0x80, 0xfd, 0xbf,
        0xbc, 0xd1, 0x2b, 0x57, 0xaf, 0xa0, 0x29, 0xa4, 0x05, 0x5d, 0x7f, 0x9a, 0x81, 0x75, 0x0f, 0x18,
        0xfc, 0x13, 0x48, 0xdc, 0xda, 0xbd, 0x6e, 0x33, 0x25, 0x5b, 0x29, 0xa5, 0xb7, 0x51, 0x16, 0xbf,
        0xf0, 0xca, 0xde, 0x45, 0xd6, 0x1c, 0x51, 0x4d, 0x86, 0x09, 0xfc, 0xa7, 0x64, 0x1c, 0x9e, 0xe2,
    }
};
#else
#error "unknown product"
#endif
// clang-format on

static bool _write_chunk(uint32_t address, const uint8_t* data)
{
    const uint32_t lock_size = FLASH_ERASE_PAGE_NUM * FLASH_PAGE_SIZE;
    if (flash_unlock(&FLASH_0, address & ~(lock_size - 1), FLASH_REGION_PAGE_NUM) !=
        FLASH_REGION_PAGE_NUM) {
        return false;
    }
    // Erase is handled inside of flash_write
    if (flash_write(&FLASH_0, address, data, FLASH_BOOTDATA_LEN) != ERR_NONE) {
        return false;
    }
    if (flash_lock(&FLASH_0, address & ~(lock_size - 1), FLASH_REGION_PAGE_NUM) !=
        FLASH_REGION_PAGE_NUM) {
        return false;
    }
    return true;
}

static void _disable_mpu(void)
{
    /* Disable MPU */

    // Disable interrupts
    __disable_irq();

    // Wait for all memory accesses and instructions to complete
    __DSB();
    __ISB();

    MPU->CTRL = MPU_DISABLE;

    // Wait for all memory accesses and instructions to complete
    __DSB();
    __ISB();

    // Enable interrupts
    __enable_irq();
    /**************/
}

void keyrotation(void)
{
    // We rotate the screen here because this function is called before the orientation screen and
    // reboots in every case. As a result, when we call `reboot()` (which stores the orientation for
    // the bootloader), the bootloader is oriented the same way as before.
    chunk_shared_t shared_data;
    memory_read_shared_bootdata(&shared_data);
    if (shared_data.fields.upside_down) {
        screen_rotate();
    }

    bool new_rootkeys_found = false;
    // Iterate from 1 to not derefence 0.
    // Don't iterate the last 32 bytes, which is where the factory randomness lives.
    for (uint32_t ptr = 1; ptr < FLASH_BOOT_LEN - (64 * 3) - 32; ptr++) {
        if (!memcmp((void*)ptr, _new_root_pubkeys, 64 * 3)) {
            new_rootkeys_found = true;
            break;
        }
        if (!memcmp((void*)ptr, _old_root_pubkeys, 64 * 3)) {
            _disable_mpu();

            if (!memory_set_attestation_bootloader_hash()) {
                screen_print_debug("could not persist attestation hash", 5000);
                reboot();
                return;
            }

            /* Disable boot protection */

            // Check if NVMCTRL is ready
            while (NVMCTRL->STATUS.bit.READY == 0) {
            }

            // Execute the SBPDIS command to disable BOOTPROT
            NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMD_SBPDIS | NVMCTRL_CTRLB_CMDEX_KEY;

            // Wait for the operation to complete
            while (NVMCTRL->STATUS.bit.READY == 0) {
            }

            if (flash_unlock(&FLASH_0, 0, FLASH_REGION_PAGE_NUM) != FLASH_REGION_PAGE_NUM) {
                screen_print_debug("first flash_unlock failed", 1500);
            }
            if (flash_unlock(&FLASH_0, 32 * 1024, FLASH_REGION_PAGE_NUM) != FLASH_REGION_PAGE_NUM) {
                screen_print_debug("second flash_unlock failed", 1500);
            }
            if (flash_write(&FLASH_0, ptr, &_new_root_pubkeys[0][0], 64 * 3) != ERR_NONE) {
                screen_print_debug("flash_write: rotation failed", 5000);
            }

            new_rootkeys_found = !memcmp((void*)ptr, _new_root_pubkeys, 64 * 3);

            break;
        }
    }
    if (!new_rootkeys_found) {
        screen_print_debug("rotation failed - not found", 5000);
        reboot();
        return;
    }

    _disable_mpu();
    boot_data_t data;
    memcpy(data.bytes, (uint8_t*)(FLASH_BOOTDATA_START), FLASH_BOOTDATA_LEN);
    data.fields.firmware_version = _new_fw_version;

    if (!_write_chunk(FLASH_BOOTDATA_START, data.bytes)) {
        screen_print_debug("could not write fw version", 2000);
    }

    reboot();
}
