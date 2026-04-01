#if defined _hardexit_included
	#endinput
#endif
#define _hardexit_included

/// Terminates the server process immediately without sending disconnect
/// packets. Players will see "Lost connection to the server" instead of
/// "Server closed the connection".
native HardExit();
