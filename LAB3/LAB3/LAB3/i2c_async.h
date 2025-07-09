/*
 * i2c_async.h
 *
 * Created: 9/07/2025 11:12:01 a. m.
 *  Author: rasor
 */ 


#ifndef I2C_ASYNC_H_
#define I2C_ASYNC_H_

#include <stdint.h>
#include <stdbool.h>

/** Resultado de la operación */
typedef enum {
    I2C_OK,
    I2C_BUSY,
    I2C_ERROR_BUS,
    I2C_ERROR_NACK
} I2C_Result_t;

/** Callback que el usuario implementa para recibir evento */
typedef void (*i2c_callback_t)(I2C_Result_t result,
                               uint8_t *data,
                               uint8_t len);

/** Inicializa TWI a 100 kHz, prescaler=1, habilita interrupciones */
void i2c_async_init(void);

/** Inicia envío de `len` bytes desde `tx_buf` a `addr`.  
    Devuelve I2C_BUSY si está ocupada. Lanza operación y retorna. */
I2C_Result_t i2c_async_write(uint8_t addr,
                             uint8_t *tx_buf, uint8_t len,
                             i2c_callback_t cb);

/** Inicia recepción de `len` bytes desde `addr` a `rx_buf`. */
I2C_Result_t i2c_async_read(uint8_t addr,
                            uint8_t *rx_buf, uint8_t len,
                            i2c_callback_t cb);

#endif  // I2C_ASYNC_H_
