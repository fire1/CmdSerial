# CmdSerial Library

**Methods:**

- [CmdSerial()](#cmdserial)
- [listen()](#listen)
- [show()](#show)
- [get()](#get)
- [set()](#set)
- [print()](#print)
- [println()](#println)


### `CmdSerial()`

The constructor for the CmdSerial class.

**Syntax**:

```cpp
CmdSerial(Stream &theSerial = Serial, const char separator = ' ', const char endOfLine = '\n')
```

**Parameters**:

- `theSerial`: The Serial stream to be used (default is Serial).
- `separator`: The separator character for command parsing (default is space ' ').
- `endOfLine`: The end-of-line character for command parsing (default is newline '\n').

**Returns**: None.

---

### `listen()`

Listens for Serial stream input and parses it based on methods used in this class. This method is required to be called
in the `loop()` function for this class to work.

**Syntax**:

```cpp
void listen()
```

**Parameters**: None.

**Returns**: None.

---

### `show()`

Shows data for given cycles.

**Syntax**:

```cpp
bool show(const String &name)
bool show(const String &name, const String &help)
bool show(const String &name, unsigned long milliSeconds)
bool _show(const String &name, unsigned long milliSeconds = 500, const String &help = "")
```

**Parameters**:

- `name`: The command name.
- `help`: Optional help text shown by `help` / `<name> help`.
- `milliSeconds`: Time/interval used to display data (default is 500 milliseconds).

**Returns**:

- `true`: If data is ready to be shown.
- `false`: If no data is available or not ready to be shown.

---

### `get()`

Gets the data received via Serial.

**Syntax**:

```cpp
String get()
```

**Parameters**: None.

**Returns**: The received data as a `String` object.

---

### `set()`

Sets data in the sketch. One templated overload covers all numeric types and `String`.

**Syntax**:

```cpp
bool set(const String &name)
bool set(const String &name, T &data, const String &help = "", bool isMessaging = true)
```

**Parameters**:

- `name`: The name of the command.
- `data`: The variable to store the data (`int`, `float`, `uint8_t`, `String`, …).
- `help`: Optional help text.
- `isMessaging`: Whether to print a confirmation after setting the data (default is true).

**Returns**:

- `true`: If the data is successfully set.
- `false`: If the data is not set.

---

### `print()`

Prints a label and a value on one line (`label <sep> value`). Works with any type `Stream::print` supports.

**Syntax**:

```cpp
void print(const Label &label, const Value &value)
```

**Parameters**:

- `label`: The message / label to print (`"..."_f`, `F("...")`, `String`, `const char*`, …).
- `value`: The value to print (`int`, `float`, `uint8_t`, `String`, …).

Prefer `"label"_f` (or `F("label")`) so the text stays in PROGMEM on AVR instead of RAM.

**Returns**: None.

---

### `"..."_f` (PROGMEM literal)

User-defined literal equivalent to `F("...")`. Defined when you include `CmdSerial.h`.

```cpp
cmd.print("Test value:"_f, testInt);
// same as:
cmd.print(F("Test value:"), testInt);
```

---

### `println()`

Prints one or more values on a single line, separated by the command separator.

**Syntax**:

```cpp
void println(const T &value)
void println(const T &first, const Args &... rest)
```

**Example**:

```cpp
cmd.println("temp"_f, temperature, "hum"_f, humidity);
// -> temp 21.50 hum 40
```

**Returns**: None.
