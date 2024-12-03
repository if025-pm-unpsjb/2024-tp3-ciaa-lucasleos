#include "sapi.h"

int main(void) {
	boardConfig();

	bool ledState[4] = { 0, 0, 0, 0 };
	bool lastButtonState[4] = { 1, 1, 1, 1 };
	int buttons[4] = { TEC1, TEC2, TEC3, TEC4 };
	int leds[4] = { CIAA_BOARD_LED, LED1, LED2, LED3 };

	while (1) {
		for (int i = 0; i < 4; i++) {
			bool currentButtonState = gpioRead(buttons[i]);

			if (lastButtonState[i] && !currentButtonState) {
				ledState[i] = !ledState[i];
				gpioWrite(leds[i], ledState[i]);
			}

			lastButtonState[i] = currentButtonState;
		}
		delay(10);
	}

	return 0;
}
