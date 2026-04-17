#pragma once

#include "Engine/StreamableManager.h"
#include "CoreMinimal.h"
#include "ItemAnimation.generated.h"

const static FName BoltSlotName = FName("BoltSlot");

// Assets
class UAnimMontage;
class UAnimSequenceBase;
class USoundBase;

USTRUCT(BlueprintType)
struct FItemAnimation
{
	GENERATED_BODY()

private:
	// ~Variables
	FSimpleMulticastDelegate OnAssetsLoadedDelegate;
	// Number of assets that still have to be loaded
	uint8 NumAssetsPendingLoad = 2;

public:
	// Whether the async load has already been requested
	uint8 bRequestedLoad : 1 = false;
	// Whether all assets have been loaded
	uint8 bLoadComplete : 1 = false;
	// Pointers to the dynamically created montages, needed to reuse them
	UPROPERTY(Transient)
	UAnimMontage* CharacterMontage = nullptr;
	UPROPERTY(Transient)
	UAnimMontage* ItemMontage = nullptr;
	UPROPERTY(Transient)
	USoundBase* FoleySoundPtr = nullptr;
	// ~Variables

	// ~Properties
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAnimSequenceBase> CharacterSequence;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAnimSequenceBase> ItemSequence;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundBase> FoleySound;
	// How long the action is, or how long it can block other actions
	UPROPERTY(EditAnywhere)
	float Length = 0.5f;
	// Speed factor of this action, useful to fine adjust reload times etc.
	UPROPERTY(EditAnywhere)
	float PlayRate = 1.f;
	UPROPERTY(EditAnywhere)
	float BlendInTime = 0.25f;
	UPROPERTY(EditAnywhere)
	float BlendOutTime = 0.25f;
	// ~Properties

	// ~Destructor
	virtual ~FItemAnimation()
	{
	}

	// Return whether the animations have been loaded
	bool AreAssetsReady() const;
	// Call this to load and initialize the assets
	bool LoadAssets(const FStreamableDelegate& Delegate = FStreamableDelegate());

protected:
	// Calls LoadAsset on all soft object paths that have to be loaded
	virtual void LoadAssetsInternal();
	// Load a single asset
	void LoadAsset(const TSoftObjectPtr<UObject>& SoftPtr, const FStreamableDelegate& Delegate);
	// Called after all assets were loaded and are ready
	void OnAllAssetsLoaded();
	// Should always be called on the async loading delegates
	void OnAnyAssetLoaded(UObject* LoadedAsset);
	virtual void OnCharacterSequenceLoaded();
	void OnItemSequenceLoaded();
	void OnFoleySoundLoaded();
	int32 GetMontageLoopCount() const
	{
		// Loop infinitely if the action is also infinite
		return Length <= 0.f ? MAX_int32 : 1;
	}

	// Return the animation slot in which the item montage should take place
	virtual FName GetItemAnimSlot() const;
	// Can be overridden to customize the item anim blend times
	virtual void GetItemBlendTimes(float& OutBlendInTime, float& OutBlendOutTime) const;
};

USTRUCT()
struct FEquipAnimation : public FItemAnimation
{
	GENERATED_BODY()

public:
	FEquipAnimation()
		: FItemAnimation()
	{
		BlendInTime = 0.f;
	}
};

USTRUCT()
struct FUnequipAnimation : public FItemAnimation
{
	GENERATED_BODY()

public:
	FUnequipAnimation()
		: FItemAnimation()
	{
	}

	// ~Super Interface
	virtual void OnCharacterSequenceLoaded() override;
	// ~Super Interface
};

USTRUCT()
struct FFireAnimation : public FItemAnimation
{
	GENERATED_BODY()

	// ~Variables
	UPROPERTY(Transient)
	UAnimMontage* FireEmptyMontage = nullptr;
	// ~Variables

	// ~Properties
	// Optional alternative fire sequence played when the last round in the magazine is fired
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAnimSequenceBase> FireEmptySequence;
	// ~Properties

	// ~Constructor
	FFireAnimation()
		: FItemAnimation()
	{
		BlendInTime = 0.05f;
		BlendOutTime = 0.1f;
	}

	// ~Super Interface
	virtual void LoadAssetsInternal() override;
	virtual void GetItemBlendTimes(float& OutBlendInTime, float& OutBlendOutTime) const override;
	virtual FName GetItemAnimSlot() const override;
	// ~Super Interface

private:
	void OnFireEmptySequenceLoaded();
};

USTRUCT()
struct FCockAnimation : public FItemAnimation
{
	GENERATED_BODY()

	// Time into the animation where the bolt is all the way pulled back
	UPROPERTY(EditAnywhere)
	float PulledTime = 0.25f;
	// Time when the bolt is released and back in the default position
	UPROPERTY(EditAnywhere)
	float ReleasedTime = 0.5f;

	// ~Super Interface
	virtual FName GetItemAnimSlot() const override;
	// ~Super Interface
};

USTRUCT()
struct FReloadAnimation : public FItemAnimation
{
	GENERATED_BODY()

	// Time when the character grabs a new magazine
	UPROPERTY(EditAnywhere)
	float GrabMagTime = 0.25f;
	// Time when the magazine has been inserted again
	UPROPERTY(EditAnywhere)
	float InsertedTime = 0.5f;
};

USTRUCT()
struct FHealAnimation : public FItemAnimation
{
	GENERATED_BODY()
};

USTRUCT()
struct FThrowAnimation : public FItemAnimation
{
	GENERATED_BODY()

	// Anim sequence time at which the explosive leaves the hand
	UPROPERTY(EditAnywhere)
	float ThrowTime = 0.5f;

	// ~Constructor
	FThrowAnimation()
		: FItemAnimation()
	{
		BlendOutTime = 0.f;
	}

	// ~Super Interface
	virtual void GetItemBlendTimes(float& OutBlendInTime, float& OutBlendOutTime) const override;
	// ~Super Interface
};

USTRUCT()
struct FSprayAnimation : public FItemAnimation
{
	GENERATED_BODY()

	// Time at which the paint is applied
	UPROPERTY(EditAnywhere)
	float SprayTime = 0.5f;

	// ~Constructor
	FSprayAnimation()
		: FItemAnimation()
	{
		BlendOutTime = 0.f;
	}
};
