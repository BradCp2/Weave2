/**
 * @file CLIRenderer.h
 * @brief Declares the CLI Renderer.
 */

#ifndef WEAVE2_CLIRENDERER_H
#define WEAVE2_CLIRENDERER_H

#include "events/EventQueue.h"
#include <thread>
#include <atomic>
#include <string_view>

namespace weave::cli
{
    /**
     * @class CLIRenderer
     * @brief Handles the presentation of application events in the CLI.
     *
     * Runs a dedicated rendering thread that consumes events from an
     * EventQueue and displays them according to the configured verbosity
     * and debug settings.
     *
     * Supports filtering events by type and formatting output with
     * appropriate labels and terminal colours.
     *
     * @see weave::events::EventQueue
     * @see weave::events::Event
     */
    class CLIRenderer
    {
        public:
        /**
         * @brief Constructs a CLI renderer.
         *
         * @param eventQueue Reference to the event queue consumed by the renderer.
         */
        explicit CLIRenderer(events::EventQueue& eventQueue);

        /**
         * @brief Stops the rendering thread and releases resources.
         */
        ~CLIRenderer();

        /**
         * @brief Starts the rendering thread.
         *
         * Begins consuming and rendering events from the associated EventQueue.
         */
        void start();

        /**
         * @brief Stops the rendering thread.
         *
         * Signals the event queue to stop and waits for the rendering thread
         * to finish processing any remaining events.
         */
        void stop();

        /**
         * @brief Enables or disables verbose output.
         *
         * @param enabled True to enable verbose output, otherwise false.
         */
        void setVerbose(bool enabled);

        /**
         * @brief Enables or disables debug output.
         *
         * @param enabled True to enable debug output, otherwise false.
         */
        void setDebug(bool enabled);

        private:
        events::EventQueue& eventQueue;
        std::thread _renderThread;

        std::atomic<bool> isVerbose{false};
        std::atomic<bool> isDebug{false};

        /**
         * @brief Continuously retrieves and renders queued events.
         *
         * Runs on the dedicated rendering thread until the event queue
         * is stopped and no events remain.
         */
        void renderLoop();

        /**
         * @brief Formats and displays an individual event.
         *
         * Applies event filtering before writing the formatted message
         * to the console.
         *
         * @param event Event to render.
         */
        void renderEvent(const events::Event& event);

        /**
         * @brief Determines whether an event should be displayed.
         *
         * Uses the current verbose and debug settings to filter events.
         *
         * @param type Type of event to evaluate.
         * @return True if the event should be displayed, otherwise false.
         */
        bool shouldDisplay(events::EventType type) const;

        /**
         * @brief Converts an event type into its formatted display label.
         *
         * @param type Event type to convert.
         * @return String view containing the formatted event label.
         */
        static constexpr std::string_view toString(events::EventType type)
        {
            switch (type) {
                case events::EventType::Error: return "\033[31m[Error]\033[0m";
                case events::EventType::Warning: return "\033[33m[Warning]\033[0m";
                case events::EventType::Debug: return "\033[34m[Debug]\033[0m";
                case events::EventType::Success: return "\033[32m[Success]\033[0m";
                case events::EventType::Information: return "[*]";
            }
            return "\033[31m [Unknown]\033[0m";
        }

        /**
         * @brief Converts an event source into its display name.
         *
         * @param source Event source to convert.
         * @return String view containing the source name.
         */
        static constexpr std::string_view toString(events::EventSource source)
        {
            switch (source) {
                case events::EventSource::Parser: return "Parser";
                case events::EventSource::Dispatcher: return "Dispatcher";
                case events::EventSource::Command: return "Command";
                case events::EventSource::Worker: return "Worker";
            }
            return "Unknown";
        }
    };
}

#endif //WEAVE2_CLIRENDERER_H
