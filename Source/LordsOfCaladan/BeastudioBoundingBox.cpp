// Fill out your copyright notice in the Description page of Project Settings.

#include "BeastudioBoundingBox.h"


// Sets default values
ABeastudioBoundingBox::ABeastudioBoundingBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementBoundsLength = 10000.0f;
	MovementBoundsWidth = 10000.0f;
	OffsetTraceZMax = 5000.0f;
	OffsetTraceZMin = -100.0f;

}

// Called when the game starts or when spawned
void ABeastudioBoundingBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABeastudioBoundingBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}