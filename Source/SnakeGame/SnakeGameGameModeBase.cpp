// Copyright Epic Games, Inc. All Rights Reserved.


#include "SnakeGameGameModeBase.h"
#include "GameFramework/PlayerController.h"

void ASnakeGameGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
    }
}

void ASnakeGameGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    HandleEscapeKey();
}

void ASnakeGameGameModeBase::HandleEscapeKey()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (PC->WasInputKeyJustPressed(EKeys::Escape))
        {
            FGenericPlatformMisc::RequestExit(false);
        }
    }
}