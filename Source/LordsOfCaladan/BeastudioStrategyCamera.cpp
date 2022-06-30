// Fill out your copyright notice in the Description page of Project Settings.

#include "BeastudioStrategyCamera.h"


// Sets default values for this component's properties
UBeastudioStrategyCameraComponent::UBeastudioStrategyCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// initial values
	_InitialValues();
	_ConstructionScript();
}

// Called when the game starts
void UBeastudioStrategyCameraComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UBeastudioStrategyCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_UpdateMovement();
	_UpdateRotation();
	_UpdateZoom();
	_ProcessMouseWindowBorderScroll();
}

void UBeastudioStrategyCameraComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
	_ConstructionScript();
}

// Functions
void UBeastudioStrategyCameraComponent::SetBoundingBoxReference(class ABeastudioBoundingBox* GivenBoundingBox)
{
	BoundingBox = GivenBoundingBox;
	if (BoundingBox != nullptr)
	{
		MovementBoundsLength = BoundingBox->MovementBoundsLength;
		MovementBoundsWidth = BoundingBox->MovementBoundsWidth;
		OffsetTraceZmax = BoundingBox->OffsetTraceZMax;
		OffsetTraceZmin = BoundingBox->OffsetTraceZMin;
		boxOk = true;
		_ConstructionScript();
	}
	else
	{
		boxOk = false;
	}
}

void UBeastudioStrategyCameraComponent::_InitialValues()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;

	// Variables

	MovementBoundsLength = 10000.0f;
	MovementBoundsWidth = 10000.0f;
	OffsetTraceZmax = 5000.0f;
	OffsetTraceZmin = -100.0f;
	TraceChannel = ECC_Camera;
	Offset = 100.0f;
	InitialX = 0.0f;
	InitialY = 0.0f;
	InitialRotation = 0.0f;
	InitialDistance = 5000.0f;
	MinDistance = 1000.0f;
	MaxDistance = 20000.0f;
	Angle = 65.0f;
	ScrollBorderSize = 20.0f;
	MovementSpeed = 2500.0f;
	MaxMovementSpeedMultiplier = 10.0f;
	RotationSpeed = 180.0f;
	ZoomSpeed = 1000.0f;
	Smoothing = 5.0f;

	// private objects

	// Root Component
	this->SetMobility(EComponentMobility::Movable);

	// Static Mesh Component
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(this);
	TargetMesh->SetMobility(EComponentMobility::Movable);
	ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Engine/BasicShapes/Sphere.sphere"));
	if (SphereAsset.Succeeded())
	{
		TargetMesh->SetStaticMesh(SphereAsset.Object);
		TargetMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		TargetMesh->SetWorldScale3D(FVector(1.0f));
		ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Game/Assets/Materials/M_Blue.M_Blue"));
		if (MaterialFinder.Object != NULL)
		{
			UMaterial* MyMaterial = (UMaterial*)MaterialFinder.Object;
			MyMaterial->bUsedWithInstancedStaticMeshes = true;
			UMaterialInstanceDynamic* MyMaterialDynamic = CreateDefaultSubobject<UMaterialInstanceDynamic>(TEXT("MyMaterialDynamic"));
			TargetMesh->SetMaterial(0, MyMaterialDynamic);
			TargetMesh->SetCollisionProfileName(TEXT("NoCollision"));
			TargetMesh->SetVisibility(false);
			if (TargetMesh != nullptr)
			{
				meshOk = true;
			}
		}
	}

	// Spring arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(TargetMesh);
	SpringArm->bDoCollisionTest = false;
	if (SpringArm != nullptr)
	{
		armOk = true;
	}

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	if (Camera != nullptr)
	{
		camOk = true;
	}
}

void UBeastudioStrategyCameraComponent::_ConstructionScript()
{
	float halfOffsetTraceDifference = (OffsetTraceZmax - OffsetTraceZmin) / 2.0f;
	FVector NewLocation = FVector(MovementBoundsLength / 2.0f, MovementBoundsWidth / 2.0f, halfOffsetTraceDifference);
	FVector theVector = GetComponentLocation();
	MinX = theVector.X - MovementBoundsLength / 2;
	MaxX = theVector.X + MovementBoundsLength / 2;
	MinY = theVector.Y - MovementBoundsWidth / 2;
	MaxY = theVector.Y + MovementBoundsWidth / 2;
	TraceStartZ = theVector.Z + OffsetTraceZmax;
	TraceEndZ = theVector.Z + OffsetTraceZmin;
	TargetX = FMath::Clamp(theVector.X + InitialX, MinX, MaxX);
	TargetY = FMath::Clamp(theVector.Y + InitialY, MinY, MaxY);
	TargetZ = TraceEndZ;
	_SetTargetZ();
	TargetMesh->SetWorldLocation(FVector(TargetX, TargetY, TargetZ), false, nullptr);
	TargetRotation = InitialRotation;
	CurrentRotation = InitialRotation;
	TargetMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, TargetRotation));
	SpringArm->SetRelativeRotation(FRotator(0.0f - Angle, 0.0f, 0.0f));
	TargetDistance = FMath::Clamp(InitialDistance, MinDistance, MaxDistance);
	SpringArm->TargetArmLength = TargetDistance;
	MovementSpeedRange = MaxMovementSpeedMultiplier * MovementSpeed - MovementSpeed;
	_SetAdjustedMovementSpeed();
}

void UBeastudioStrategyCameraComponent::_SetTargetZ()
{
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), false, nullptr);
	RV_TraceParams.bTraceComplex = false;
	//RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	FHitResult RV_Hit(ForceInit);
	FVector Start = FVector(TargetX, TargetY, TraceStartZ);
	FVector End = FVector(TargetX, TargetY, TraceEndZ);
	if (GetWorld()->LineTraceSingleByChannel(RV_Hit, Start, End, TraceChannel, RV_TraceParams))
	{
		if (RV_Hit.bBlockingHit)
		{
			TargetZ = RV_Hit.ImpactPoint.Z + Offset;
		}
	}
}

void UBeastudioStrategyCameraComponent::_SetAdjustedMovementSpeed()
{
	AdjustedMovementSpeed = (TargetDistance - MinDistance) / (MaxDistance - MinDistance) * MovementSpeedRange + MovementSpeed;
}

void UBeastudioStrategyCameraComponent::MoveCamera(float X, float Y, bool SetRotation, float Rotation, bool SetDistance, float Distance, bool Teleport)
{
	TargetX = FMath::Clamp(X, MinX, MaxX);
	TargetY = FMath::Clamp(Y, MinY, MaxY);
	_SetTargetZ();
	if (Teleport)
	{
		TargetMesh->SetWorldLocation(FVector(TargetX, TargetY, TargetZ), false, false);
	}
	else
	{
		IsMoving = true;
		WasInputEnabled = IsInputEnabled;
		IsInputEnabled = false;
	}
	if (SetRotation)
	{
		if (Teleport)
		{
			CurrentRotation = Rotation;
			TargetRotation = Rotation;
			TargetMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, Rotation));
		}
		else
		{
			TargetRotation = Rotation;
		}
	}
	if (SetDistance)
	{
		float NewDistance = FMath::Clamp(Distance, MinDistance, MaxDistance);
		if (Teleport)
		{
			SpringArm->TargetArmLength = NewDistance;
		}
		TargetDistance = NewDistance;
		_SetAdjustedMovementSpeed();
	}
}

bool UBeastudioStrategyCameraComponent::IsCameraMoving()
{
	return IsMoving;
}

void UBeastudioStrategyCameraComponent::SetInputEnabled(bool InputEnabled)
{
	IsInputEnabled = InputEnabled;
}

void UBeastudioStrategyCameraComponent::_AddRotation(float Rotation)
{
	if (Rotation != 0.0f)
	{
		TargetRotation = Rotation * RotationSpeed * GetWorld()->GetDeltaSeconds() + TargetRotation;
	}
}

void UBeastudioStrategyCameraComponent::_SetTargetXY(FVector DeltaLocation)
{
	FTransform theTransform = FTransform(TargetMesh->GetComponentRotation(), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
	FVector theLocation = UKismetMathLibrary::TransformLocation(theTransform, DeltaLocation);
	TargetX = FMath::Clamp(TargetX + theLocation.X, MinX, MaxX);
	TargetY = FMath::Clamp(TargetY + theLocation.Y, MinY, MaxY);
}

void UBeastudioStrategyCameraComponent::_AddMovementX(float X)
{
	if (X != 0.0f)
	{
		_SetTargetXY(FVector(X * AdjustedMovementSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 0.0f));
		_SetTargetZ();
	}
}

void UBeastudioStrategyCameraComponent::_AddMovementY(float Y)
{
	if (Y != 0.0f)
	{
		_SetTargetXY(FVector(0.0f, Y * AdjustedMovementSpeed * GetWorld()->GetDeltaSeconds(), 0.0f));
		_SetTargetZ();
	}
}

void UBeastudioStrategyCameraComponent::_UpdateMovement()
{
	FVector Vec1 = TargetMesh->GetComponentLocation();
	FVector Vec2 = FVector(TargetX, TargetY, TargetZ);
	FVector Vec3 = UKismetMathLibrary::VLerp(Vec1, Vec2, FMath::Clamp(GetWorld()->GetDeltaSeconds() * Smoothing, 0.0f, 1.0f));
	bool comparison1 = UKismetMathLibrary::EqualEqual_VectorVector(Vec1, Vec2, 100.0f);
	bool comparison2 = UKismetMathLibrary::EqualEqual_VectorVector(Vec1, Vec2, 0.00001f);
	bool comparison3 = UKismetMathLibrary::EqualEqual_VectorVector(Vec2, Vec3, 0.01f);
	if (IsMoving && comparison1)
	{
		IsMoving = false;
		IsInputEnabled = WasInputEnabled;
	}
	if (!comparison2)
	{
		if (IsMoving)
		{
			if (UKismetMathLibrary::VSize(Vec1 - Vec2) > (AdjustedMovementSpeed * 0.5f))
			{
				TargetMesh->SetWorldLocation(Vec1 + AdjustedMovementSpeed *GetWorld()->GetDeltaSeconds() * 2.0f * UKismetMathLibrary::GetDirectionUnitVector(Vec1, Vec2));
			}
			else
			{
				if (comparison3)
				{
					TargetMesh->SetWorldLocation(Vec2);
				}
				else
				{
					TargetMesh->SetWorldLocation(Vec3);
				}
			}
		}
		else
		{
			if (comparison3)
			{
				TargetMesh->SetWorldLocation(Vec2);
			}
			else
			{
				TargetMesh->SetWorldLocation(Vec3);
			}
		}
	}
}

void UBeastudioStrategyCameraComponent::_UpdateRotation()
{
	float theLerp = UKismetMathLibrary::Lerp(CurrentRotation, TargetRotation, FMath::Clamp(GetWorld()->GetDeltaSeconds() * Smoothing, 0.0f, 1.0f));
	if (CurrentRotation != TargetRotation)
	{
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(TargetRotation, theLerp, 0.01f))
		{
			CurrentRotation = TargetRotation;
			TargetMesh->SetRelativeRotation(FRotator(0.0f, CurrentRotation, 0.0f));
			CurrentRotation = TargetMesh->GetRelativeTransform().GetRotation().Z;
			TargetRotation = CurrentRotation;
		}
		else
		{
			CurrentRotation = theLerp;
			TargetMesh->SetRelativeRotation(FRotator(0.0f, CurrentRotation, 0.0f));
		}

	}
}

void UBeastudioStrategyCameraComponent::_UpdateZoom()
{
	float theLerp = UKismetMathLibrary::Lerp(SpringArm->TargetArmLength, TargetDistance, FMath::Clamp(GetWorld()->GetDeltaSeconds() * Smoothing, 0.0f, 1.0f));
	if (SpringArm->TargetArmLength != TargetDistance)
	{
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(TargetDistance, theLerp, 0.01f))
		{
			SpringArm->TargetArmLength = TargetDistance;
		}
		else
		{
			SpringArm->TargetArmLength = theLerp;
		}

	}
}

void UBeastudioStrategyCameraComponent::_ProcessMouseWindowBorderScroll()
{
	APlayerController* controllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	float MouseX;
	float MouseY;
	int32 ScreenX;
	int32 ScreenY;
	controllerRef->GetViewportSize(ScreenX, ScreenY);
	if (controllerRef->GetMousePosition(MouseX, MouseY))
	{
		if (IsInputEnabled)
		{
			if (MouseX < ScrollBorderSize)
			{
				_AddMovementY(-1.0f);
			}
			else
			{
				if (MouseX >(float) ScreenX - ScrollBorderSize)
				{
					_AddMovementY(1.0f);
				}
			}
			if (MouseY < ScrollBorderSize)
			{
				_AddMovementX(1.0f);
			}
			else
			{
				if (MouseY >(float) ScreenY - ScrollBorderSize)
				{
					_AddMovementX(-1.0f);
				}
			}
		}
	}
}


void UBeastudioStrategyCameraComponent::CameraMoveForward(float Value)
{
	if (IsInputEnabled)
	{
		_AddMovementX(Value);
	}
}

void UBeastudioStrategyCameraComponent::CameraMoveRight(float Value)
{
	if (IsInputEnabled)
	{
		_AddMovementY(Value);
	}
}

void UBeastudioStrategyCameraComponent::CameraRotate(float Value)
{
	if (IsInputEnabled)
	{
		_AddRotation(Value);
	}
}

void UBeastudioStrategyCameraComponent::CameraZoomIn()
{
	if (IsInputEnabled)
	{
		TargetDistance = FMath::Clamp(TargetDistance - ZoomSpeed, MinDistance, MaxDistance);
		_SetAdjustedMovementSpeed();
	}
}

void UBeastudioStrategyCameraComponent::CameraZoomOut()
{
	if (IsInputEnabled)
	{
		TargetDistance = FMath::Clamp(TargetDistance + ZoomSpeed, MinDistance, MaxDistance);
		_SetAdjustedMovementSpeed();
	}
}

void UBeastudioStrategyCameraComponent::CameraResetRotation()
{
	TargetRotation = 0.0f;
}
