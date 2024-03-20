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

You can define commands using the show() method. For example, to respond to the command "show data", you would write:

```cpp
int testInt=42;
if (cmd.show(F("data"))) { // By default 500ms will  display contents in if statement.
    cmd.print(F("Output message"),testInt); // This will be displayed in Serial monitor.
}

uint8_t banana= 100;
if(cmd.show(F("banana"), 1000)){ // Changing the display time to 1000ms 
    cmd.print(F("Banana output"), banana);
}

```

### Setting Values

You can set values in your sketch based on commands received via Serial. Use the set() method to define how values
should be set. For example, to set a value named "temperature", you would write:

```cpp
float temperature;
if (cmd.set(F("temperature"), temperature)) {
    // Value has been set, do something with it...
}
```

## Online example / Playground

You can check online simulation example of the library here: \
https://wokwi.com/projects/392856762315911169

* `test 12` will set `testInt` to 12.
* `test show` Starts monitoring display of actual value in the sketch.
* `test stop` Stops monitoring.

## License

This library is released under the MIT License. See the LICENSE file for details.
