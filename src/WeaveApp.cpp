//
// Created by mrble on 09/10/2026.
//

#include "WeaveApp.h"

#include "commands/TestCommand.h"

#include <iostream>
#include <ostream>
#include <memory>

#include "cli/CommandDispatcher.h"

weave::WeaveApp::WeaveApp() :
    workerManager_(eventQueue_),
    renderer_(eventQueue_),
    parser_(eventQueue_),
    context_{
        .repository = repository_,
        .workerManager = workerManager_,
        .eventQueue = eventQueue_,
    },
    dispatcher_(commandRegistry_, context_) {}

weave::WeaveApp::~WeaveApp() = default;

void weave::WeaveApp::registerCommands()
{
    commandRegistry_.registerCommand(
        "test",
        [](
            const cli::data::parsedCommand& parsedCommand,
            commands::data::CommandContext& commandContext)
        {
            return std::make_unique<commands::TestCommand>(
                parsedCommand,
                commandContext
            );
        }
    );

}

int weave::WeaveApp::run(const std::vector<std::string>& tokens)
{
    parser_.parse(tokens);

    registerCommands();

    renderer_.setVerbose(parser_.hasVerbose());
    renderer_.setDebug(parser_.hasDebug());

    renderer_.start();

    eventQueue_.pushEvent
    (
        events::EventType::Debug,
        events::EventSource::Parser,
        parser_.toString()
    );

    if (!parser_.getParsedCommand().validFlag) { return 1;}

    return dispatcher_.dispatch(parser_.getParsedCommand());
}

