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
int testInt=42;
if (cmd.show(F("data"))) { // By default 500ms will  display contents in if statement.
    cmd.print(F("Output message"),testInt); // This will be displayed in Serial monitor.
}

uint8_t banana= 100;
if(cmd.show(F("banana",F("Some information for help."), 1000)){ // Changing the display time to 1000ms
    cmd.print(F("Banana output"), banana);
}



```

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
* `test show` Starts monitoring of the actual value in the sketch.
* `test stop` Stops the monitoring.

## License

This library is released under the MIT License. See the LICENSE file for details.
