//
// Created by mrble on 19/10/2026.
//

#include "TestCommand.h"

#include <iostream>

std::string weave::commands::TestCommand::name() const {
    return "test";
}

bool weave::commands::TestCommand::validate() const {
    return true;
}

int weave::commands::TestCommand::execute()
{
    context_.eventQueue.pushEvent(
        events::EventType::Success,
        events::EventSource::Command,
        "Executed Successfully.\n"
        );

    return 1;
}
