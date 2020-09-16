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
#s%d,%d,%d,%d,t%.2f,d%.2f,%.2f
```

A sample output would then look like:
```
#s654,782,654,357,t45.23,d23.17,31.62
```

A simple parser would look like this then:
```python
  output = serial.read()
  results = []
  next_result = []
  for char in output:
    if char != '#':
      next_result.append(char)
    elif char == '#':
      if len(next_result) > 1 and next_result[0] == '#':
        results.push(next_result)
      next_result = []
```

