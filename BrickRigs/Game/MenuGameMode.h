// Copyright Fluppisoft, 2016

#pragma once

#include "Settings/MatchSettings.h"
#include "GameFramework/GameModeBase.h"
#include "Misc/BrickAssetManager.h"
#include "Misc/FluAsyncAssetLoader.h"
#include "MenuGameMode.generated.h"

class AMenuSequence;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMenuSequenceCreated, AMenuSequence*);

	// ~Variables
	FFluAsyncAssetLoader AssetLoader_MenuSequence;
	// The menu sequence class that is pending to be created
	UPROPERTY(Transient)
	UClass* MenuSequenceClass;
	// The menu sequence, if it has already been created
	UPROPERTY(Transient)
	AMenuSequence* MenuSequence;
	// The world setup params to use
	FWorldSetupParams WorldSetupParams;
	// Whether the UI intro sequence has been finished yet
	uint8 bIsIntroSequenceFinished : 1;
	// Whether the class has finished loading, also true if it was null
	uint8 bHasMenuSequenceClassBeenLoaded : 1;
	// Whether the menu sequence itself has been created, also true if the class was null
	uint8 bHasMenuSequenceBeenCreated : 1;
	// ~Variables

protected:
	// ~Properties
#if WITH_EDITORONLY_DATA
	// Optional menu sequence to use for testing
	UPROPERTY(EditAnywhere, Category = Menu)
	TSoftClassPtr<AMenuSequence> MenuSequenceOverride;
#endif
	// ~Properties

public:
	// ~Delegates
	FOnMenuSequenceCreated OnMenuSequenceCreatedDelegate;
	// ~Delegates

	// ~Constructor
	AMenuGameMode();

	// ~Super Interface
	virtual void InitGameState() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual bool AllowCheats(APlayerController* P) override;
	// ~Super Interface

	// Called from the window manager to notify about the intro sequence having been loaded
	void OnIntroSequenceFinished();

	auto HasMenuSequenceBeenCreated() const
	{
		return bHasMenuSequenceBeenCreated;
	}

	auto* GetMenuSequence() const
	{
		return MenuSequence;
	}

	const auto& GetWorldSetupParams() const
	{
		return WorldSetupParams;
	}

private:
	// Creates the menu sequence actor if all conditions are met
	void ConditionalCreateMenuSequence();
};
