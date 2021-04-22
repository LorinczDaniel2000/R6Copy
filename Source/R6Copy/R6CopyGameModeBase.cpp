// Copyright Epic Games, Inc. All Rights Reserved.

#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.h"
#include "R6CopyGameModeBase.h"
#include "Blueprint/UserWidget.h"


int32 AR6CopyGameModeBase::GetKillCount()
{
	if (_KillCount >= 4)
	{

		if ( wWinScreen != nullptr )
		{
			m_WinScreen = CreateWidget<UUserWidget>(GetWorld(), wWinScreen);
			CreateWinScreen();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("wWinScreen is pointing to 0"));
		}
		IsRoundWon = true;
	}
	else
	{
		++_KillCount;
		UE_LOG(LogTemp, Warning, TEXT("Kills %d"), _KillCount);

		return _KillCount;
	}

	return _KillCount;

}

void AR6CopyGameModeBase::CreateWinScreen()
{
	if (m_WinScreen != nullptr)
	{
		if (m_WinScreen->GetIsVisible() == false)
		{
			m_WinScreen->AddToViewport();
		}
		
	}


}

