# HardExit

An open.mp server component that terminates the server process immediately without sending RakNet disconnect packets. Players see **"Lost connection to the server"** (timeout) instead of **"Server closed the connection"**, causing them to automatically reconnect.

## Usage

### Pawn

```pawn
#include <hardexit>

// Example: restart command with a brief delay for messages to arrive
CMD:restart(playerid)
{
    SendClientMessageToAll(-1, "Server is restarting...");
    SetTimer("DoHardExit", 200, false);
    return 1;
}

forward DoHardExit();
public DoHardExit()
{
    HardExit();
}
```

### RCON / Server Console

```
hardexit
```

## Installation

1. Download `HardExit.so` (Linux) or `HardExit.dll` (Windows) from the [releases](https://github.com/IM-RP/open.mp-hardexit/releases)
2. Place it in your server's `components/` directory
3. Copy `hardexit.inc` to your `qawno/include/` directory

## License

Copyright (c) Italy Mafia Roleplay (www.sa-mp.im)
