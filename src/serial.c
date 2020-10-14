#include "serial.h"
#include "assembly.h"
#include "util.h"

void init_serial() {
    io_out8(SERIAL_PORT_ADDR + 1, 0x00);   
    io_out8(SERIAL_PORT_ADDR + 3, 0x80);    
    io_out8(SERIAL_PORT_ADDR + 0, 0x01);    
    io_out8(SERIAL_PORT_ADDR + 1, 0x00);    
    io_out8(SERIAL_PORT_ADDR + 3, 0x03);    
    io_out8(SERIAL_PORT_ADDR + 2, 0xC7);    
    io_out8(SERIAL_PORT_ADDR + 4, 0x0B);
}

uint8_t is_serial_received() {
    return io_in8(SERIAL_PORT_ADDR + 5) & 1;
}

uint8_t read_serial() {
    while(!is_serial_received());

    return io_in8(SERIAL_PORT_ADDR);
}

uint8_t is_transmit_empty() {
    return io_in8(SERIAL_PORT_ADDR + 5) & 0x20;
}

void write_serial(const char value) {
    while(!is_transmit_empty());

    io_out8(SERIAL_PORT_ADDR, value);
}

void send_serials(const char *value) {
    int i = 0;
    while(value[i] != '\0') {
        write_serial(value[i]);
        i++;
    }
}

void log_u64(uint64_t value) {
    send_addr(value);
    send_serials("\n\r");
}

void log_char(char *value) {
    send_serials(value);
    send_serials("\n\r");
}

void send_u64(uint64_t value) {
    char str[1024];
    convert_u64_to_char(str, value);
    send_serials(str);
}

void send_addr(uint64_t value) {
    char *str = "0x";
    send_serials(str);
    send_u64(value);
}