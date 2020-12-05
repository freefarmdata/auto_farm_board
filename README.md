# auto_farm_board

This is the library containing the Arudino sketch
to communicate with various sensors, like the ds18b20,
capacitive soil mositure sensors, and the DHT11.

It works by printing out the readings to the serial console
and being read by auto_farm_source on a raspberry pi
via serial input.

## Serial Output

Serial output is in JSON format. Like so:
```json
{"temp":[1.23, 4.56],"pressure":[1.23,4.56]}
```

There may be other output for error reasons, but a basic parser in python looks like
```python
def read_sensors(self):
    output = serial.read(1000).decode("utf-8")
    output = list(map(lambda l: l.strip(), output.split('\n')))
    messages = []
    for line in output:
    try:
        messages.append(json.loads(line))
    except Exception as e:
        pass
    return messages
```
its okay if an exception is thrown. Most likely a normal thing that you can ignore.

