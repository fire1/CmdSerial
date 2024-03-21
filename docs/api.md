# CmdSerial Library

**Methods:**

- [CmdSerial()](#cmdserial)
- [listen()](#listen)
- [show()](#show)
- [get()](#get)
- [set()](#set)
- [print()](#print)


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
bool show(const String &name, unsigned long milliSeconds = 500)
```

**Parameters**:

- `name`: The command name.
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

Sets data in the sketch with specific types.

**Syntax**:

```cpp
 bool set(const String name, T &data, bool isMessaging = true)
 ```

**Parameters**:

- `name`: The name of the command.
- `data`: The variable to store the data.
- `isMessaging`: Whether to print a message after setting the data (default is true).

- **Returns**:
  - `true`: If the data is successfully set.
  - `false`: If the data is not set.

---

### `print()`

Prints messages with different data types.

**Syntax**:

```cpp
 void print(const String &msg, T value)
```

**Parameters**:

- `msg`: The message to print.
- `value`: The value to print /`int`, `float`, `uint8_t` etc../.
- **Returns**: None.


