**wm_tool** is a command line tool that provides firmware packaging, firmware download, and log capture for the w600 chip. 

wm_tool runs on Windows, Linux and Mac OS X.

Usage
-----
```
wm_tool <arguments>
```

The program interprets the arguments given on the command line.
The following commands are currently available:

Argument | Description
---------|-------------
```-h``` | show usage.
```-v``` | show version.
```-b <binary>``` | original binary file.
```-o <output>``` | output firmware file. the default is the same as the original binary file name.
```-sb <secboot> ```| second boot file, used to generate fls file.
```-fc <uncompress | compress>``` | whether the firmware is compressed, default is compressed.
```-it <1M | 2M>``` | firmware image layout type, default is 1M layout.
```-ua <address>``` | upgrade storage location (hexadecimal).
```-ra <address>``` | runtime position (hexadecimal).
```-df``` | generate debug firmware for openocd.
```-l``` | list the local serial port.
```-c <serial>``` | connect a serial port.
```-ws <baudrate>``` | set the serial port speed during normal work, default is 115200.
```-ds <baudrate>``` | set the serial port speed when downloading, default is 115200.
```-rs <none | at | rts>``` | set device reset method, default is manual control.
```-eo <secboot | all>``` | firmware area erase option.
```-dl <firmware>``` | firmware file to be downloaded, default download compressed image.
```-sl <str | hex>``` | display the log information output from the serial port.

Examples
--------

##### Generate 1M Flash firmware
```
wm_tool -b wm_w600.bin
```
or:
```
wm_tool -b wm_w600.bin -sb secboot.img
```
or:
```
wm_tool -b wm_w600.bin -sb secboot.img -df -o wm_w600_1M
```

##### Generate 2M Flash firmware
```
wm_tool -b "../Bin/wm_w600.bin" -sb "../Bin/secboot.img" -it 2M -ua 100000 -ra 10100 -df -o "../Bin/wm_w600"
```

##### Use the AT command reset device to download firmware
```
wm_tool -c COM7 -ds 2M -dl wm_w600_gz.img -ws 115200 -rs at
```
or:
```
wm_tool -c COM7 -ds 2M -dl wm_w600.fls -eo secboot -ws 115200 -rs at
```

##### Use the RTS pin instruction to reset the device to download firmware
```
wm_tool -c COM7 -ds 2M -dl wm_w600_gz.img -rs rts
```
or:
```
wm_tool -c COM7 -ds 2M -dl wm_w600.fls -eo secboot -rs rts
```

##### Capture serial port log
```
wm_tool -c COM7 -ws 115200 -sl str
```
or:
```
wm_tool -c COM7 -ws 115200 -sl hex
```

##### Generate firmware, download firmware and capture log
```
wm_tool -b ../Bin/wm_w600.bin -c COM7 -dl ../Bin/wm_w600_gz.img -rs at -sl str
```


Build
-----
```
gcc ./wm_tool.c -lpthread -Wall -O2 -o ./wm_tool
```

Windows system needs to be compiled in MSYS environment.
Linux/Mac system needs to install the gcc compiler.