//
// Created by mrble on 09/10/2026.
//

#include "CommandDispatcher.h"

#include "commands/data/CommandRegistry.h"
#include "CommandParser.h"
#include "commands/ICommand.h"

namespace weave::cli
{
    CommandDispatcher::CommandDispatcher(
        const commands::data::CommandRegistry &commandRegistry,
        commands::data::CommandContext &commandContext
    )
        : commandRegistry(commandRegistry),
          commandContext(commandContext)
    {
    }

    int CommandDispatcher::dispatch(const data::parsedCommand &parsedCommand) const
    {
        auto handler = commandRegistry.createCommand(
            parsedCommand.command,
            parsedCommand,
            commandContext
        );

        if (!handler)
        {
            commandContext.eventQueue.pushEvent
            (
                events::EventType::Error,
                events::EventSource::Dispatcher,
                "Command not found : \"" + parsedCommand.command + "\"\n"
            );
            return 1;
        }

        return handler->tryExecute();
    }
} // cli
