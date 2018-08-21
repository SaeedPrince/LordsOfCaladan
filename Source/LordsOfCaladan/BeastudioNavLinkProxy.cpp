// Fill out your copyright notice in the Description page of Project Settings.

#include "BeastudioNavLinkProxy.h"

ABeastudioNavLinkProxy::ABeastudioNavLinkProxy()
{
	BeastudioLink = GetSmartLinkComp();
}

void ABeastudioNavLinkProxy::SetMyLinkData(const FVector& RelativeStart, const FVector& RelativeEnd, ENavLinkDirection::Type Direction)
{
	BeastudioLink->SetLinkData(RelativeStart, RelativeEnd, Direction);
}


