// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UObject/ObjectMacros.h"
#include "ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BeastudioBoundingBox.h"
#include "BeastudioStrategyCamera.generated.h"

UCLASS(Blueprintable, ClassGroup = Camera, meta = (BlueprintSpawnableComponent))
class LORDSOFCALADAN_API UBeastudioStrategyCameraComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBeastudioStrategyCameraComponent();

	// Public functions
	UFUNCTION(BlueprintCallable, Category = "Strategy Camera Functions")
		void SetBoundingBoxReference(class ABeastudioBoundingBox* GivenBoundingBox);

	UFUNCTION(BlueprintCallable, Category = "Strategy Camera Functions")
		void MoveCamera(float X, float Y, bool SetRotation, float Rotation, bool SetDistance, float Distance, bool Teleport);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Strategy Camera Functions")
		bool IsCameraMoving();

	UFUNCTION(BlueprintCallable, Category = "Strategy Camera Functions")
		void SetInputEnabled(bool InputEnabled);

	UFUNCTION(BlueprintCallable, Category = "Strategy Camera Functions")
		void CameraMoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = "Strategy Camera Functions")
		void CameraMoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Strategy Camera Functions")
		void CameraRotate(float Value);

	UFUNCTION(BlueprintCallable, Category = "Strategy Camera Functions")
		void CameraZoomIn();

	UFUNCTION(BlueprintCallable, Category = "Strategy Camera Functions")
		void CameraZoomOut();

	UFUNCTION(BlueprintCallable, Category = "Strategy Camera Functions")
		void CameraResetRotation();

	// Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float MovementBoundsLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float MovementBoundsWidth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float OffsetTraceZmax;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float OffsetTraceZmin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float InitialX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float InitialY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float InitialRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float InitialDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float MinDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float MaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float ScrollBorderSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float MaxMovementSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float ZoomSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Camera Settings")
		float Smoothing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component Setup")
		bool boxOk = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component Setup")
		bool meshOk = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component Setup")
		bool armOk = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component Setup")
		bool camOk = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component Coordination")
		float TargetX = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component Coordination")
		float TargetY = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component Coordination")
		float TargetZ = 0.0f;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Activate(bool bReset) override;

private:
	// Subroutines
	void _InitialValues();
	void _ConstructionScript();
	void _SetTargetZ();
	void _SetAdjustedMovementSpeed();
	void _AddRotation(float Rotation);
	void _SetTargetXY(FVector DeltaLocation);
	void _AddMovementX(float X);
	void _AddMovementY(float Y);
	void _UpdateMovement();
	void _UpdateRotation();
	void _UpdateZoom();
	void _ProcessMouseWindowBorderScroll();

	// Components

	class UStaticMeshComponent* TargetMesh;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	class ABeastudioBoundingBox* BoundingBox;

	// Variables
	float MinX = 0.0f;
	float MaxX = 0.0f;
	float MinY = 0.0f;
	float MaxY = 0.0f;
	float TraceStartZ = 0.0f;
	float TraceEndZ = 0.0f;
	float MovementSpeedRange = 0.0f;
	float AdjustedMovementSpeed = 0.0f;
	float CurrentRotation = 0.0f;
	float TargetRotation = 0.0f;
	float TargetDistance = 0.0f;
	bool IsInputEnabled = true;
	bool WasInputEnabled = true;
	bool IsMoving = false;
	bool IsTouch1 = false;
	bool IsTouch2 = false;
	FVector2D TouchLocation1 = FVector2D(0.0f, 0.0f);
	FVector2D TouchLocation2 = FVector2D(0.0f, 0.0f);

};
