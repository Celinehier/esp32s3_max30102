# ESP32 OXIMETER
Source code of an esp32 + maxim max30102 oximeter and heart rate device.
This code is based on the great job of [Aromring](https://github.com/aromring/MAX30102_by_RF)


## Espressif IoT Development Framework version

These project was built using esp-idf version: v5.2.3

## Connections beetwen esp32 and maxim max30102
MAX30102 Vin   ->   3v3

MAX30102 GND   ->   GND

MAX30102 SDA   -> GPIO5

MAX30102 SCL   -> GPIO6


## Flashing the code
Afer connecting everything up plug you esp32 to the pc and then in the project directory terminal run these commands:

```
idf.py fullclean

idf.py build

idf.py flash
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
