// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeastudioBoundingBox.generated.h"

UCLASS()
class LORDSOFCALADAN_API ABeastudioBoundingBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABeastudioBoundingBox();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounding Box Settings")
		float MovementBoundsLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounding Box Settings")
		float MovementBoundsWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounding Box Settings")
		float OffsetTraceZMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounding Box Settings")
		float OffsetTraceZMin;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
