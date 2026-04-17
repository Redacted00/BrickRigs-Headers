// Copyright Fluppisoft, 2015

#pragma once

#include "BrickEditor/Components/BrickEditorAudioComponent.h"
#include "Bricks/Brick.h"
#include "SirenBrick.generated.h"

class USeatBrick;
class UAudioComponent;
class USirenType;

UCLASS(Abstract)
class BRICKRIGS_API USirenBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	USirenBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API USirenBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	// Currently active siren sound
	int32 ActiveSirenIndex;
	// Current horn input channel value
	bool bHornInputValue;
	// Audio component used for the siren
	TBrickEditorComponentPtr<UBrickEditorAudioComponent> SirenAC;
	// Audio component used for the horn sound
	TBrickEditorComponentPtr<UBrickEditorAudioComponent> HornAC;
	// ~Variables

protected:
	// ~Brick properties
	UPROPERTY(EditDefaultsOnly, Category = Siren)
	TSubclassOf<USirenType> SirenType;
	UPROPERTY(EditDefaultsOnly, Category = Siren)
	float HornPitch;
	UPROPERTY(EditDefaultsOnly, Category = Siren)
	FVehicleInputChannel InputChannel;
	// ~Brick properties

public:
	// ~Constructor
	USirenBrick();

	// ~Super Interface
	virtual void PostConstructVehicle() override;
	virtual void UninitializeBrickEditorObject() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void OnBrickDamageStateChanged(EBrickDamageState PrevState) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual void UpdateFocusedBrickProperty(const FBrickPropertyFocusEvent& Event) override;
	// ~Super Interface

	// Get the seat controlling the siren
	USeatBrick* GetControllingSeat() const;
	// Get the number of available siren types
	int32 GetNumSirenSounds() const;

private:
	// Sets the currently played siren sound
	void SetActiveSirenIndex(int32 InIndex);
	// Starts or stops the siren sound, depending on whether it could/should be playing
	void UpdateSiren();
	// Starts or stops the horn sound as needed
	void UpdateHorn();
};
