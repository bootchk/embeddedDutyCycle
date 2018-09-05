
/*
 * Knows app's use of LED.
 */

class LED {
public:
	static void configure();
	static void turnOn();
	static void turnOff();
	static void toggle();
	/*
	 * Busy wait blink.
	 */
	static void blink();
};
