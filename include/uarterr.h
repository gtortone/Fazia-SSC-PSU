#ifndef _UARTERROR_H
#define _UARTERROR_H

typedef struct {
    UINT16         uart_oerr;  // UART overrun error
    UINT16         uart_ferr;  // UART frame error
} UartErrLog;

#endif

//EOF
