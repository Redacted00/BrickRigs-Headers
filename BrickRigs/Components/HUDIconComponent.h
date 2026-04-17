// Copyright Fluppisoft, 2020

#pragma once

#include "Interaction/InteractionOptions.h"
#include "Settings/BrickUserSettingsTypes.h"
#include "Player/PlayerSpawnRequest.h"
#include "UI/HUD/HUDIcons/HUDIconWidget.h"
#include "BrickEditor/Components/BrickEditorComponentInterface.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUDIconComponent.generated.h"

class ABrickPlayerController;

#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
static int32 GShowAllHUDIcons = 0;
#endif

// This struct is used to share HUD icon properties more easily
USTRUCT(BlueprintType)
struct FHUDIconProperties
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UHUDIconWidget> IconWidgetClass;

public:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = EHUDIconDomain))
	uint8 VisibilityFlags;
	// Indicates what HUD visibility is required to display this icon
	UPROPERTY(EditAnywhere)
	EHUDVisibility MaxHUDVisibility;
	// Maximum 3D draw distance, doesn't have an effect on 2D maps
	UPROPERTY(EditAnywhere)
	float MaxDrawDist;

	// Constructor
	FHUDIconProperties()
	{
		MaxHUDVisibility = EHUDVisibility::Minimal;
		MaxDrawDist = 20000.f;
		SetAllVisibilityFlags(true);
	}

	// Helper to set the icon reference from C++
	void SetIconClassPath(const FString& InPath)
	{
		IconWidgetClass = FSoftObjectPath(InPath);
		// IMPORTANT: This has to be called in order for the class to be packaged
		IconWidgetClass.ToSoftObjectPath().PostLoadPath(nullptr);
	}

	const TSoftClassPtr<UHUDIconWidget>& GetIconClass() const
	{
		return IconWidgetClass;
	}

	// Whether an item with these properties could ever be drawn
	bool CanEverBeVisible() const
	{
		return !IconWidgetClass.IsNull() && VisibilityFlags != 0;
	}

	// Set all visibility flags at once
	void SetAllVisibilityFlags(bool bVisible)
	{
		VisibilityFlags = bVisible ? MAX_uint8 : 0;
	}

	// Change the state of a single flag
	void SetVisibilityFlag(EHUDIconDomain InDomain, bool bVisible)
	{
		FFluEnumStatics::BitmaskSetFlag(VisibilityFlags, InDomain, bVisible);
	}
};

/**
* This component can be added to actors that require an icon to be drawn either on the HUD or the map
*/
UCLASS(ClassGroup = (BrickRigs), meta = (BlueprintSpawnableComponent))
class BRICKRIGS_API UHUDIconComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_RetVal_TwoParams(bool, FShouldBeVisible, ABrickPlayerController*, const FHUDIconContext&);
	DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FShouldBeVisibleScript, ABrickPlayerController*, PC, const FHUDIconContext&, Context);

	DECLARE_DELEGATE_RetVal_OneParam(float, FGetMaxDrawDist, ABrickPlayerController*);
	DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(float, FGetMaxDrawDistScript, ABrickPlayerController*, PC);

	DECLARE_DELEGATE_TwoParams(FGetInteractionOptions, ABrickPlayerController*, FInteractionOptions&);
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FGetInteractionOptionsScript, ABrickPlayerController*, PC, FInteractionOptions&, OutOptions);

	DECLARE_DYNAMIC_DELEGATE_RetVal(FVector, FGetIconLocationScript);

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = HUDIcon)
	FHUDIconProperties HUDIconProperties;
	// World coordinates where the icon should appear
	TAttribute<FVector> IconLocation;
	// Whether the line of sight should be tested for this icon
	UPROPERTY(EditDefaultsOnly, Category = HUDIcon)
	bool bTestLineOfSight;
	// ~Properties

	// ~Delegates
	FShouldBeVisible ShouldIconBeVisibleDelegate;
	FGetMaxDrawDist GetMaxDrawDistDelegate;
	FGetInteractionOptions GetInteractionOptionsDelegate;
	// ~Delegates

	// ~Constructor
	UHUDIconComponent();

	// ~Super Interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;
	// ~Super Interface

	// Allows blueprints to create a normal interaction option
	UFUNCTION(BlueprintPure)
	static FInteractionOption MakeInteractionOption(FText DisplayText, bool bIsEnabled, FOnInteraction OnInteraction);
	// Allows blueprints to create an axis interaction option
	UFUNCTION(BlueprintPure)
	static FInteractionOption MakeAxisInteractionOption(FText DisplayText, bool bIsEnabled, FOnInteractionAxis OnInteractionAxis);

	// Whether the icon should ever be shown in the given type of HUD
	bool CanIconEverBeVisible(EHUDIconDomain InDomain) const
	{
		return FFluEnumStatics::BitmaskHasFlag(HUDIconProperties.VisibilityFlags, InDomain);
	}

	// Return whether the icon should currently be shown for the given player
	virtual bool ShouldIconBeVisible(ABrickPlayerController* PC, const FHUDIconContext& Context) const;
	// Allows blueprints to bind the icon visibility delegate
	UFUNCTION(BlueprintCallable)
	void SetShouldIconBeVisibleDelegate(FShouldBeVisibleScript Delegate);

	// Get the maximum draw distance on the HUD
	float GetMaxDrawDist(ABrickPlayerController* PC) const;
	// Allows blueprints to bind the draw distance delegate
	UFUNCTION(BlueprintCallable)
	void SetGetMaxDrawDistDelegate(FGetMaxDrawDistScript Delegate);

	// Get the world location this icon points to
	FVector GetIconLocation() const;
	// Sets the icon location by value
	UFUNCTION(BlueprintCallable)
	void SetIconLocation(const FVector& NewLocation)
	{
		IconLocation = NewLocation;
	}

	// Sets the icon location via a delegate
	UFUNCTION(BlueprintCallable)
	void SetIconLocationDelegate(FGetIconLocationScript Delegate);
	// Return whether this icon represents a spawn point
	bool IsSpawnPoint() const;
	// Return whether the player is allowed to spawn on this spawn point
	bool CanPlayerSpawn(ABrickPlayerController* PC) const;

	// Whether the icon can be interacted with
	bool IsInteractiveIcon() const;
	// Gather all interaction options for the icon
	void GetInteractionOptions(ABrickPlayerController* PC, FInteractionOptions& OutOptions);
	// Allows blueprints to bind the options delegate
	UFUNCTION(BlueprintCallable)
	void SetGetInteractionOptionsDelegate(FGetInteractionOptionsScript Delegate);

private:
	// Makes a spawn request for spawning or checking if the player can spawn
	FPlayerSpawnRequest MakeSpawnRequest() const;

	// Interaction callbacks
	UFUNCTION()
	void Interact_Spawn(ABrickPlayerController* PC);
	UFUNCTION()
	void Interact_PlaceMarker(ABrickPlayerController* PC);
};

struct FBrickEditorHUDIconComponentParams : public FBrickEditorComponentParams
{
	FHUDIconProperties HUDIconProperties;
	TAttribute<FVector> IconLocation;
	bool bTestLineOfSight;
	UHUDIconComponent::FShouldBeVisible ShouldIconBeVisibleDelegate;
	UHUDIconComponent::FGetMaxDrawDist GetMaxDrawDistDelegate;
	UHUDIconComponent::FGetInteractionOptions GetInteractionOptionsDelegate;

	FBrickEditorHUDIconComponentParams(const UHUDIconComponent* Default = GetDefault<UHUDIconComponent>())
		: FBrickEditorComponentParams(Default),
		  HUDIconProperties(Default->HUDIconProperties),
		  bTestLineOfSight(Default->bTestLineOfSight)
	{
	}

	void InitializeComponent(UHUDIconComponent* Comp)
	{
		FBrickEditorComponentParams::InitializeComponent(Comp);

		Comp->HUDIconProperties = HUDIconProperties;
		Comp->IconLocation = IconLocation;
		Comp->bTestLineOfSight = bTestLineOfSight;
		Comp->ShouldIconBeVisibleDelegate = ShouldIconBeVisibleDelegate;
		Comp->GetMaxDrawDistDelegate = GetMaxDrawDistDelegate;
		Comp->GetInteractionOptionsDelegate = GetInteractionOptionsDelegate;
	}
};
