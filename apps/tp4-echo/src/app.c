#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"

#define BUFFER_SIZE 1000

int main(void) {
    boardConfig();

    uartConfig(UART_USB, 9600);

    uint8_t receivedByte;
    uint8_t buffer[BUFFER_SIZE];
    uint32_t index = 0;

    while (1) {

        if (uartReadByte(UART_USB, &receivedByte) == TRUE) {
            uartWriteByte(UART_USB, receivedByte);

            if (index < (BUFFER_SIZE - 1)) {
                buffer[index++] = receivedByte;
            }

            if (receivedByte == '\r') {
                buffer[index] = '\0';

                uartWriteString(UART_USB, "\r\n");
                uartWriteString(UART_USB, (char*) buffer);
                uartWriteString(UART_USB, "\r\n");

                index = 0;
            }
        }
    }

    return 0;
}
