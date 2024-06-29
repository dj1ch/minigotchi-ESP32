# Install guide

Here you can find out how to install Minigotchi on the ESP32. Installing on the Pico is not possible as the code is meant for the ESP32.

**For building on an ESP32, see [this](https://github.com/Pwnagotchi-Unofficial/minigotchi/blob/main/INSTALL.md). Don't use this repository for the ESP866.**

## Building using Arduino IDE

### Step 1: Configuration

- Download the latest release [here](https://github.com/Pwnagotchi-Unofficial/minigotchi-ESP32/releases).

- Unzip the file

- Navigate to the `config.cpp` file in the unzipped folder, and open it up with the text editor of your choice. Configurations are always controlled here.

**If any of these lines are not in your config, such as screens, do not add them. That information is for testers on the development branch. Only modify the config values you have. Some of the data types for these values have changed, keep them as is in your config.**

- At the set of lines that says:

```cpp
// set to true normally
bool Config::deauth = true;
bool Config::advertise = true;
```

From here, you can turn off deauthing or advertising. Since this is the main feature of the minigotchi, I highly recommend you don't turn either of these off.

- After that, there should be a line that states the baud rate.

```cpp
// baud rate
int Config::baud = 115200;
```

Here, you can adjust the baud rate, which is of course optional. Make sure your serial terminal is running at the same baud rate as the Minigotchi.

- Here, we can adjust the channel we start on.

```cpp
// define init channel
int Config::channel = 1;
```

Replace the `1` with the channel you prefer(not in quotations).

- After this, we can configure our screen (Not in any version `<= 3.0.1`)

```cpp
// screen configuration
bool Config::display = false;
std::string Config::screen = "";
```

There are multiple different screen types available:

- `SSD1306`
  
- `SSD1305`
  
- `IDEASPARK_SSD1306`

- `WEMOS_OLED_SHIELD`

- `CYD`

- `T_DISPLAY_S3`
  
- `M5StickCP`
  
- `M5StickCP2`
  
- `M5Cardputer`

Set `bool Config::display = false;` to true, and `std::string Config::screen = "<YOUR_SCREEN_TYPE>";` to one of those screen types if your screen is supported.

**Keep in mind when you do enable a screen you are at a higher risk of your Minigotchi crashing...**

- There should also be a line that says:

```cpp
// define whitelist
std::string Config::whitelist[] = {"SSID", "SSID", "SSID"};
```

This defines our whitelist. The Minigotchi will not deauth these access points/any networks corresponding to that access point name. We can have up to ten values here. Three were added as an example, you can remove those.

- Here, we set the channels we hop from time to time.

```cpp
// define channels
int Config::channels[13] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
```

Usually, this shouldn't be changed as these are the only channels we can access on the ESP32. Some ESP32s may be able to access 5ghz channels but not all of them. These are only 2.4 ghz channels.

- Save and exit the file when you have configured everything to your liking. Note you cannot change this after it is flashed onto the board.

### Step 2: Building and flashing

- Download the [Arduino IDE](https://https://www.arduino.cc/en/software), preferably the latest version.

- In the IDE, go to `File` > `Preferences`, then in the popup by clicking the box icon next to `Additional board manager URLS`, paste the following:

```
https://dl.espressif.com/dl/package_esp32_index.json
https://espressif.github.io/arduino-esp32/package_esp32_index.json
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
```

- Now go to `Tools` > `Board` > `Boards Manager` and search `esp32` and click `Install` on `esp32`, using the version `2.0.10`.

- Click `Ok` and plug the board into your computer. It should blink, and make sure it is receiving the correct amount of voltage, too much will burn it(I learned that the hard way)

- Open up the Minigotchi folder through the IDE by pushing `Ctrl+O`, or by going to `File` > `Open`, then selecting the folder the .ino is in. If other tabs don't show up, along with `minigotchi.ino`, make sure to copy and paste the files into the same directory/folder as that lone `.ino` file. It won't be able to compile if that happens.

- Install the following dependencies with the library manager: `ArduinoJson`, `Adafruit GFX`, and your screen library(see below), etc with all their dependencies (Please install all of them for them to work correctly).

| `SSD1306`                                                                  | `WEMOS_OLED_SHIELD`                                                        | `CYD`                                                                                                                                                                 | `T_DISPLAY_S3`                                                                                                                                                                                                    |
| -------------------------------------------------------------------------- | -------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `Adafruit SSD1306`, remove `Adafruit SSD1306 Wemos Mini OLED` if installed | `Adafruit SSD1306 Wemos Mini OLED`, remove `Adafruit SSD1306` if installed | Follow the [CYD documentation](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/blob/main/SETUP.md), it uses a similar library much like the `T_DISPLAY_S3` | Follow the [T-Display-S3 documentation](https://github.com/Xinyuan-LilyGO/T-Display-S3/tree/main?tab=readme-ov-file#4%EF%B8%8F%E2%83%A3--arduino-ide-manual-installation), it uses a library similar to the `CYD` |

Make sure you install the correct library, they aren't the same library and if you install the wrong one it will result in the compilation failing.

- Go to `Tools` > `Board` and use one of the boards in the `esp32` section. (Example: `Adafruit Feather ESP32-S3 TFT`)

- Open the following with your text editor `C:\Users\<YOUR USERNAME>\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.10\platform.txt` and add the following:

  1. For `build.extra_flags.esp32`, `build.extra_flags.esp32s2`, `build.extra_flags.esp32s3`, `build.extra_flags.esp32c3`, add `-w` to their compile settings

  2. For `compiler.c.elf.libs.esp32`, `compiler.c.elf.libs.esp32s2`, `compiler.c.elf.libs.esp32s3`, `compiler.c.elf.libs.esp32c3`, add `-zmuldefs` to their compile settings

  3. For `compiler.c.elf.libs.esp32`, `compiler.c.elf.libs.esp32s2`, `compiler.c.elf.libs.esp32s3`, `compiler.c.elf.libs.esp32c3`, add `-zmuldefs` to their compile settings

  4. More may be added [here](https://github.com/justcallmekoko/ESP32Marauder/wiki/arduino-ide-setup#if-you-are-following-these-instructions-you-do-not-need-to-do-this)

- Select your COM port/Serial port through `Tools` > `Port` where the ESP32 is plugged in

- Click on the upload button(arrow pointing to the left).

**OR**

- Go to `Sketch` > `Export Compiled Binary` to generate a `.bin` file to flash to the board, which also works. This can also be done with `Alt+Ctrl+S`.

**Note: if you get any errors, let me know ASAP with a GitHub issue, sending me a discord message, or perhaps even messaging me through my portfolio site.**

### Step 3: Post Install

- You can click the _Serial Monitor_ button on the top bar to see the serial monitor output. Make sure the baud rate is `115200`. You can also use https://serial.huhn.me/ as a serial monitor, I also recommend using this!

- I also highly recommend doing the following:

1. Disabling AI on your Pwnagotchi so you can change your settings without AI changing them.

2. Setting `personality.channels[]` in your Pwnagotchi's `/etc/pwnagotchi/config.toml` to match your `Config::channels[13]` so that your Minigotchi has a higher chance of finding your Pwnagotchi.

- Happy hacking!
