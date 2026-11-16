/**
 * @file TestCommand.h
 * @brief Declares test command.
 */

#ifndef WEAVE2_TESTCOMMAND_H
#define WEAVE2_TESTCOMMAND_H

#include "ICommand.h"
#include "commands/data/CommandContext.h"

#include <string>

namespace weave::commands
{
    /**
     * @class TestCommand
     * @brief Test command implementation.
     *
     * A concrete implementation of ICommand used for testing
     * command registration, creation and execution.
     */
    class TestCommand : public data::ICommand
    {
    public:
        /**
         * @brief Inherits contructors from ICommand.
         *
         * Allows test command to be constructed via factory with
         * both a ParsedCommand and CommandContext.
         */
        using ICommand::ICommand;

        /**
         * @brief Returns the name identifying this command.
         * @return The commands unique name.
         *
         * @see ICommand::name()
         */
        std::string name() const override;

    protected:
        /**
         * @brief Validates command-specific arguments and flags.
         * @note Test command has no validation.
         * @return True, this command should always be valid.
         */
        bool validate() const override;

        /**
         * @brief Executes command-specific implementation.
         * @note Test command has no implementation.
         * @return 0, this command should always execute.
         */
        int execute() override;
    };
}

#endif //WEAVE2_TESTCOMMAND_H
