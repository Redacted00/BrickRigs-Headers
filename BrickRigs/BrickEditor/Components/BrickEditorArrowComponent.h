// Copyright Fluppisoft, 2020

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "BrickEditor/Components/BrickEditorStaticMeshComponent.h"
#include "BrickEditorArrowComponent.generated.h"

class UStaticMesh;
class UMaterialInstanceDynamic;

UENUM(BlueprintType)
enum class EBrickEditorArrowType : uint8
{
	Linear,
	Circular
};

/**
 * Can be used to visualize directions and angles in the brick editor
 */
UCLASS(Abstract, meta = (BlueprintSpawnableComponent))
class BRICKRIGS_API UBrickEditorArrowComponent : public UBrickEditorStaticMeshComponent
{
	GENERATED_BODY()

private:
	// ~Variables
	// Cached pointer to the current UI style
	FBrickUIStyleSafePtr UIStyle;
	// The arrow MID used to change the color and size
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MID;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Arrow)
	UStaticMesh* LinearArrowMesh;
	UPROPERTY(EditDefaultsOnly, Category = Arrow)
	UStaticMesh* CircularArrowMesh;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Arrow)
	EBrickEditorArrowType ArrowType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Arrow)
	float LinearArrowLength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Arrow, meta = (ClampMin = 0.f, ClampMax = 360.f))
	float CircularArrowAngle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Arrow, meta = (ClampMin = 0.f))
	float CircularArrowRadius;
	// The color style of the arrow
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Arrow)
	EBrickUIColorStyle ColorStyle;
	// ~Properties

	// ~Constructor
	UBrickEditorArrowComponent();

	// ~Super Interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	// ~Super Interface

	// Changes the arrow type
	UFUNCTION(BlueprintCallable)
	void SetArrowType(EBrickEditorArrowType NewType);
	// Sets the length of the linear arrow
	UFUNCTION(BlueprintCallable)
	void SetLinearArrowLength(float NewLength);
	// Set the angle that should be displayed
	UFUNCTION(BlueprintCallable)
	void SetCircularArrowAngle(float NewAngle);
	// Set the radius of the circular arrow
	UFUNCTION(BlueprintCallable)
	void SetCircularArrowRadius(float NewRadius);
	// Changes the color style of the arrow
	UFUNCTION(BlueprintCallable)
	void SetColorStyle(EBrickUIColorStyle NewStyle);

private:
	// Updates the displayed mesh
	void UpdateMesh();
	// Updates the MID parameters
	void UpdateMID();
	// Callback for the UI style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
};

struct FBrickEditorArrowComponentParams : public FBrickEditorPrimitiveComponentParams
{
	EBrickEditorArrowType ArrowType;
	float LinearArrowLength;
	float CircularArrowAngle;
	float CircularArrowRadius;
	EBrickUIColorStyle ColorStyle;

	FBrickEditorArrowComponentParams(const UBrickEditorArrowComponent* Default = GetDefault<UBrickEditorArrowComponent>())
		: FBrickEditorPrimitiveComponentParams(Default),
		  ArrowType(Default->ArrowType),
		  LinearArrowLength(Default->LinearArrowLength),
		  CircularArrowAngle(Default->CircularArrowAngle),
		  CircularArrowRadius(Default->CircularArrowRadius)
	{
	}

	void InitializeComponent(UBrickEditorArrowComponent* Comp)
	{
		FBrickEditorPrimitiveComponentParams::InitializeComponent(Comp);

		Comp->ArrowType = ArrowType;
		Comp->LinearArrowLength = LinearArrowLength;
		Comp->CircularArrowAngle = CircularArrowAngle;
		Comp->CircularArrowRadius = CircularArrowRadius;
		Comp->ColorStyle = ColorStyle;
	}
};
