/**
 * @file CommandContext.h
 * @brief Defines the shared context provided to command implementations.
 */

#ifndef WEAVE2_COMMANDCONTEXT_H
#define WEAVE2_COMMANDCONTEXT_H

#include "cli/CLIRenderer.h"
#include "core/Repository.h"
#include "workers/WorkerManager.h"

namespace weave::commands::data
{
    /**
     * @struct CommandContext
     * @brief Provides access to shared application services.
     *
     * Groups references to services required by command implementations,
     * avoiding the need to pass each dependency individually.
     *
     * The context does not own these services; it holds references
     * to instances managed elsewhere by the application.
     *
     * @see ICommand
     * @see CommandRegistry
     */
    struct CommandContext
    {
        /** @brief Reference to the active repository. */
        weave::core::Repository& repository;

        /** @brief Reference to the worker manager. */
        weave::workers::WorkerManager& workerManager;

        /** @brief Reference to the application event queue. */
        weave::events::EventQueue& eventQueue;
    };
}

#endif // WEAVE2_COMMANDCONTEXT_H