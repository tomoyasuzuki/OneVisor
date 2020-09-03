#define SERIAL_PORT_ADDR 0x3f8
#include <stdint.h>

void init_serial();

uint8_t is_serial_received();

uint8_t read_serial();

void write_serial(const char value);

uint8_t is_transmit_empty();

void send_serials(const char *value);