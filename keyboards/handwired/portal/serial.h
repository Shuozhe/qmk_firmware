#pragma once

#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define I2C_READ 1
#define I2C_WRITE 0

#define I2C_ACK 1
#define I2C_NACK 0

#define SLAVE_BUFFER_SIZE 0x10

// i2c SCL clock frequency 400kHz
#define SCL_CLOCK  400000L

extern volatile uint8_t i2c_slave_buffer[SLAVE_BUFFER_SIZE];

void i2c_master_init(void);
uint8_t i2c_master_start(uint8_t address);
void i2c_master_stop(void);
uint8_t i2c_master_write(uint8_t data);
uint8_t i2c_master_read(int);

#ifndef SOFT_SERIAL_H
#define SOFT_SERIAL_H

#include <stdbool.h>

// /////////////////////////////////////////////////////////////////
// Need Soft Serial defines in config.h
// /////////////////////////////////////////////////////////////////
// ex.
//  #define SOFT_SERIAL_PIN ??   // ?? = D0,D1,D2,D3,E6
//  OPTIONAL: #define SELECT_SOFT_SERIAL_SPEED ? // ? = 1,2,3,4,5
//                                               //  1: about 137kbps (default)
//                                               //  2: about 75kbps
//                                               //  3: about 39kbps
//                                               //  4: about 26kbps
//                                               //  5: about 20kbps
//
// //// USE Simple API (OLD API, compatible with let's split serial.c)
// ex.
//  #define SERIAL_SLAVE_BUFFER_LENGTH MATRIX_ROWS/2
//  #define SERIAL_MASTER_BUFFER_LENGTH 1
//
// //// USE flexible API (using multi-type transaction function)
//  #define SERIAL_USE_MULTI_TRANSACTION
//
// /////////////////////////////////////////////////////////////////


#ifndef SERIAL_USE_MULTI_TRANSACTION
/* --- USE Simple API (OLD API, compatible with let's split serial.c) */
#if SERIAL_SLAVE_BUFFER_LENGTH > 0
extern volatile uint8_t serial_slave_buffer[SERIAL_SLAVE_BUFFER_LENGTH];
#endif
#if SERIAL_MASTER_BUFFER_LENGTH > 0
extern volatile uint8_t serial_master_buffer[SERIAL_MASTER_BUFFER_LENGTH];
#endif

void serial_master_init(void);
void serial_slave_init(void);
int serial_update_buffers(void);

#endif // USE Simple API

// Soft Serial Transaction Descriptor
typedef struct _SSTD_t {
    uint8_t* status;
    uint8_t initiator2target_buffer_size;
    uint8_t* initiator2target_buffer;
    uint8_t target2initiator_buffer_size;
    uint8_t* target2initiator_buffer;
} SSTD_t;
#define TID_LIMIT( table ) (sizeof(table) / sizeof(SSTD_t))

// initiator is transaction start side
void soft_serial_initiator_init(SSTD_t* sstd_table, int sstd_table_size);
// target is interrupt accept side
void soft_serial_target_init(SSTD_t* sstd_table, int sstd_table_size);

// initiator resullt
#define TRANSACTION_END 0
#define TRANSACTION_NO_RESPONSE 0x1
#define TRANSACTION_DATA_ERROR  0x2
#define TRANSACTION_TYPE_ERROR  0x4
#ifndef SERIAL_USE_MULTI_TRANSACTION
int  soft_serial_transaction(void);
#else
int  soft_serial_transaction(int sstd_index);
#endif

// target status
// *SSTD_t.status has
//   initiator:
//       TRANSACTION_END
//    or TRANSACTION_NO_RESPONSE
//    or TRANSACTION_DATA_ERROR
//   target:
//       TRANSACTION_DATA_ERROR
//    or TRANSACTION_ACCEPTED
#define TRANSACTION_ACCEPTED 0x8
#ifdef SERIAL_USE_MULTI_TRANSACTION
int  soft_serial_get_and_clean_status(int sstd_index);
#endif

#endif /* SOFT_SERIAL_H */

void i2c_reset_state(void);
void i2c_slave_init(uint8_t address);


static inline unsigned char i2c_start_read(unsigned char addr)
{
    return i2c_master_start((addr << 1) | I2C_READ);
}

static inline unsigned char i2c_start_write(unsigned char addr)
{
    return i2c_master_start((addr << 1) | I2C_WRITE);
}

// from SSD1306 scrips
extern unsigned char i2c_rep_start(unsigned char addr);
extern void i2c_start_wait(unsigned char addr);
extern unsigned char i2c_readAck(void);
extern unsigned char i2c_readNak(void);
extern unsigned char i2c_read(unsigned char ack);

#define i2c_read(ack)  (ack) ? i2c_readAck() : i2c_readNak();
