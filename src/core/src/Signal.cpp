//
// Created by xerox on 8/8/2026.
//

#include <Signal.h>

#include <ranges>

#include <nlohmann/json.hpp>

void core::Signal::emit(const json& args = {}) const {
    for (const auto &callbacks : m_callbacks | std::views::values) {
        for (const auto &callback : callbacks) {
            callback(args);
        }
    }
}

void core::Signal::connect(const int id, const SignalCallback &callback) {
    if (m_callbacks.contains(id)) {
        m_callbacks[id].push_back(callback);
    } else {
        m_callbacks[id] = {callback};
    }
}

void core::Signal::disconnect(const int id) {
    m_callbacks.erase(id);
}
