// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void AMyHUD::DrawHUD()
{
    Super::DrawHUD();

    if (bIsGameOver)
    {
        FString GameOverText = TEXT("Игра окончена. Нажмите Esc для выхода.");
        FVector2D TextSize;
        GetTextSize(GameOverText, TextSize.X, TextSize.Y, GEngine->GetLargeFont());

        FVector2D ScreenCenter(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
        FVector2D TextPosition(ScreenCenter.X - (TextSize.X * 0.5f), ScreenCenter.Y - (TextSize.Y * 0.5f));

        FCanvasTextItem TextItem(TextPosition, FText::FromString(GameOverText), GEngine->GetLargeFont(), FLinearColor::White);
        Canvas->DrawItem(TextItem);
    }
}

void AMyHUD::ShowGameOverMessage()
{
    bIsGameOver = true;
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
}
