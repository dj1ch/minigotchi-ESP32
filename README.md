# minigotchi-ESP32 - ミニゴッチ

<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/dj1ch/minigotchi-ESP32"></img>
<img alt="GitHub License" src="https://img.shields.io/github/license/dj1ch/minigotchi-ESP32?label=license">
<a href="https://github.com/dj1ch/minigotchi-ESP32/graphs/contributors"><img alt="GitHub contributors" src="https://img.shields.io/github/contributors/dj1ch/minigotchi-ESP32"></a>
<img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/m/dj1ch/minigotchi-ESP32"></img>
<img alt="GitHub Repo stars" src="https://img.shields.io/github/stars/dj1ch/minigotchi-ESP32?label=stars">

## The [Minigotchi](https://github.com/dj1ch/minigotchi) ported to the ESP32

<img src="images/IMG_1213.jpeg"></img>

### Intro

Due to a surge in people asking for ESP32 support, I have created this fork. Fundamentally, the ESP8266 and ESP32 can perform similar functions, although the ESP32 has more features, memory, computing power, etc. Most, if not all ESP32s are supported. Feel free to ask me about supporting a device; however if it's not an ESP32, I won't be able to help you as this repository is made specifically for ESP32 devices. There are additional ports you may try, however this is the only one currently maintained by me. I've worked on this project since the start of high school during my free time; there's a lot of work done here to make the ESP32 closer to the original Minigotchi firmware, as well as the Pwnagotchi.

We made a [discord server](https://discord.gg/EhTAr7PwVP)! Feel free to join! (Though it's mostly dead, sorry)

### Install guide

See the [install guide](INSTALL.md).

### Languages

The code is written in these languages.

- _C_
- _C++_

Note that the _C++_ in Arduino is slightly modified from what I have heard. See [this Reddit question](https://www.reddit.com/r/arduino/comments/x46sml/is_arduino_programming_language_c/) for more about this. Arduino probably has tweaked it to be more beginner-friendly, although still being [low-level](https://en.wikipedia.org/wiki/Low-level_programming_language). The _Pwnagotchi_ is a complex machine written in _Python_ and _Go_, so code from the Pwnagotchi Project is **not** compatible with mine. Plugins, handshakes, and additional features cannot be loaded onto the Minigotchi without prior configuration onto the ESP32. Despite this fact, the Minigotchi is an easily hackable device that can be modified to your liking in a low-level language, allowing for finer control of your hardware.

### Prerequisites

- Arduino IDE
- A reasonable understanding of programming embedded devices
- Any ESP32\* based microcontroller (modules do not work, must be a microcontroller)
- A reliable and appropriate power supply for the hardware; batteries work too!
- Patience (a lot of it)
- The ability to read

### How it operates/works

See the [wiki](https://github.com/dj1ch/minigotchi/wiki/How-the-Minigotchi-works).

### Hardware

See the [wiki](https://github.com/dj1ch/minigotchi/wiki/Hardware).

### To do

See this [project](https://github.com/orgs/dj1ch/projects/4).

### Contributing

See the Contributing Guidelines [here](CONTRIBUTING.md).

### FAQ

See the [wiki](https://github.com/dj1ch/minigotchi/wiki/FAQ).

### Communication

See the [wiki](https://github.com/dj1ch/minigotchi/wiki/Communication#communication).

### Last but not least... Some thank yous

**If you believe your project wasn't properly credited, please let me know ASAP! Here's how you can [communicate](https://github.com/dj1ch/minigotchi/wiki/Communication#communication) with me.**

**Thanks to these projects for making this a little bit more possible for me. I have used libraries/code from these projects and implemented it here:**

- [Pwngrid](https://github.com/evilsocket/pwngrid), this is what the project is trying to replicate, specifically the Pwnagotchi's system of peer detection. If this didn't exist, this project probably wouldn't have either.

- [ESP32 Marauder](https://github.com/justcallmekoko/ESP32Marauder), similar to the ESP8266 Deauther, although packed with more features and support for Flipper Zero's ESP32 WiFi Card Module. It has wardriving capabilities, BLE, Pwnagotchi Scanning, and Deauthing available. This is a very cool project. I highly recommend checking this out if you have an ESP32.

- [Milkydeveloper](https://github.com/MilkyDeveloper), ~~idk what he did, but he wanted to be here ig so...(this is totally not an advertisement)~~ he has recently deleted all of his accounts. Last time I heard from him he won some stuff at ISEF and is doing great. He played a major inspiration to me in the building of my own personal projects and such. Wishing you the best if you're reading this man. I won't be seeing you at USC though, however, you'll know where to find me. Good luck in college dawg. :)

- [esp-idf](https://github.com/espressif/esp-idf), the Espressif development framework for IoT devices like the ESP32

- [arduino-ESP32](https://github.com/espressif/arduino-esp32), the core Arduino libraries for the ESP32 compatibility in the Arduino IDE

- I'd also like to thank [PiSugar](https://www.pisugar.com/) for the partnership with Pwnagotchi-Unofficial! Your batteries are amazing and have contributed heavily in the creation of this project.

**Made with ❤️ by [@dj1ch](https://github.com/dj1ch) and the rest of the Pwnagotchi-Unofficial team!**
