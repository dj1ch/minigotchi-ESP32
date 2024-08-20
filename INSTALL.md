# Install guide

Here you can find out how to install Minigotchi on the ESP32. Installing on the Pico is not possible as the code is meant for the ESP32.

**For building on an ESP8266, see [this](https://github.com/Pwnagotchi-Unofficial/minigotchi/blob/main/INSTALL.md). Don't use this repository for the ESP866.**

## Building using Arduino IDE

### Step 1: Configuration

- Download the latest release [here](https://github.com/Pwnagotchi-Unofficial/minigotchi-ESP32/releases).

- Unzip the file

- Navigate to the `config.cpp` file in the unzipped folder, and open it up with the text editor of your choice. Configurations are always controlled here.

**If any of these lines are not in your config, such as screens, do not add them. That information is for testers on the development branch. Only modify the config values you have. Some of the data types for these values have changed, keep them as is in your config.**

- At the set of lines that says:

```cpp
// define if features will be used
bool Config::deauth = true;
bool Config::advertise = true;
bool Config::scan = true;
```

From here, you can turn off deauthing, advertising, or Pwnagotchi scanning. Since these are the main features of the minigotchi, I highly recommend you don't turn any of these off.

- With `Config::shortDelay` and `Config::longDelay`, we can adjust the speed of our Minigotchi.

```cpp
// define universal delays
int Config::shortDelay = 500;
int Config::longDelay = 5000;
```

A `Config::shortDelay` is a shorter delay used in between doing tasks, though sometimes too many of them can be problematic. A `Config::longDelay` is a longer delay which isn't used as much but it often used before and/or after something major, to allow the user to read serial/screen output. I put down the recommended default values but you can also change them to whatever you like.

- We can also enable parasite mode, which requires the [minigotchi plugin](https://github.com/matrix224/pwnagotchi_plugins/tree/main). This will allow our Minigotchi to communicate with a Pwnagotchi over serial connection to the Pwnagotchi.

```cpp
bool Config::parasite = false;
```

It's false by default, but you can enable it by making it `true`.

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

- `WEMOS_OLED_SHIELD`

- `CYD`

- `T_DISPLAY_S3`

- `M5STICKCP`

- `M5STICKCP2`

- `M5CARDPUTER`

- `SSD1305`

- `IDEASPARK_SSD1306`

- `SH1106`

Set `bool Config::display = false;` to true, and `std::string Config::screen = "<YOUR_SCREEN_TYPE>";` to one of those screen types if your screen is supported.

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

```sh
https://dl.espressif.com/dl/package_esp32_index.json
https://espressif.github.io/arduino-esp32/package_esp32_index.json
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
```

- Now go to `Tools` > `Board` > `Boards Manager` and search `esp32` and click `Install` on `esp32`, using the version `2.0.10`.

- Click `Ok` and plug the board into your computer. It should blink, and make sure it is receiving the correct amount of voltage, too much will burn it(I learned that the hard way)

- Open up the Minigotchi folder through the IDE by pushing `Ctrl+O`, or by going to `File` > `Open`, then selecting the folder the .ino is in. If other tabs don't show up, along with `minigotchi-ESP32.ino`, make sure to copy and paste the files into the same directory/folder as that lone `.ino` file. It won't be able to compile if that happens.

- Install the following dependencies (with their dependencies as well) with the library manager:
  - `ArduinoJson`
  - `Adafruit GFX`
  - `Adafruit SSD1306`
  - `TFT_eSPI`
  - `Adafruit SSD1305`
  - `u8g2`
  - `AsyncTCP`
  - Remove/uninstall/modify your screen library depending on your display for `Config::screen` below, some of these dependencies may have already been installed previously.
  - Sometimes the repository owner of certain libraries may require you to install it a certain way, be sure to follow their guides if needed/included.

| `SSD1306`                                                                  | `WEMOS_OLED_SHIELD`                                                        | `CYD`                                                                                                                                                                 | `T_DISPLAY_S3`                                                                                                                                                                                                                                                                                               | Any `M5`\* board                                 | `SSD1305`                  | `IDEASPARK_SSD1306`        | `SH1106`                   |
| -------------------------------------------------------------------------- | -------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------ | -------------------------- | -------------------------- | -------------------------- |
| `Adafruit SSD1306`, remove `Adafruit SSD1306 Wemos Mini OLED` if installed | `Adafruit SSD1306 Wemos Mini OLED`, remove `Adafruit SSD1306` if installed | Follow the [CYD documentation](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/blob/main/SETUP.md), it uses a similar library much like the `T_DISPLAY_S3` | Follow the [T-Display-S3 documentation](https://github.com/Xinyuan-LilyGO/T-Display-S3/tree/main?tab=readme-ov-file#4%EF%B8%8F%E2%83%A3--arduino-ide-manual-installation), it uses a library similar to the `CYD`. If it's an off-brand/clone of some sort, install `TFT_eSPI` and follow this [guide](/TFT) | Install `TFT_eSPI` and follow this [guide](/TFT) | Install `Adafruit SSD1305` | Install the `u8g2` library | Install the `u8g2` library |

Make sure you install the correct library, they aren't the same library and if you install the wrong one it will result in the compilation failing.

- You'll also need to install this fork of [`ESPAsyncWebServer`](https://github.com/bmorcelli/ESPAsyncWebServer) by downloading a ZIP of that repository through `<img> Code` > `Download ZIP`. If you already have `ESPAsyncWebServer` installed please **remove it** and follow this guide.

<img src="images/download (20).png"> </img>

- After doing that you'll need to go back to Arduino IDE then add the library by going to `Sketch` > `Include Library` > `Add .ZIP library...`, then selecting the `.ZIP` file of the repository that was just downloaded. This will add the library to Arduino IDE.

- Go to `Tools` > `Board` and use one of the boards in the `esp32` section. (Example: `Adafruit Feather ESP32-S3 TFT`)

#### Pre-requisite for ESP32 boards <=2.0.10

- Open the following with your text editor `C:\Users\<YOUR USERNAME>\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.10\platform.txt` and add the following:

  1. For `build.extra_flags.esp32`, `build.extra_flags.esp32s2`, `build.extra_flags.esp32s3`, `build.extra_flags.esp32c3`, add `-w` to their compile settings

  2. For `compiler.c.elf.libs.esp32`, `compiler.c.elf.libs.esp32s2`, `compiler.c.elf.libs.esp32s3`, `compiler.c.elf.libs.esp32c3`, add `-zmuldefs` to their compile settings

  3. More may be added [here](https://github.com/justcallmekoko/ESP32Marauder/wiki/arduino-ide-setup#if-you-are-following-these-instructions-you-do-not-need-to-do-this). See an example `platform.txt` [here](/misc/2.0.10/platform.txt).

#### Pre-requisite for ESP32 boards > 3.0.1

Please note that that is a WIP and may not work correctly. Users may get this error seen in [#80](https://github.com/dj1ch/minigotchi-ESP32/issues/80)

```sh
C:\Users\USERNAME\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32c6/include/esp_wifi/include/esp_private/wifi_os_adapter.h:84:29: error: operator '||' has no left operand
   84 | #if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
      |                             ^~
C:\Users\USERNAME\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32c6/include/esp_wifi/include/esp_private/wifi_os_adapter.h:116:29: error: operator '!' has no right operand
  116 | #if !CONFIG_IDF_TARGET_ESP32
```

- Look for the file `%localappdata%\Arduino15\packages\esp32\tools\esp32-arduino-libs\<idf-release_v5.x-version>\esp32\flags\ld_flags` (the file doesn't have extension)
- Add `-zmuldefs` at the beginning of the file
- do the same in the following files:

```powershell
"%localappdata%\Arduino15\packages\esp32\tools\esp32-arduino-libs\<idf-release_v5.x-version>\esp32\flags\ld_flags"
"%localappdata%\Arduino15\packages\esp32\tools\esp32-arduino-libs\<idf-release_v5.x-version>\esp32c3\flags\ld_flags"
"%localappdata%\Arduino15\packages\esp32\tools\esp32-arduino-libs\<idf-release_v5.x-version>\esp32c6\flags\ld_flags"
"%localappdata%\Arduino15\packages\esp32\tools\esp32-arduino-libs\<idf-release_v5.x-version>\esp32h2\flags\ld_flags"
"%localappdata%\Arduino15\packages\esp32\tools\esp32-arduino-libs\<idf-release_v5.x-version>\esp32s2\flags\ld_flags"
"%localappdata%\Arduino15\packages\esp32\tools\esp32-arduino-libs\<idf-release_v5.x-version>\esp32s3\flags\ld_flags"
```

#### Compile and upload

- Select your COM port/Serial port through `Tools` > `Port` where the ESP32 is plugged in

- Enable the erasing of your entire flash by going to `Tools` > `Erase All Flash Before Sketch Upload` then enabling that setting

- Click on the upload button(arrow pointing to the left). If you see any errors that you cannot solve, feel free to make an [issue](https://github.com/dj1ch/minigotchi/issues).

##### OR

- Go to `Sketch` > `Export Compiled Binary` to generate a `.bin` file to flash to the board, which also works. This can also be done with `Alt+Ctrl+S`.

**Note: if you get any errors, let me know ASAP with a GitHub issue, sending me a discord message, or perhaps even messaging me through my portfolio site.**

### Step 3: Post Install

- After plugging in your Minigotchi, it should tell you that it's starting a Web Server. In order to use it, you have to:

1. Connect to the access point `minigotchi` or `minigotchi 2` (sometimes `minigotchi 2` pops up)
2. Go to the website http://192.168.4.1
3. Add your whitelist in the first input box in a format like this. Click `Submit` after you're done

```sh
SSID,SSID,SSID
```

4. Make sure to turn off the WebUI going to the last text box and typing `true` so that your Minigotchi knows your configuration is done.
5. Exit the tab and disconnect from the `minigotchi` AP if it hasn't been done for you already.

- You can click the _Serial Monitor_ button on the top bar of Arduino IDE to see the serial monitor output. Make sure the baud rate is `115200`. You can also use [Spacehuhn's Serial Terminal](https://serial.huhn.me/) as a serial monitor, I also recommend using this!

- I also highly recommend doing the following:

1. Disabling AI on your Pwnagotchi so you can change your settings without AI changing them.

2. Setting `personality.channels[]` in your Pwnagotchi's `/etc/pwnagotchi/config.toml` to match your `Config::channels[13]` so that your Minigotchi has a higher chance of finding your Pwnagotchi.

- Happy hacking!
