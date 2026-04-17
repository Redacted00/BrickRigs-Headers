#pragma once

#include "BrickEditor/BrickEditorObjectReference.h"
#include "Properties/BrickPropertyInterface.h"
#include "VehicleInputAxis.h"
#include "VehicleInputChannel.generated.h"

class UBrick;
class USeatBrick;

struct FInputChannelChangedParams
{
	float Value;
	bool bIsRepairing;

	bool ValueAsBool() const
	{
		return Value > 0.5f;
	}
};

DECLARE_MULTICAST_DELEGATE_OneParam(FInputChannelValueChanged, const FInputChannelChangedParams&);
using FInputChannelDelegate = FInputChannelValueChanged::FDelegate;

USTRUCT(BlueprintType)
struct FVehicleInputChannel : public FBrickStructPropertyInterface
{
	GENERATED_BODY()

	// ~Properties
	UPROPERTY(EditAnywhere)
	EVehicleInputAxis InputAxis;
	// Sources of the input channel, could be seats or bricks with output channels
	UPROPERTY(EditAnywhere)
	TArray<FBrickEditorObjectPtr> SourceBricks;
	// Static value for always on channels
	UPROPERTY(EditAnywhere)
	float Value = 1.f;
	// Index that was used before channels were associated by brick references
	uint8 ChannelIndex_DEPRECATED = 0;
	// ~Properties

	// ~Constructor
	explicit FVehicleInputChannel(const EVehicleInputAxis InAxis = EVehicleInputAxis::None)
		: InputAxis(InAxis)
	{
	}

	// ~Constructor
	explicit FVehicleInputChannel(const EVehicleInputAxis InAxis, const TArray<FBrickEditorObjectPtr>& InSourceBricks)
		: InputAxis(InAxis), SourceBricks(InSourceBricks)
	{
	}

	// ~Super Interface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~Super Interface

	bool IsValidChannel() const
	{
		return InputAxis != EVehicleInputAxis::None;
	}

	bool IsAltAxis() const
	{
		return InputAxis >= EVehicleInputAxis::FirstAltAxis && InputAxis <= EVehicleInputAxis::LastAltAxis;
	}

	// Whether the source bricks property is used for the selected input axis
	bool UsesSourceBricks() const
	{
		return InputAxis != EVehicleInputAxis::None && InputAxis != EVehicleInputAxis::AlwaysOn;
	}

	// Returns the first seat from the source bricks
	USeatBrick* GetSeat() const;

	// Whether the input axis is player input
	bool IsPlayerInput() const
	{
		return InputAxis >= EVehicleInputAxis::FirstAxis && InputAxis <= EVehicleInputAxis::LastPlayerAction;
	}

	// Whether the input is associated to a certain seat
	bool IsSeatInput() const
	{
		// NOTE: Also handle custom channels as seat input, so an authoritative seat can be assigned
		return UsesSourceBricks();
	}

	// Whether the given object can be used as a source for this channel
	EBrickEditorObjectAllowed IsSourceBrickAllowed(const TArray<FBrickEditorObjectPtr>& Selection, const UBrickEditorObject* Object) const;

private:
	// Property callbacks
	static EBrickEditorObjectAllowed IsSourceBrickAllowed(const FBrickEditorObjectAllowedParams& Params);
	static bool GetSourceBricksDefaultValue(const FBrickPropertyContainer& Container, TArray<FBrickEditorObjectPtr>& OutObjects, FDisplayInfo& OutDisplayInfo);
	static int32 GetMaxNumSourceBricks(const FBrickPropertyContainer& Container);
	int32 GetMaxNumSourceBricks() const;
	static void GetInputAxisItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
};

// Basic output channel
USTRUCT(BlueprintType)
struct FVehicleOutputChannel : public FBrickStructPropertyInterface
{
	GENERATED_BODY()

private:
	// ~Variables
	// Current internal value of the output channel
	float CurrentValue = 0.f;
	// ~Variables

public:
	// ~Properties
	// Index that was used before channels were associated by brick references
	uint8 ChannelIndex_DEPRECATED = 0;
	// ~Properties

	// ~Delegates
	// Delegate broadcast when the value has changed
	FInputChannelValueChanged ValueChangedDelegate;
	// ~Delegates

	// ~Super Interface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	// Updates the value
	bool SetValue(float NewValue, bool bIsRepairing)
	{
		if (NewValue != CurrentValue)
		{
			CurrentValue = NewValue;
			ValueChangedDelegate.Broadcast({NewValue, bIsRepairing});
			return true;
		}

		return false;
	}

	float GetValue() const
	{
		return CurrentValue;
	}
};

// Extended output channel that allows adjusting the input and output range
USTRUCT(BlueprintType)
struct FSensorOutputChannel : public FVehicleOutputChannel
{
	GENERATED_BODY()

	// ~Properties
	UPROPERTY(EditAnywhere)
	float MinIn = -1.f;
	UPROPERTY(EditAnywhere)
	float MaxIn = 1.f;
	UPROPERTY(EditAnywhere)
	float MinOut = -1.f;
	UPROPERTY(EditAnywhere)
	float MaxOut = 1.f;
	// ~Properties

	// ~Super Interface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	// Converts the given value according to the channel settings
	float MapValue(float Val) const
	{
		return FMath::GetMappedRangeValueClamped({MinIn, MaxIn}, {MinOut, MaxOut}, Val);
	}
};
