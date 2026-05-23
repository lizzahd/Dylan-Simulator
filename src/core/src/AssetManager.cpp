//
// Created by xerox on 3/14/2026.
//

#include <filesystem>
#include <ranges>

#include <AssetManager.h>

namespace fs = std::filesystem;

AssetManager::AssetManager() {
    // Load Textures
    for (const auto& entry : fs::directory_iterator("../../assets/textures/")) {
        fs::path path = entry.path();
        if (path.extension() != ".png") {
            continue;
        }

        m_textures.emplace(path.stem().string(), LoadTexture(path.string().c_str()));
    }

    // Load Sounds
    for (const auto& entry : fs::directory_iterator("../../assets/sounds/")) {
        fs::path path = entry.path();
        if (path.extension() != ".wav") {
            continue;
        }

        // Sound
        std::string key = path.stem().string();
        m_sounds.emplace(key, LoadSound(path.string().c_str()));

        // Sound Alias
        auto& aliases = m_soundAliases[key];
        for (int i = 0; i < m_soundAliasCount; i++) {
            aliases.push_back(LoadSoundAlias(m_sounds[key]));
        }
    }
}

AssetManager::~AssetManager() {
    // Unload textures
    for (const auto &tex: m_textures | std::views::values) {
        UnloadTexture(tex);
    }

    // Unload sounds and their aliases
    for (const auto& aliases: m_soundAliases | std::views::values) {
        for (const auto& alias : aliases) {
            UnloadSoundAlias(alias);
        }
    }

    for (const auto& sound : m_sounds | std::views::values) {
        UnloadSound(sound);
    }
}

raylib::Texture2D &AssetManager::getTex(const std::string& name) {
    return m_textures[name];
}

void AssetManager::playSound(const std::string& name) {
    for (const auto& alias : m_soundAliases[name]) {
        if (!IsSoundPlaying(alias)) {
            PlaySound(alias);
            return;
        }
    }
}
