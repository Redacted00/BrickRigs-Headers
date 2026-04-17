// Copyright Fluppisoft, 2016

#pragma once

#include "IdlerWheelBrick.h"
#include "TrackWheelBrick.h"
#include "SprocketWheelBrick.generated.h"

class UIdlerWheelBrick;
class UWheelBrick;
class UBrickEditorISMComponent;

UCLASS(Abstract)
class BRICKRIGS_API USprocketWheelBrickStaticInfo : public UTrackWheelBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Track)
	UStaticMesh* TrackMesh;
	// Rest size of the track mesh
	UPROPERTY(EditDefaultsOnly, Category = Track)
	FVector2D TrackMeshSize;
	// Phys material to apply to wheels when connected to the track
	UPROPERTY(EditDefaultsOnly, Category = Track)
	UBrickPhysicalMaterial* TrackPhysMaterial;
	// Sound to play when the track breaks
	UPROPERTY(EditDefaultsOnly, Category = Track)
	USoundBase* TrackBreakSound;
	// Desired distance between individual links
	UPROPERTY(EditDefaultsOnly, Category = Track)
	float TrackSpacing = 30.f;
	UPROPERTY(EditDefaultsOnly, Category = Track)
	float TrackThickness = 10.f;
	UPROPERTY(EditDefaultsOnly, Category = Track)
	int32 MaxNumTrackInstances = 50;
	UPROPERTY(EditDefaultsOnly, Category = Track)
	int32 MaxNumIdlerWheels = 20;
	// How many of the track instances should be spawned as particles
	UPROPERTY(EditDefaultsOnly, Category = Track)
	float TrackBreakParticleRatio = 0.25f;
	// ~Properties

	// ~Constructor
	USprocketWheelBrickStaticInfo();
};

/**
 * Main wheel of a track assembly, contains the track that wraps around this wheel and idler wheels
 */
UCLASS()
class BRICKRIGS_API USprocketWheelBrick : public UTrackWheelBrick
{
	GENERATED_BODY()

	enum class ETrackPrimitiveData : uint8
	{
		Color = 0,
		LengthOffset = Color + 4,
		WidthOffset = LengthOffset + 1,
		InvScale2D = WidthOffset + 1,
		Max = InvScale2D + 2
	};

	enum class EIdlerWheelFlag : uint8
	{
		Flipped,
		Dispensable,
		Max
	};

	// ~Variables
	// Flags for the idler wheels
	TBitArray<> IdlerWheelFlags;
	// Current forward offset along the spline
	float TrackOffset;
	// Average speed of the tracks along all connected wheels
	float AverageTrackSpeed;
	// Number of instances to use for the track
	int32 NumTrackInstances;
	// Current scaling applied to track instances
	FVector2D TrackScale2D;
	// Whether the track is currently valid
	bool bIsTrackValid;
	// ~Variables

	// ~Components
	TBrickEditorComponentPtr<UBrickEditorISMComponent> TrackISMComp;
	// ~Components

protected:
	// ~Brick Properties
	UPROPERTY()
	TArray<FBrickEditorObjectPtr> IdlerWheels;
	UPROPERTY()
	FBrickColorWithAlpha TrackColor;
	// ~Brick Properties

public:
	// ~Constructor
	USprocketWheelBrick();

	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;
	virtual bool IsBrickPropertyMirroredFrom(const UBrickEditorObject* OtherObject, const FBrickPropertyInstance& Property, const EBrickEditorMirrorMode MirrorMode) const override;
	virtual void OnMirrorBrickEditorObject(EBrickEditorMirrorMode MirrorMode) override;
	virtual void UpdateCustomDepth(bool bEnable, uint8 Stencil) override;
	virtual void TickBrick(float DeltaTime) override;
	virtual bool ShouldBrickTick() const override;
	virtual void RepairBrick() override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual USprocketWheelBrick* GetSprocketWheel() const override;
	// ~Super Interface

	// Returns the current speed of the track
	auto GetTrackSpeed() const
	{
		return AverageTrackSpeed;
	}

	// Executes a custom function on each currently connected idler wheel
	void ForEachConnectedIdlerWheel(const TFunction<void(UIdlerWheelBrick*, bool)>& Func);

private:
	// Is the given wheel connected to the sprocket cluster?
	bool IsTrackWheelConnected(const UWheelBrick* Wheel) const;
	// Updates the validity state
	void UpdateTrackValid();
	// Updates the track instances
	void UpdateTrack(const float DeltaTime, const bool bInit = false);
	// Generates the position and rotation curve for the track
	void GenerateTrackSpline(float& SplineLength, FInterpCurveVector& PositionCurve, FInterpCurveQuat& RotationCurve, const bool bInit);
	// Updates the track ISM component
	void UpdateTrackISM(const bool bInit, const float SplineLength, const FInterpCurveVector& PositionCurve, const FInterpCurveQuat& RotationCurve);
	// Plays sound and visual effects after the track has broken
	void SpawnTrackBreakEffect();
	// Returns how much the track should extend/shrink in XY direction
	FVector2D GetTrackSizeOffset() const;
	// Returns the wheel flag for a specific index
	bool GetIdlerWheelFlag(const int32 WheelIdx, const EIdlerWheelFlag Flag) const;
	// Helper to get the index of an idler wheel flag
	static int32 GetIdlerWheelFlagIndex(const int32 WheelIdx, const EIdlerWheelFlag Flag);

	// Sets up custom primitive data for the track component
	template <typename T>
	void AssignTrackCustomPrimitiveData(T Container)
	{
		Container->SetCustomPrimitiveDataVector4(static_cast<int32>(ETrackPrimitiveData::Color), TrackColor.ToLinearRGB());
		const auto SizeOffset = GetTrackSizeOffset();
		Container->SetCustomPrimitiveDataFloat(static_cast<int32>(ETrackPrimitiveData::LengthOffset), SizeOffset.X);
		Container->SetCustomPrimitiveDataFloat(static_cast<int32>(ETrackPrimitiveData::WidthOffset), SizeOffset.Y);
		const auto InvScale2D = FVector2D(1.f) / TrackScale2D;
		Container->SetCustomPrimitiveDataVector2(static_cast<int32>(ETrackPrimitiveData::InvScale2D), InvScale2D);
	}
};
