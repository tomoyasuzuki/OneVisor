#include "util.h"
#include <stdint.h>

void convert_u64_to_char(char *str, uint64_t desc) {
    int length = 0, length_buff;
    int buff[10];
    while(1) {
        buff[length++] = desc % 16;
        if (desc < 16) break;
        desc /=16;
    }

    length_buff = length;

    while(length) {
        length--;
        *(str++) = (buff[length] < 10) ? (buff[length] + 0x30) : (buff[length] - 9 + 0x60);
    }

    *str = 0x00;
}

