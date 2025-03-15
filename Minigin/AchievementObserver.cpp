#include "AchievementObserver.h"
#include "GameObject.h"
#include "ScoreComponent.h"
#include <iostream>
using namespace dae;

AchievementObserver::AchievementObserver()
{
	m_SteamAchievements = new CSteamAchievements(m_Achievements.data(), 4);
}

AchievementObserver::~AchievementObserver()
{
	delete m_SteamAchievements;
}

void AchievementObserver::Notify(const GameObject* actor, EventId event)
{
	switch (event)
	{
	case EventId::SCORE_UPDATED:
	{
		if (actor->GetComponent<ScoreComponent>()->GetScore() >= 500)
		{
			Unlock("ACH_WIN_ONE_GAME");
			std::cout << "Achievement unlocked: Winner" << std::endl;
		}
		break;
	}
	}
}

void AchievementObserver::Unlock(const char* ID) const
{
	m_SteamAchievements->SetAchievement(ID);
}
