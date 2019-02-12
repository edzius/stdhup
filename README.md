# StdHUP

Library for runtime STDIO switching.

## Building

> make

### Targets

- stdhup.o -- library object for linking
- luahup.so -- LUA bindings module

### Utils

- runhup -- C test utility
- testhup.lua -- LUA test utility

## Running

1. Open first terminal

C utility:

> ./runhup

LUA script:

> ./testhup.lua

2. Open other terminal

> kill -HUP `pidof runhup`
or
> kill -HUP `pidof testhup.lua`

## Program API

Noting needed, just require("luahup") module and call
returned function to initialize facility:

```
local hup = require("luahup")
hup()
```

## Usage

- Send SIGHUP once switch STDIO to current TTY
- Send SIGHUP once more switch STDIO to /dev/null
- Send SIGHUP once more switch STDIO to current TTY
- Send SIGHUP from other terminal switch STDIO to current TTY
