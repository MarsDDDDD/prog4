// PengoGame.h (Game manager class)
#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "Direction.h"
    
namespace dae
{
    enum class BlockType
    {
        Empty,
        Ice,
        SnoBee,
        Diamond
    };

    class PengoGame
    {
    public:
        PengoGame(int width, int height);
        ~PengoGame() = default;
        
        void Initialize(Scene& scene);
        void Update(float deltaTime);
        
        // Grid manipulation
        BlockType GetBlockAt(int x, int y) const;
        void SetBlockAt(int x, int y, BlockType type);
        
        // Game actions
        void MovePlayer(Direction direction);
        void PushBlock(int x, int y, Direction direction);
        
    private:
        int m_Width;
        int m_Height;
        std::vector<BlockType> m_Grid;
        std::vector<GameObject*> m_BlockObjects;
        GameObject* m_Player{nullptr};
        
        void CreateGridLayout();
        void UpdateBlockVisuals();
    };
}
