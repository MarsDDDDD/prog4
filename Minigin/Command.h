#pragma once
class GameActor;
class Fire;

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};


	class GameActorCommand : public Command
	{
		GameActor* m_actor;
	protected:
		GameActor* GetGameActor() const { return m_actor; }
	public:
		GameActorCommand(GameActor* actor);
		virtual ~GameActorCommand();
	};

	class Fire : public GameActorCommand
	{
	public:
		void Execute() override
		{
			//GetGameActor()->Fire();
			// additional code is possible too, of course
		}
	};
}
// These commands need to be registered with the Input manager
// The input manager keeps track of all active commands
// Checks if the input for the command has been executed by the user
// Calls the Execute method on the registered command
// Don't forget to unregister!
