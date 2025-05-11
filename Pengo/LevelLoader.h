#pragma once
#include <vector>
#include <string>
#include "GameObject.h"
#include "NewGridComponent.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "Scene.h"

namespace dae
{
    class LevelLoader
    {
    public:
        // Represents different block types in your level
        enum class BlockType
        {
            Empty = 0,
            Ice = 1,
            Diamond = 2
        };

        static std::vector<int> ParseLevelString(const std::string& levelStr)
        {
            std::vector<int> levelData;
            for (char c : levelStr) {
                if (c >= '0' && c <= '9') {
                    levelData.push_back(c - '0');
                }
            }
            return levelData;
        }

        static void LoadLevel(const std::vector<int>& levelData, int gridWidth, int gridHeight,
            NewGridComponent* gridComponent,
            std::vector<GameObject*>& characters, Scene& scene)
        {
            if (levelData.size() != gridWidth * gridHeight) {
                throw std::runtime_error("Level data size doesn't match grid dimensions");
            }

            // Load block textures
            auto iceTexture = ResourceManager::GetInstance().LoadTexture("iceblock.png");
            auto diamondTexture = ResourceManager::GetInstance().LoadTexture("diamondblock.png");

            // Keep track of empty spaces for character spawning
            std::vector<std::pair<int, int>> emptySpaces;

            // Create blocks based on level data
            for (int y = 0; y < gridHeight; y++) {
                for (int x = 0; x < gridWidth; x++) {
                    int index = y * gridWidth + x;
                    int blockType = levelData[index];

                    if (blockType == static_cast<int>(BlockType::Empty)) {
                        // Keep track of empty spaces for character spawning
                        emptySpaces.push_back({ x, y });
                    }
                    else if (blockType == static_cast<int>(BlockType::Ice) ||
                        blockType == static_cast<int>(BlockType::Diamond)) {
                        // Create block
                        auto block = std::make_unique<GameObject>();

                        // Add texture based on block type
                        auto texture = (blockType == static_cast<int>(BlockType::Ice)) ?
                            iceTexture : diamondTexture;
                        auto blockTextureComponent = std::make_unique<TextureComponent>(block.get(), texture);
                        block->AddComponent(std::move(blockTextureComponent));

                        // Register the block with the grid
                        gridComponent->RegisterEntity(block.get(), x, y);
                        scene.Add(std::move(block));
                    }
                }
            }

            // Spawn characters in empty spaces
            if (!emptySpaces.empty() && !characters.empty()) {
                // Use different empty spaces for each character
                for (size_t i = 0; i < characters.size() && i < emptySpaces.size(); i++) {
                    auto [x, y] = emptySpaces[i];
                    gridComponent->RegisterEntity(characters[i], x, y);
                }
            }
        }
    };
}
