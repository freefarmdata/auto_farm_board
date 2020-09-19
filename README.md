# auto_farm_board

This is the library containing the Arudino sketch
to communicate with various sensors, like the ds18b20,
capacitive soil mositure sensors, and the DHT11.

It works by printing out the readings to the serial console
and being read by auto_farm_source on a raspberry pi
via serial input.

## Serial Output

Serial output follows this simple format as interpreted
by sprintf:
```
#s[%d,%d,%d,%d] t[%.2f] h[%.2f,%.2f]#
```

