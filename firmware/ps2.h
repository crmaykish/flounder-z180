#ifndef PS2_H
#define PS2_H

#include <stdbool.h>

#define PS2_BREAK_BYTE 0xF0

/**
 * @brief Check if there's a PS/2 scan code ready to be read
 *
 * @return true Scan code available
 * @return false No scan code available
 */
bool ps2_char_available();

/**
 * @brief Block until a PS/2 scancode is available
 *
 * @return char The scancode read from the PS/2 controller
 */
char ps2_get_char();

#endif
