
# CmdSerial Library

## Overview

CmdSerial is a simple Arduino library that provides a command parsing functionality for Serial communication. It allows you to define commands and their corresponding actions in your sketch, making it easier to interact with your Arduino device via the Serial monitor.

## Features

- Parse incoming Serial commands
- Define actions for specific commands
- Set sketch variables from Serial (any numeric type or String via a single templated API)
- Print labels and values without per-type overloads
- Print several fields on one line with `println(...)`
- PROGMEM string literal `"..."_f` (same as `F("...")`)
