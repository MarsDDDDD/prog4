#pragma once
namespace dae
{
	class GameObject; // Forward declare GameObject

	class BaseComponent
	{
	public:
		virtual ~BaseComponent() = default;
		virtual void Update(float /*deltaTime*/) {} // Pure virtual makes BaseComponent abstract
		virtual void FixedUpdate(float /*fixedTimeStep*/) {} // Pure virtual
		virtual void Render() const {} // Pure virtual

		GameObject* GetGameObject() const { return m_pGameObject; }
	protected:
		friend class GameObject;
		explicit BaseComponent(GameObject* pOwner = nullptr) : m_pGameObject(pOwner) {} // Add and make protected
		//void SetGameObject(GameObject* gameObject) { m_pGameObject = gameObject; } // Remove!
		GameObject* m_pGameObject{};
		GameObject* GetOwner() const;
		void SetOwner(GameObject* pParent);
		// Pointer to the owning GameObject.
		GameObject* m_pOwner;
	};
}