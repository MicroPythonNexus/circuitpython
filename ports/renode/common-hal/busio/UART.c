/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 microDev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "shared-bindings/busio/UART.h"

#define RXTX ((volatile uint32_t *)0x40000004)
#define NO_CHAR (0xffffffff)

void common_hal_busio_uart_construct(busio_uart_obj_t *self,
    const mcu_pin_obj_t *tx, const mcu_pin_obj_t *rx,
    const mcu_pin_obj_t *rts, const mcu_pin_obj_t *cts,
    const mcu_pin_obj_t *rs485_dir, bool rs485_invert,
    uint32_t baudrate, uint8_t bits, busio_uart_parity_t parity, uint8_t stop,
    mp_float_t timeout, uint16_t receiver_buffer_size, byte *receiver_buffer,
    bool sigint_enabled) {
    self->pending_char = NO_CHAR;
}

bool common_hal_busio_uart_deinited(busio_uart_obj_t *self) {
    return true;
}

void common_hal_busio_uart_deinit(busio_uart_obj_t *self) {
    if (common_hal_busio_uart_deinited(self)) {
        return;
    }
}

// Write characters.
size_t common_hal_busio_uart_write(busio_uart_obj_t *self, const uint8_t *data, size_t len, int *errcode) {
    for (size_t i = 0; i < len; i++) {
        *RXTX = data[i];
    }
    return len;
}

// Read characters.
size_t common_hal_busio_uart_read(busio_uart_obj_t *self, uint8_t *data, size_t len, int *errcode) {
    size_t read = 0;
    while (self->pending_char != NO_CHAR && read < len) {
        data[read] = self->pending_char;
        read++;
        self->pending_char = *RXTX;
    }
    return read;
}

uint32_t common_hal_busio_uart_get_baudrate(busio_uart_obj_t *self) {
    return 0;
}

void common_hal_busio_uart_set_baudrate(busio_uart_obj_t *self, uint32_t baudrate) {
    mp_raise_NotImplementedError(NULL);
}

mp_float_t common_hal_busio_uart_get_timeout(busio_uart_obj_t *self) {
    return 0;
}

void common_hal_busio_uart_set_timeout(busio_uart_obj_t *self, mp_float_t timeout) {
}

uint32_t common_hal_busio_uart_rx_characters_available(busio_uart_obj_t *self) {
    if (self->pending_char != NO_CHAR) {
        return 1;
    }
    self->pending_char = *RXTX;
    if (self->pending_char != NO_CHAR) {
        return 1;
    }
    return 0;
}

void common_hal_busio_uart_clear_rx_buffer(busio_uart_obj_t *self) {
    self->pending_char = NO_CHAR;
}

bool common_hal_busio_uart_ready_to_tx(busio_uart_obj_t *self) {
    return true;
}

void common_hal_busio_uart_never_reset(busio_uart_obj_t *self) {
}
