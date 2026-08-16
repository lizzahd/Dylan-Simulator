//
// Created by xerox on 8/8/2026.
//

#pragma once

#include <functional>

#include <nlohmann/json_fwd.hpp>

using json = nlohmann::json;

namespace core {
    using SignalCallback = std::function<void(const json &)>;

    class Signal {
    public:
        ~Signal() = default;
        Signal() = default;

        void emit(const json& args) const;
        void connect(int id, const SignalCallback &callback);
        void disconnect(int id);

    private:
        std::unordered_map<int, std::vector<SignalCallback>> m_callbacks;
    };

    struct SignalManager {
        inline static Signal input_event;
    };
}
