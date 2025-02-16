#pragma once
#include <memory>
#include "Transform.h"

namespace dae
{
	class Texture2D;

	// todo: this should become final, after the textobject is changed into textcomponent
	class GameObject //final
	{
	public:
		virtual void Update(float deltaTime); // Modified Update method
		virtual void FixedUpdate(float /*fixedTimeStep*/) {}; // New FixedUpdate method - default empty implementation
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_texture{};
	};
}