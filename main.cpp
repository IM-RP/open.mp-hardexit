/*
 *  HardExit component for open.mp
 *
 *  Terminates the server process immediately via std::abort() so that no
 *  RakNet disconnect packet is sent to clients.  Players see "Lost connection
 *  to the server" (timeout) instead of "Server closed the connection".
 *
 *  Usage:
 *    RCON / server console:  hardexit
 *    Pawn:                   native HardExit();
 */

#include <Server/Components/Console/console.hpp>
#include <Server/Components/Pawn/pawn.hpp>
#include <sdk.hpp>

#include <cstdlib>

#ifndef HARDEXIT_VERSION_MAJOR
#define HARDEXIT_VERSION_MAJOR 1
#define HARDEXIT_VERSION_MINOR 0
#define HARDEXIT_VERSION_PATCH 0
#define HARDEXIT_VERSION_PREREL 0
#endif

static ICore* g_core = nullptr;

// -- Pawn native --------------------------------------------------------------

static cell AMX_NATIVE_CALL n_HardExit(AMX*, const cell*)
{
	if (g_core)
	{
		g_core->logLn(LogLevel::Message, "HardExit: non-graceful server termination initiated via script.");
	}
	std::abort();
	return 0;
}

static const AMX_NATIVE_INFO natives[] = {
	{ "HardExit", n_HardExit },
	{ nullptr, nullptr }
};

// -- Component ----------------------------------------------------------------

struct HardExitComponent final
	: public IComponent
	, public ConsoleEventHandler
	, public PawnEventHandler
{
	ICore* core = nullptr;
	IConsoleComponent* console = nullptr;
	IPawnComponent* pawn = nullptr;

	// IComponent

	UID getUID() override { return UID(0xa1b2c3d4e5f60001); }

	StringView componentName() const override { return "HardExit"; }

	SemanticVersion componentVersion() const override
	{
		return SemanticVersion(HARDEXIT_VERSION_MAJOR, HARDEXIT_VERSION_MINOR, HARDEXIT_VERSION_PATCH, HARDEXIT_VERSION_PREREL);
	}

	void onLoad(ICore* c) override
	{
		core = c;
		g_core = c;
		core->logLn(LogLevel::Message, "  (c) Italy Mafia Roleplay (www.sa-mp.im)");
	}

	void onInit(IComponentList* components) override
	{
		console = components->queryComponent<IConsoleComponent>();
		if (console)
		{
			console->getEventDispatcher().addEventHandler(this);
		}

		pawn = components->queryComponent<IPawnComponent>();
		if (pawn)
		{
			pawn->getEventDispatcher().addEventHandler(this);
		}
	}

	void onFree(IComponent* component) override
	{
		if (console && component == console)
		{
			console->getEventDispatcher().removeEventHandler(this);
			console = nullptr;
		}
		if (pawn && component == pawn)
		{
			pawn->getEventDispatcher().removeEventHandler(this);
			pawn = nullptr;
		}
	}

	void free() override { delete this; }

	void reset() override { }

	// ConsoleEventHandler

	bool onConsoleText(StringView command, StringView parameters, const ConsoleCommandSenderData& sender) override
	{
		if (command == "hardexit")
		{
			core->logLn(LogLevel::Message, "HardExit: non-graceful server termination initiated via console.");
			std::abort();
			return true;
		}
		return false;
	}

	void onConsoleCommandListRequest(FlatHashSet<StringView>& commands) override
	{
		commands.emplace("hardexit");
	}

	// PawnEventHandler

	void onAmxLoad(IPawnScript& script) override
	{
		script.Register(natives, -1);
	}

	void onAmxUnload(IPawnScript& script) override { }
};

COMPONENT_ENTRY_POINT()
{
	return new HardExitComponent();
}
