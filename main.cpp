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

// -- Pawn native --------------------------------------------------------------

static cell AMX_NATIVE_CALL n_HardExit(AMX*, cell*)
{
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

	SemanticVersion componentVersion() const override { return SemanticVersion(1, 0, 0, 0); }

	void onLoad(ICore* c) override
	{
		core = c;
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
			core->logLn(LogLevel::Message, "HardExit: terminating server process immediately.");
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
