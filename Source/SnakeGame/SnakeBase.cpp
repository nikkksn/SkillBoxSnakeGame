// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Interactable.h"
#include "Food.h"
#include "MyHUD.h"
#include "BoundaryWall.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 10.f;
	LastMoveDirection = EMovementDirection::DOWN;
	bCanChangeDirection = true;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(5);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; ++i)
	{
		FVector NewLocation;

		if (SnakeElements.Num() > 0)
		{
			ASnakeElementBase* LastElement = SnakeElements.Last();
			FVector LastLocation = LastElement->GetActorLocation();
			FVector DirectionOffset = FVector::ZeroVector;

			switch (LastMoveDirection)
			{
			case EMovementDirection::UP:
				DirectionOffset = FVector(-ElementSize, 0, 0);
				break;
			case EMovementDirection::DOWN:
				DirectionOffset = FVector(ElementSize, 0, 0);
				break;
			case EMovementDirection::LEFT:
				DirectionOffset = FVector(0, ElementSize, 0);
				break;
			case EMovementDirection::RIGHT:
				DirectionOffset = FVector(0, -ElementSize, 0);
				break;
			}

			NewLocation = LastLocation + DirectionOffset;
		}
		else
		{
			// Начальная позиция для первого элемента змейки
			NewLocation = GetActorLocation();
		}

		FTransform NewTransform = FTransform(NewLocation);
		ASnakeElementBase* NewSnakeElement = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewSnakeElement->SnakeOwner = this;
		int32 ElementIndex = SnakeElements.Add(NewSnakeElement);
		if (ElementIndex == 0)
		{
			NewSnakeElement->SetFirstElementType();
		}
	}
}

void ASnakeBase::Move()
{
	FVector MovementVector = FVector::ZeroVector;

	switch (LastMoveDirection)
	{
	case EMovementDirection::UP:
		MovementVector.X += ElementSize;
		break;
	case EMovementDirection::DOWN:
		MovementVector.X -= ElementSize;
		break;
	case EMovementDirection::LEFT:
		MovementVector.Y += ElementSize;
		break;
	case EMovementDirection::RIGHT:
		MovementVector.Y -= ElementSize;
		break;
	}

	SnakeElements[0]->ToggleCollision();

	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
	}

	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();

	// Разрешаем изменение направления после обновления позиции
	bCanChangeDirection = true;
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other)
{
	if (IsValid(OverlappedElement))
	{
		int32 ElemIndex;
		SnakeElements.Find(OverlappedElement, ElemIndex);
		bool bIsFirst = ElemIndex == 0;
		IInteractable* IInteractableInterface = Cast<IInteractable>(Other);
		if (IInteractableInterface)
		{
			IInteractableInterface->Interact(this, bIsFirst);

			// Проверяем, является ли объект едой
			if (Other->IsA<AFood>())
			{
				// Генерируем случайное число от 0 до 1
				float RandomValue = FMath::FRand();
				// Если случайное число меньше или равно 0.5 (50% вероятность)
				if (RandomValue <= 0.5f)
				{
					// Увеличиваем скорость змейки в 2 раза
					ChangeMovementSpeed(2.0f);
				}
			}
		}
	}
}

void ASnakeBase::ChangeMovementSpeed(float SpeedMultiplier)
{
	MovementSpeed /= SpeedMultiplier;
	SetActorTickInterval(MovementSpeed);
}

void ASnakeBase::GameOver()
{
	if (AMyHUD* MyHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->ShowGameOverMessage();
	}
	Destroy();
}

