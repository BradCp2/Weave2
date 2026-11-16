/**
 * @file CommandRegistry.h
 * @brief Declares the command registry.
 */

#ifndef WEAVE2_COMMANDREGISTRY_H
#define WEAVE2_COMMANDREGISTRY_H

#include <functional>
#include <memory>
#include "commands/ICommand.h"

namespace weave::commands::data
{
    /**
     * @brief Factory function used to create command instances.
     *
     * Accepts a parsed command and application context, returning
     * a newly constructed command implementation.
     *
     * @see weave::commands::ICommand
     * @see weave::commands::data::CommandContext
     */
    using CommandFactory = std::function<std::unique_ptr<ICommand>(
        const cli::data::parsedCommand& parsedCommand,
        CommandContext& context
    )>;

    /**
    * @class CommandRegistry
    * @brief Maps command names to their corresponding factories.
    *
    * Stores command factory functions and provides a mechanism
    * for creating command instances without requiring the
    * dispatcher to depend on individual command instances.
    *
    * @see weave::commands::ICommand
    * @see CommandFactory
    * @see weave::cli::CommandDispatcher
    * @see weave::WeaveApp::registerCommands
    */
    class CommandRegistry
    {
    public:
        /**
         * @brief Registers a command factory.
         *
         * Associates a command name with a factory function that can be
         * used to construct the corresponding command.
         *
         * @param name Unique name used to identify each command.
         * @param factory Factory function used to create the command.
         */
        void registerCommand(const std::string& name, CommandFactory factory);

        /**
         * @brief Creates a factory instance by associated name.
         *
         * Looks up the registered factory in commandSet and invokes it using
         * the supplied parsed command data and context.
         *
         * @param name Name of the command to be created.
         * @param parsedCommand Parsed command data containing users input.
         * @param context Shared application context passed to the command.
         *
         * @return A unique pointer to the created command.
         */
        std::unique_ptr<ICommand> createCommand
        (
            const std::string& name,
            const cli::data::parsedCommand& parsedCommand,
            CommandContext& context
        ) const;

    private:
        std::unordered_map<std::string, CommandFactory> commandSet;
    };
} // commands

#endif //WEAVE2_COMMANDREGISTRY_H
