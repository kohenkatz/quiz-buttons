# Quiz Buttons

This project implements wireless "quiz show" buttons.

The senders are ESP8266 devices.
We chose the Wemos D1 Mini Pro because it has built-in battery management.

The receiver should be an ESP32 because it can emulate a keyboard.

Communication uses ESP NOW, which is a 2.4GHz connectionless protocol intended for small data packets.

Each sender is identified on the receiver by its MAC address, which the receiver will map to a keyboard keypress.
This allows for easily enrolling new senders or changing keys without needing to reflash the senders.
The senders actually send a "magic" string, because this will prevent accidental triggers from other nearby ESP NOW devices.
This does NOT prevent intentional/malicious interference, but we have decided not to use encryption because it limits how many senders can be used.
