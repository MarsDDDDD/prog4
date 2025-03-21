#pragma once
#include <steam_api.h>

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
struct Achievement_t
{
	int m_eAchievementID;
	const char* m_pchAchievementID;
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};


//Next we define a helper class that will wrap all of the Steam Stats API calls as well as creating all of the Steam callbacks.
class CSteamAchievements
{
private:
	int64 m_iAppID; // Our current AppID
	Achievement_t* m_pAchievements; // Achievements data
	int m_iNumAchievements; // The number of Achievements
	bool m_bInitialized; // Have we called Request stats and received the callback?

public:
	CSteamAchievements(Achievement_t* Achievements, int NumAchievements);
	~CSteamAchievements() = default;

	bool RequestStats();
	bool SetAchievement(const char* ID);
	//Check if achievement is already unlocked
	bool IsAchieved(const char* ID) {
		for (int i = 0; i < m_iNumAchievements; i++)
		{
			if (strcmp(m_pAchievements[i].m_pchAchievementID, ID) == 0)
			{
				return m_pAchievements[i].m_bAchieved;
			}
		}
		return false;
	}

	STEAM_CALLBACK(CSteamAchievements, OnUserStatsReceived, UserStatsReceived_t,
		m_CallbackUserStatsReceived);
	STEAM_CALLBACK(CSteamAchievements, OnUserStatsStored, UserStatsStored_t,
		m_CallbackUserStatsStored);
	STEAM_CALLBACK(CSteamAchievements, OnAchievementStored,
		UserAchievementStored_t, m_CallbackAchievementStored);
};