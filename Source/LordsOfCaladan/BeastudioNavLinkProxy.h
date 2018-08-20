// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Navigation/NavLinkProxy.h"
#include "AI/Navigation/NavLinkCustomComponent.h"
#include "BeastudioNavLinkProxy.generated.h"

/**
 * 
 */
UCLASS()
class LORDSOFCALADAN_API ABeastudioNavLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()


public:
	
	ABeastudioNavLinkProxy();

	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	void SetMyLinkData(const FVector& RelativeStart, const FVector& RelativeEnd, ENavLinkDirection::Type Direction);

private:

	UNavLinkCustomComponent* BeastudioLink;
	


};
