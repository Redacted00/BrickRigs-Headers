#pragma once

#include "UGC/UGCTypes.h"
#include "PawnRestartTransform.h"
#include "CoreMinimal.h"
#include "PlayerSpawnRequest.generated.h"

UENUM(BlueprintType)
enum class EPlayerSpawnType : uint8
{
	None,
	OnSpot,
	AtTransform,
	AtSpawnPoint
};

UENUM(BlueprintType)
enum class EPlayerVehicleSpawnType : uint8
{
	Exit,
	Teleport,
	Scrap,
	SpawnNew,
	ReplaceCurrent
};

UENUM(BlueprintType)
enum class EPlayerVehicleSpawnInvincibility : uint8
{
	None,
	Invincible,
	Pinned
};

UENUM(BlueprintType)
enum class EPlayerSpawnResult : uint8
{
	Success,
	Failure,
	UnsavedChanges,
	SentToServer,
	BrickLimitExceeded,
	MassExceeded,
	SizeExceeded,
	VehicleLimitExceeded,
	VehicleNotWhitelisted,
	OnlyAdminsCanSpawn,
	InsufficientFunds,
	NoObjects,
	Cooldown
};

USTRUCT(BlueprintType)
struct FVehicleSpawnProperties
{
	GENERATED_BODY()

	UPROPERTY()
	uint16 NumBricks = 0;
	UPROPERTY()
	FVector Dimensions = FVector::ZeroVector;
	UPROPERTY()
	float Mass = 0.f;
	UPROPERTY()
	float Price = 0.f;

	float GetSize() const
	{
		return Dimensions.GetAbsMax();
	}
};

// This struct is used to pass along spawn info more easily
USTRUCT(BlueprintType)
struct FPlayerSpawnRequest
{
	GENERATED_BODY()

protected:
	// ~Variables
	// The type of this request
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerSpawnType Type;
	// Optional world transform
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPawnRestartTransform SpawnTransform;
	// Optional actor to spawn on
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor> SpawnPoint;
	// Optional vehicle to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUGCFileInfo VehicleFileInfo;
	// Determines whether a vehicle should be spawned or teleported
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerVehicleSpawnType VehicleSpawnType;
	// Determines if the vehicle should be spawned invincible/pinned
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerVehicleSpawnInvincibility VehicleInvincibility;
	// ~Variables

public:
	// ~Constructor
	FPlayerSpawnRequest()
		: Type(EPlayerSpawnType::OnSpot), VehicleSpawnType(EPlayerVehicleSpawnType::Teleport), VehicleInvincibility(EPlayerVehicleSpawnInvincibility::None)
	{
	}

	EPlayerSpawnType GetType() const
	{
		return Type;
	}

	FPlayerSpawnRequest& SetSpawnTransform(UWorld* World, const FVector& InLocation, const FRotator& InRotation)
	{
		Type = EPlayerSpawnType::AtTransform;
		SpawnTransform.Set(World, InLocation, InRotation);
		return *this;
	}

	void GetSpawnTransform(const UWorld* World, FVector& OutLocation, FRotator& OutRotation) const
	{
		OutLocation = SpawnTransform.GetLocation(World);
		OutRotation = SpawnTransform.GetRotation();
	}

	FPlayerSpawnRequest& SetSpawnPoint(AActor* InSpawnPoint)
	{
		Type = EPlayerSpawnType::AtSpawnPoint;
		SpawnPoint = InSpawnPoint;
		return *this;
	}

	AActor* GetSpawnPoint() const
	{
		return SpawnPoint.Get();
	}

	bool ShouldSpawnVehicle() const
	{
		return VehicleSpawnType == EPlayerVehicleSpawnType::SpawnNew || VehicleSpawnType == EPlayerVehicleSpawnType::ReplaceCurrent;
	}

	bool ShouldTeleportVehicle() const
	{
		return VehicleSpawnType == EPlayerVehicleSpawnType::Teleport;
	}

	bool ShouldSpawnOrTeleportVehicle() const
	{
		return ShouldSpawnVehicle() || ShouldTeleportVehicle();
	}

	bool ShouldScrapVehicle() const
	{
		return VehicleSpawnType == EPlayerVehicleSpawnType::Scrap || VehicleSpawnType == EPlayerVehicleSpawnType::ReplaceCurrent;
	}

	// Clears the spawn or teleport request
	void DontSpawnOrTeleportVehicle()
	{
		VehicleSpawnType = EPlayerVehicleSpawnType::Exit;
	}

	// Clears the teleport request
	void DontTeleportVehicle()
	{
		if (VehicleSpawnType == EPlayerVehicleSpawnType::Teleport)
		{
			VehicleSpawnType = EPlayerVehicleSpawnType::Exit;
		}
	}

	// Clears the scrap request
	void DontScrapVehicle()
	{
		if (VehicleSpawnType == EPlayerVehicleSpawnType::Scrap)
		{
			VehicleSpawnType = EPlayerVehicleSpawnType::Exit;
		}
		else if (VehicleSpawnType == EPlayerVehicleSpawnType::ReplaceCurrent)
		{
			VehicleSpawnType = EPlayerVehicleSpawnType::SpawnNew;
		}
	}

	// Optionally sets a vehicle to spawn
	FPlayerSpawnRequest& SetVehicle(const FUGCFileInfo& InVehicleFileInfo, bool bReplaceCurrent)
	{
		VehicleSpawnType = bReplaceCurrent ? EPlayerVehicleSpawnType::ReplaceCurrent : EPlayerVehicleSpawnType::SpawnNew;
		VehicleFileInfo = InVehicleFileInfo;
		return *this;
	}

	FPlayerSpawnRequest& SetVehicleInvincibility(EPlayerVehicleSpawnInvincibility NewInvincibility)
	{
		VehicleInvincibility = NewInvincibility;
		return *this;
	}

	auto GetVehicleInvincibility() const
	{
		return VehicleInvincibility;
	}

	const FUGCFileInfo& GetVehicleInfo() const
	{
		return VehicleFileInfo;
	}

	FUGCFileInfo& GetVehicleInfo()
	{
		return VehicleFileInfo;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bSuccess)
	{
		Ar << Type;

		if (Type == EPlayerSpawnType::AtTransform)
		{
			SpawnTransform.NetSerialize(Ar, Map, bSuccess);
		}
		else if (Type == EPlayerSpawnType::AtSpawnPoint)
		{
			Ar << SpawnPoint;

			// Clear the spawn point reference if it is about to be destroyed
			if (SpawnPoint.IsValid() && SpawnPoint->IsPendingKill())
			{
				SpawnPoint = nullptr;
			}
		}

		Ar << VehicleSpawnType;
		Ar << VehicleInvincibility;

		if (ShouldSpawnVehicle())
		{
			VehicleFileInfo.NetSerialize(Ar, Map, bSuccess);
		}
		else if (Ar.IsLoading())
		{
			VehicleFileInfo = FUGCFileInfo();
		}

		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FPlayerSpawnRequest> : public TStructOpsTypeTraitsBase2<FPlayerSpawnRequest>
{
	enum
	{
		// IMPORTANT: Can't use WithNetSharedSerialization, since the spawn point actor is serialized
		WithNetSerializer = true
	};
};
