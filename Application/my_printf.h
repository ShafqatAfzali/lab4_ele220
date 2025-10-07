/**
 * @file   my_printf.h
 * @author Ken Henry Andersen
 */

#ifndef _MY_PRINTF_H_
#define _MY_PRINTF_H_

/**
 * @brief Function for sending formatted print output to the UART.
 *
 * This function takes a variable number of arguments and formats a string that is sent to UART.
 *
 * @param[in] fmt Pointer to a string that contains formatting options.
 * @param[in] ... Variable number of arguments to enter the formatted string.
 */
void MyPrintf(const char *fmt, ...);

/**
 * @brief PRINTF macro for simplicity.
 */
#if 1
#define PRINTF(...) MyPrintf(__VA_ARGS__)
#else
#define PRINTF(...) do {} while(0)
#endif

#endif /* _MY_PRINTF_H_ */
