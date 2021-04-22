// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnemyCharacter.h"
#include "Blueprint/UserWidget.h"
#include "R6CopyGameModeBase.generated.h"


/**
 * 
 */

class AEnemyCharacter;


UCLASS()
class R6COPY_API AR6CopyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, Category = KillCount)
	int32 _KillCount;
public:

	int32 GetKillCount();

	AEnemyCharacter* EnemyCharacterRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WinScreen)
		TSubclassOf<class UUserWidget> wWinScreen;

	UPROPERTY()
		UUserWidget* m_WinScreen;

		bool IsRoundWon;

	void CreateWinScreen();
};


