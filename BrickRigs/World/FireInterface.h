// Copyright Fluppisoft, 2016

#pragma once

#include "Damage/DamageType_Fire.h"
#include "UObject/Interface.h"
#include "FireInterface.generated.h"

class IFireInterface;

/**
*
*/
UINTERFACE()
class BRICKRIGS_API UFireInterface : public UInterface
{
	GENERATED_BODY()
};

class IFireInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	// Return whether the object is currently burning
	virtual bool IsOnFire(const FHitResult& Hit) const = 0;
	// Used to ignite the object, called on server and/or client
	virtual bool SetOnFire(const FHitResult& Hit, APawn* DamageInstigator, AActor* DamageCauser) = 0;
	// Used to extinguish the object, called on server and/or client
	virtual bool ExtinguishFire(const FHitResult& Hit) = 0;
};
