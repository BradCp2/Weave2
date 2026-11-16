//
// Created by mrble on 09/10/2026.
//

#include "CLIRenderer.h"

#include <iostream>

namespace weave::cli
{
    CLIRenderer::CLIRenderer(events::EventQueue& eventQueue) : eventQueue(eventQueue) {}

    CLIRenderer::~CLIRenderer()
    {
        stop();
    }

    void CLIRenderer::start()
    {
        if (_renderThread.joinable()) return;

        _renderThread = std::thread(&CLIRenderer::renderLoop, this);
    }

    void CLIRenderer::stop()
    {
        eventQueue.stop();

        if (_renderThread.joinable()) _renderThread.join();
    }

    void CLIRenderer::renderLoop()
    {
        events::Event event;

        while (eventQueue.waitForEvent(event))
        {
            renderEvent(event);
        }
    }

    void CLIRenderer::renderEvent(const events::Event &event)
    {
        if (!shouldDisplay(event.type)) return;

        std::cout
            << toString(event.type)
            << " (" << toString(event.source) << ") "
            << event.message;
    }

    bool CLIRenderer::shouldDisplay(events::EventType type) const
    {
        switch (type)
        {
            case events::EventType::Error:
            case events::EventType::Warning:
                return true;

            case events::EventType::Success:
            case events::EventType::Information:
                return isDebug || isVerbose;

            case events::EventType::Debug:
                return isDebug;
        }

        return false;
    }

    void CLIRenderer::setVerbose(bool enabled)
    {
        isVerbose = enabled;
    }

    void CLIRenderer::setDebug(bool enabled)
    {
        isDebug = enabled;
    }
}
