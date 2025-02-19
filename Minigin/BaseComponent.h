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

		//void SetGameObject(GameObject* gameObject) { m_gameObject = gameObject; } // Make protected!
		GameObject* GetGameObject() const { return m_gameObject; }
	protected:
		friend class GameObject;
		void SetGameObject(GameObject* gameObject) { m_gameObject = gameObject; }
		GameObject* m_gameObject{}; // Pointer to the owning GameObject.
	};
}