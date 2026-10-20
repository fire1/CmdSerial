# Arduino CmdSerial Library

The CmdSerial library provides a simple way to handle commands and data received via the Serial port on Arduino devices.
It allows you to define commands, set values, and receive input from the Serial monitor in a structured manner.

## Installation

To use the CmdSerial library, follow these steps:

1. Download the library source code from [GitHub repository](https://github.com/example/cmd-serial).
2. Extract the downloaded ZIP file.
3. Copy the extracted folder into the `libraries` folder of your Arduino sketchbook directory.
4. Rename the folder to `CmdSerial`.
5. Restart the Arduino IDE if it's open.

## Usage

### Initialization /simple/

To use the CmdSerial library in your Arduino sketch, include the `CmdSerial.h` header file at the beginning of your
sketch:

```cpp
#include <CmdSerial.h>
```

Then, create an instance of the CmdSerial class:

```cpp
CmdSerial cmd;
```

### Listening for Commands

To listen for commands from the Serial monitor, call the listen() method in the loop() function of your sketch:

```cpp
void loop() {
    cmd.listen();
    // Your code here
} 
```

### Define command

You can define commands using the show() method. For example, to respond to the command `data show`, you would write:

```cpp
int testInt = 42;
if (cmd.show("data"_f)) { // By default every 500ms while monitoring is on
    cmd.print("Output message"_f, testInt);  // "..."_f == F("...") — stays in PROGMEM
}

uint8_t banana = 100;
if (cmd.show("banana"_f, "Some information for help."_f, 1000)) { // 1000ms interval
    cmd.print("Banana output"_f, banana);
}

// Several values on one line
float temp = 21.5;
if (cmd.show("sensors"_f)) {
    cmd.println("temp"_f, temp, "banana"_f, banana);
}
```

`F("...")` still works. `"..."_f` is the same idea with less noise.

### Setting Values

You can set values in your sketch based on commands received via Serial. Use the set() method to define how values
should be set. For example, to set a value named "temperature", you would write:

```cpp
float temperature;
if (cmd.set(F("temperature"), temperature)) {
    // Value has been set, do something extra with it...
}
```
### The Help function
The `CmdSerial` library provides a built-in mechanism to retrieve information about available commands and their usage.

Here's how to access it:

**List all commands by typing `help` in the Serial Monitor:** \
This will trigger the library to print a list of all defined (set,show) commands to the Serial Monitor.

**Individual Command Help:**\
For more detailed information about a specific command,type the command name followed by "help" in the Serial Monitor.\
For example, if you have a command named `data`, you would type `data help`.


### Serial monitor

To send data from the Serial monitor to your sketch, use the defined command name in the set() method followed by your
value.\
The library also provides two keywords: show to start monitoring and stop to stop monitoring for the defined command
name. \
If you need to customize these keywords, please refer to the source code of the library.

## Online example / Playground

You can check the online simulation example of the library here: \
https://wokwi.com/projects/392856762315911169

Type `help` in the terminal in order to get a list of avelabe commands.

* `test 12` will set `testInt` to 12.
* `test show` starts monitoring of the value in the sketch.
* `test stop` stops the monitoring.
* `sensors show` prints several fields on one line.

## Documentation

All methods can be found in [api.md](docs/api.md).

## License

This library is released under the MIT License. See the LICENSE file for details.
