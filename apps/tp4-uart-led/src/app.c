#include "../../tp4-uart-led/inc/app.h"         // <= Su propia cabecera (opcional)

#include "sapi.h"

int main(void) {
	boardConfig();

	uartConfig(UART_USB, 9600);

	uint8_t receivedByte;

	while (1) {

		if (uartReadByte(UART_USB, &receivedByte) == TRUE) {
			uartWriteByte(UART_USB, receivedByte);

			if (receivedByte == '1') {
				gpioWrite( CIAA_BOARD_LED, !gpioRead(CIAA_BOARD_LED));

			}

			if (receivedByte == '2') {
				gpioWrite(LED1, !gpioRead(LED1));

			}

			if (receivedByte == '3') {
				gpioWrite(LED2, !gpioRead(LED2));

			}

			if (receivedByte == '4') {
				gpioWrite(LED3, !gpioRead(LED3));
			}

		}
	}

	return 0;
}
