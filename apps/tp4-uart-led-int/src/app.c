#include "../../tp4-uart-led/inc/app.h"         // <= Su propia cabecera (opcional)

#include "sapi.h"

void toggleLed(void*);

int main(void) {
	boardConfig();

	uartConfig(UART_USB, 9600);

	uartInterrupt(UART_USB, TRUE);

	uartCallbackSet(UART_USB, UART_RECEIVE, toggleLed, NULL);

	while (1)
		;

	return 0;
}

void toggleLed(void *unused) {
	int leds[4] = { CIAA_BOARD_LED, LED1, LED2, LED3 };

	uint8_t receivedByte = uartRxRead(UART_USB);
	uartWriteByte(UART_USB, receivedByte);

	int index = (int) (receivedByte - '0');

	if (index > 0 && index <= 4) {
		gpioWrite(leds[index - 1], !gpioRead(leds[index - 1]));
	}

}
