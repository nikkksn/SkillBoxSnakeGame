// Fill out your copyright notice in the Description page of Project Settings.


#include "BoundaryWall.h"

// Sets default values
ABoundaryWall::ABoundaryWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh;
	WallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WallMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void ABoundaryWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoundaryWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

