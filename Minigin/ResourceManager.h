#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"
#include "SoundSystem.h"

namespace dae
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::filesystem::path& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
		std::shared_ptr<Font> LoadFont(const std::string& file, uint8_t size);
		
		// New audio related methods
		void LoadSound(const sound_id id, const std::string& file);
		std::string GetAudioPath(const std::string& file) const;

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::filesystem::path m_dataPath;

		void UnloadUnusedResources();

		std::map<std::string, std::shared_ptr<Texture2D>> m_LoadedTextures;
		std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> m_LoadedFonts;
	};
}
