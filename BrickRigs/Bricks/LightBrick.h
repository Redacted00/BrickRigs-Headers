// Copyright Fluppisoft, 2015

#pragma once

#include "Vehicle/SirenSequence.h"
#include "Bricks/Brick.h"
#include "LightBrick.generated.h"

class ULightComponent;

UENUM(BlueprintType)
enum class ELightBrickDirection : uint8
{
	None,
	Omnidirectional,
	X,
	XNeg,
	Y,
	YNeg,
	Z,
	ZNeg,
	Max
};

struct FLightBrickEditorParams : public FBrickEditorParams
{
	// Mesh used to visualize the light frustum
	TBrickEditorComponentPtr<UBrickEditorStaticMeshComponent> FrustumMeshComponent;
};

UCLASS(Abstract)
class BRICKRIGS_API ULightBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Default intensity of the light
	UPROPERTY(EditDefaultsOnly, Category = Light)
	float Intensity;
	// Cached max brightness, gathered from the halo material
	UPROPERTY(VisibleDefaultsOnly, Category = Light)
	float MaxBrightness;
	// Can be used to not restrict the possible directions
	UPROPERTY(EditDefaultsOnly, Category = Light, Meta = (Bitmask, BitmaskEnum = ELightBrickDirection))
	uint16 LightDirectionFlags = 0;
	UPROPERTY(EditDefaultsOnly, Category = Light)
	ELightBrickDirection DefaultLightDirection;
	UPROPERTY(EditDefaultsOnly, Category = Light)
	float DefaultLightConeAngle;
	// ~Properties

	// ~Constructor
	ULightBrickStaticInfo();

	// ~Super Interface
#if WITH_EDITOR
	virtual void BuildCachedData() override;
#endif
	// ~Super Interface
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API ULightBrick : public UBrick
{
	GENERATED_BODY()

private:
	enum ELightPrimitiveData
	{
		LightBrightness = Max,
		LightColor = LightBrightness + 1,
		Max = LightColor + 3
	};

	// ~Variables
	// Whether the light is currently active
	bool bIsLightEnabled;
	// Time seconds at which the light was activated, used for the flash animation
	float ActivationTime;
	// Cached input value from the input channel
	float BrightnessInput;
	// The currently displayed brightness
	float CurrentBrightness;
	// ~Variables

	// ~Components
	// Optional light component
	TBrickEditorComponentPtr<ULocalLightComponent> LightComponent;
	// ~Components

protected:
	// ~Brick Properties
	// Brightness scale for the light
	UPROPERTY(EditDefaultsOnly, Category = Light)
	float Brightness;
	// Sequence to control the flashing of this light
	UPROPERTY(EditDefaultsOnly, Category = Light)
	TSubclassOf<USirenSequence> FlashSequence;
	// Input channel used to activate or deactivate the light
	UPROPERTY(EditDefaultsOnly, Category = Light)
	FVehicleInputChannel InputChannel;
	// Direction which the light should shine to
	UPROPERTY(EditDefaultsOnly, Category = Light)
	ELightBrickDirection LightDirection;
	// Outer cone angle of the spot light in degrees
	UPROPERTY(EditDefaultsOnly, Category = Light)
	float LightConeAngle;
	// ~Brick Properties

public:
	// ~Constructor
	ULightBrick();

	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults() override;
	virtual void PostInitializeBrickEditorObject() override;
	virtual void UninitializeBrickEditorObject() override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params) override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void OnBrickDamageStateChanged(EBrickDamageState PrevState) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual bool IsBrickPatternSupported(const UBrickPattern* InPattern) const override;
	virtual bool IsBrickMaterialSupported(const UBrickMaterial* InMaterial) const override;
	virtual bool ResolveDeprecatedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	virtual void UpdateEditorVisualization() override;

	virtual TUniquePtr<FBrickEditorObjectEditorParams> CreateEditorParams() const override
	{
		return MakeUnique<FLightBrickEditorParams>();
	}

	// ~Super Interface

private:
	struct FGetLightParams
	{
		float Intensity;
		float AttenuationRadius;
		float OuterConeAngle;
		float InnerConeAngle;
		FLinearColor LightColor;
	};

	// Returns parameters associated with the light component
	FGetLightParams GetLightParams() const;
	// Returns the desired light color
	auto GetLightColor() const
	{
		// Get the color as HSV in order to remove the value component
		auto LightColor = BrickColor.ToHSV();
		LightColor.B = 1.f;
		// Convert to RGB
		return LightColor.HSVToLinearRGB();
	}

	// Returns the location and rotation where the light should be attached
	void GetLightRelativeTransform(FVector& OutLocation, FRotator& OutRotation) const;
	// Set the current brightness input, 0-1
	void SetBrightnessInput(float NewBrightness);
	// Activates or deactivates the light during gameplay
	void UpdateIsActive(bool bForceUpdateBrightness = false);
	// Update the brightness of the light
	void UpdateBrightness();
	// Updates the current brightness of the light component
	void UpdatePointLightBrightness();
	// Used for the custom primitive data
	auto GetBrightnessMaterialParameter() const
	{
		return CurrentBrightness * GetStaticInfo<ULightBrickStaticInfo>()->MaxBrightness;
	}

	auto GetLightColorMaterialParameter() const
	{
		return FVector(GetLightColor());
	}

	// Callback for the cvar
	void OnMinLightIntensityCVarChanged(IConsoleVariable* CVar);

	// Property callbacks
	static void GetLightDirectionItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
};
