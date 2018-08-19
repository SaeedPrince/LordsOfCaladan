// Fill out your copyright notice in the Description page of Project Settings.

#include "BeastudioNavLinkProxy.h"

void ABeastudioNavLinkProxy::SetMyLinkData(const FVector& RelativeStart, const FVector& RelativeEnd, ENavLinkDirection::Type Direction)
{
	BeastudioLink = GetSmartLinkComp();
	BeastudioLink->SetLinkData(RelativeStart, RelativeEnd, Direction);
}


