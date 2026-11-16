//
// Created by mrble on 19/10/2026.
//

#include "CommandRegistry.h"

namespace weave::commands::data
{
    void CommandRegistry::registerCommand(
        const std::string &name,
        CommandFactory factory)
    {
        if (!factory)
        {
            // TODO DEBUG ERROR null factory
            return;
        }

        if (name.empty())
        {
            // TODO DEBUG ERROR empty command name
            return;
        }

        const auto [it, inserted] = commandSet.emplace(
            name,
            std::move(factory)
        );

        if (!inserted)
        {
            //TODO DEBUG ERROR Duplicate command
            return;
        }
    }

    std::unique_ptr<ICommand> CommandRegistry::createCommand(
        const std::string &name,
        const cli::data::parsedCommand &parsedCommand,
        CommandContext &context) const
    {
        const auto it = commandSet.find(name);

        if (it == commandSet.end()) {
            return nullptr;
        }
        
        return it->second(parsedCommand, context);
    }
} // commands