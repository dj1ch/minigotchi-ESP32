# Install guide

Here you can find out how to install minigotchi on the ESP8266. Installing on the Pico is not possible as the code is meant for the ESP8266. 

**For building on an ESP32, see [this](https://github.com/Pwnagotchi-Unofficial/minigotchi-ESP32/blob/development/INSTALL.md). Don't use this repository for the ESP32.**

## Building using Arduino IDE

### Step 1: Configuration
- Download the latest release [here](https://github.com/Pwnagotchi-Unofficial/minigotchi/releases).

- Unzip the file

- Navigate to the `config.cpp` file in the unzipped folder, and open it up with the text editor of your choice. Configurations are always controlled here. 

- At the set of lines that says:

```cpp
// set to true normally 
bool Config::deauth = true;
bool Config::advertise = true;
```

From here, you can turn off deauthing or advertising. Since this is the main feature of the minigotchi, I highly recommend you don't turn either of these off. 

- After that, there should be a line that states the buad rate. 

```cpp
// baud rate
int Config::baud = 115200;
```

Here, you can adjust the baud rate, which is of course optional. Make sure your serial terminal is running at the same buad rate as the minigotchi.

- Here, we can adjust the BSSID we listen on, and the channel we start on.

```cpp
// define init bssid, channel
const char* Config::bssid = "fo:od:ba:be:fo:od";
int Config::channel = 1;
```

Replace the `"fo:od:ba:be:fo:od"` with your actual BSSID(in the quotations), and the `1` with the channel you prefer(not in quotations). Note that the WiFi network you're listening on should be on a specific WiFi channel anyway, each AP is on a specific one. The BSSID in question should be one of your own, assuming the pwnagotchi is in your home it should be able to associate to your home WiFi network.

- There should also be a line that says:

```cpp
// define whitelist 
std::string Config::whitelist[] = {"fo:od:ba:be:fo:od", "fo:od:ba:be:fo:od", "fo:od:ba:be:fo:od"};
```

This defines our whitelist. The minigotchi will not deauth these access points. We can have up to ten values here. Three were added as an example, you can remove those. 

- Here, we set the channels we hop from time to time.

```cpp
// define channels
int Config::channels[3] = {1, 6, 11};
```

Usually this shouldn't be changed as these are the best channels for IOT boards such as an ESP8266.

- Save and exit the file when you have configured everything to your liking. Note you cannot change this after this is flashed onto the board. 

### Step 2: Building and flashing

- Download the [Arduino IDE](https://https://www.arduino.cc/en/software), preferably the latest version.

- In the IDE, go to `File` > `Preferences`, then in the `Additional board manager URLS`, paste the link `https://arduino.esp8266.com/stable/package_esp8266com_index.json` to add the ESP8266 board(s) to the board manager

- Click `Ok` and plug the board into your computer. It should blink, make sure it is receiving the correct amount of voltage, too much will burn it(I learned that the hard way)

- Open up the minigotchi folder through the IDE by pushing `Ctrl+O`, or by going to `File` > `Open`, then selecting the folder the .ino is in. If other tabs don't show up, along with `minigotchi.ino`, make sure to copy and paste the files into the same directory/folder as that lone `.ino` file. It won't be able to compile if that happens. 

- Install the following dependancies with the library manager: `ArduinoJson`, etc with all with their dependancies(Please install all of them for them to work correctly).

- Select the board as `ESP8266 WEMOS(LOLIN) D1 mini Lite`, and select the port it is plugged into(if you haven't already, plug in the board)
- Click on the upload button(arrow pointing to the left). 

**OR**

- Go to `Sketch` > `Export Compiled Binary` to generate a `.bin` file to flash to the board, which also works. This can also be done with `Alt+Ctrl+S`.

**Note: if you get any errors, let me know ASAP with a github issue, sending me a discord message, or perhaps even messaging me through my portfolio site.**

### Step 3: Post Install

- You can click on the *Serial Monitor* button on the top bar to see the serial monitor output. Make sure the baud rate is `115200`. You can also use https://serial.huhn.me/ as a serial monitor, I also recommend using this! 

- Happy hacking!

###
