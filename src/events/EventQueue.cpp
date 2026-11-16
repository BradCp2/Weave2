//
// Created by mrble on 09/10/2026.
//

#include "EventQueue.h"

#include <iostream>

namespace weave::events
{
    EventQueue::EventQueue() = default;

    void EventQueue::pushEvent(
        EventType type,
        EventSource source,
        const std::string &message,
        std::size_t id)
    {
        {
            std::lock_guard lock(mutex_);

            if (stopping_) return;

            events_.emplace(type, source, message, id);
        }
        condition_.notify_one();
    }

    bool EventQueue::waitForEvent(Event& event)
    {
        std::unique_lock lock(mutex_);

        condition_.wait(lock, [this]
        {
            return !events_.empty() || stopping_;
        });

        if (events_.empty()) return false;
        event = std::move(events_.front());
        events_.pop();

        return true;
    }

    void EventQueue::stop()
    {
        {
            std::lock_guard lock(mutex_);
            stopping_ = true;
        }

        condition_.notify_all();
    }
}
