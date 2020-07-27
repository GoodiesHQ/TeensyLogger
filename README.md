# TeensyLogger
An open source, capable USB hardware keylogger built on Arduino Teensy for a low-cost, low-profile offensive security device to have in your toolkit. Covertly capture keystrokes without arousing suspicion from either side.

While I developed and deploying using [VisualTeensy](https://github.com/luni64/VisualTeensy/wiki/Quick-Start-Guide), it should function just fine with Arduino + TeensyDuino addon.

## Software
**TeensyLogger (TL)** is a relatively simple implementation of a keylogger:


  1. Upon bootup, **TL** will locate the file *BOOT.TXT* containing a hexadecimal value known as the **Boot ID**. If the file does not exist, 0 will be used and written to the file, creating it. This ID is incremented by one each time **TL** boots up and *BOOT.TXT* is overwritten with the new value.
  2. An XOR key can be statically defined  
  3. When the logging starts, it will be started at part 0. To keep the log files relatively small, the log will be rotated every 1,000,000,000
  4. Each logfile will be named ***TL_{{BOOT ID}}_{{part #}}.TLOG***. E.G. the 10th computer's log files will look like:
     - TL_A_0.TLOG
     - TL_A_1.TLOG
     - TL_A_2.TLOG
     - etc
  5. The logfile can be retrieved and XORed with the static key that is built-into the program (default is **"\x00"** which provides NO ENCRYPTION).
  
### Chaning the XOR Key

The macro definition for `XOR_KEY` in `xor.c` can be changed:

    #define XOR_KEY "\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff"


## Hardware

### Teensy 4.1 + Header Pins ($30)
At 600 MHz, the Teensy 4.X family is nearly 5x faster than any previous generation. While the Teensy 4.0 is perfectly capable of SD communications, the Teensy 4.1 for under $30 comes with the SD card reader already soldered in place ready to use. The tradeoff, however, is a slightly larger form factor with the 4.1 coming it at 61mm (2.4") compared to the 4.0's 35mm (1.4") design. Other Teensy models (specifically, 3.5 and 3.6) would be capable of SD keylogging, but they are priced nearly the same as the 4.1 making it the obvious choice unless youh have an existing previous generation handy. One benefit of the Teensy 4.1 is the possibility to use 100mbit Ethernet and send keystrokes over the network, however combining any generation of Teensy with an ESP32 WiFi-capable microcontroller will allow for the same result wirelessly.

![Teensy 4.1](https://forum.pjrc.com/teensy41_4.jpg)

### USB Host Cable (I picked up 2 for $3)
For the female inlet for the host device to capture (i.e. a keyboard), you must connect a USB host cable to the pins labeled "USB Host" on the Teensy. I chose to solder on header pins with the elongated part pointing up, so I can connect a female header breakout to it, though a dedicated directly-soldered wire would result in a lower profile and allow for a smaller case.   

![USB Host Cable](https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcT3o6Z4MrHjN364qJrKoiqCPFzSRM1f6-dq_N5ZDMLaZ7da-4B8UgUExA7h_jWc3m4EN_1FsAvA&usqp=CAc)

### Micro USB Male to USB-A Male Adapter
The Keylogger will receive input via the female USB Host cable, and will send output over its MicroUSB interface. This connection can trivially be made with your run-of-the=mill MicroUSB charging cable, but for a low-profile form factor, I chose to use a short adapter. This will undoubtedly add some distance 

![MicroUSB to USB Adapter](https://cdn.shopify.com/s/files/1/0020/4433/0057/products/278b27db0115f4eddd8bcf3569dc11ab_300x300.jpg?v=1571712194)

### SD Card for storing logs
You can find a Class 10 MicroSD card literally anywhere for cheap. 32GB is a safe storage amount for multiple long-term deployments,

![Micro SD Card](https://pisces.bbystatic.com/image2/BestBuy_US/images/products/6346/6346822_sd.jpg;maxHeight=250;maxWidth=300)
