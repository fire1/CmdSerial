# CmdSerial

Debug and control Arduino sketches from the Serial Monitor.

Set variables, watch live values, and list commands — without scattering `Serial.print` calls through your firmware.

## Features

- **Set** sketch variables from Serial (`temp 23.4`)
- **Show / stop** live monitoring of values (`sensors show`)
- Built-in **help** listing for all registered commands
- Templated `set()` / `print()` — works with `int`, `float`, `String`, and other numeric types
- `"..."_f` PROGMEM labels (same idea as `F("...")`, less noise)
- `println(...)` to dump several fields on one line

## Installation

**Arduino Library Manager:** search for `CmdSerial`, or install from ZIP / clone into your `libraries` folder:

```bash
git clone https://github.com/fire1/CmdSerial.git
```

Then restart the Arduino IDE.

## Quick start

```cpp
#include <CmdSerial.h>

CmdSerial cmd;
int testInt = 0;
float temperature = 21.5;

void setup() {
    Serial.begin(9600);
}

void loop() {
    cmd.listen();  // required every loop

    cmd.set("test"_f, testInt, "Set the test integer"_f);
    cmd.set("temp"_f, temperature, "Set temperature"_f);

    if (cmd.show("test"_f, "Monitor testInt"_f)) {
        cmd.print("Test value:"_f, testInt);
    }

    if (cmd.show("sensors"_f, "Print several values"_f)) {
        cmd.println("temp"_f, temperature, "test"_f, testInt);
    }
}
```

Open the Serial Monitor (newline ending). Try:

| Command | Effect |
|---------|--------|
| `help` | List all commands |
| `test help` | Help for one command |
| `test 12` | Set `testInt` to `12` |
| `temp 23.4` | Set `temperature` |
| `test show` | Start monitoring |
| `test stop` | Stop monitoring |
| `sensors show` | Print several fields each interval |

## Usage

### Setup

```cpp
#include <CmdSerial.h>
CmdSerial cmd;           // uses Serial by default
// CmdSerial cmd(Serial1);  // or any Stream
```

Call `cmd.listen()` once per `loop()`.

### Set a variable

```cpp
float temperature;
if (cmd.set("temperature"_f, temperature, "Board temperature"_f)) {
    // optional: ran only when a new value was applied
}
```

Serial input: `temperature 21.5`

`set()` works with numeric types and `String`. Confirmation messaging can be disabled with a fourth argument: `false`.

### Monitor values (`show`)

```cpp
if (cmd.show("data"_f)) {                    // every 500 ms while active
    cmd.print("Output"_f, testInt);
}

if (cmd.show("banana"_f, "Help text"_f, 1000)) {  // custom interval
    cmd.print("Banana"_f, banana);
}
```

- `name show` — start printing on the interval
- `name stop` — stop

### Print helpers

```cpp
cmd.print("label"_f, value);                           // one line: label <sep> value
cmd.println("temp"_f, temperature, "hum"_f, humidity); // several fields on one line
```

### PROGMEM strings (`_f`)

`"Hello"_f` keeps the text in flash on AVR, like `F("Hello")`:

```cpp
cmd.print("Test value:"_f, testInt);
// same as:
cmd.print(F("Test value:"), testInt);
```

### Help

| Input | Result |
|-------|--------|
| `help` | Lists every `set` / `show` command registered in the sketch |
| `<name> help` | Help text for that command |

### Keywords

Default separator is space; end of line is `\n`.

| Keyword | Role |
|---------|------|
| `show` | Start monitoring |
| `stop` | Stop monitoring |
| `help` | List or describe commands |

Override via macros before including the header (`CmdSerial_KeywordShow`, `CmdSerial_ShowTime`, …) — see `src/CmdSerial.h`.

## Try it online

[Wokwi playground](https://wokwi.com/projects/392856762315911169) — type `help` in the terminal.

## Documentation

Full API: [docs/api.md](docs/api.md)

## License

MIT — see [LICENSE.txt](LICENSE.txt).
