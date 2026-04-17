// Copyright Fluppisoft, 2020

#pragma once

#include "Player/ViewTargetInterface.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BrickEditorStaticInfo.generated.h"

class UBrickEditorArrowComponent;
class UBrickConnectorsISMComponent;
class UStaticMesh;
class UMaterialInterface;
class UPopupWidget;

/**
 * Used for shared static information of editors
 */
UCLASS(Abstract, Blueprintable)
class BRICKRIGS_API UBrickEditorStaticInfo : public UObject
{
	GENERATED_BODY()

public:
	// ~Properties
	// Size of the usable area
	UPROPERTY(EditAnywhere, Category = Editor)
	FVector EditorBounds;
	// The distance camera and bounds where the bounds starts to fade out, and the distance at which it is fully faded out
	UPROPERTY(EditAnywhere, Category = Editor)
	FFloatInterval BoundsFadeRange;
	// The mesh displayed for the editor bounds
	UPROPERTY(EditAnywhere, Category = Editor)
	UStaticMesh* BoundsMesh;
	// Mesh displayed for the center of mass
	UPROPERTY(EditAnywhere, Category = Editor)
	UStaticMesh* CenterOfMassMesh;
	// The mesh to display for the grid component
	UPROPERTY(EditAnywhere, Category = Editor)
	UStaticMesh* GridMesh;
	// The mesh to display for the movement gizmo
	UPROPERTY(EditAnywhere, Category = Editor)
	UStaticMesh* GizmoMesh;
	// Target size of the gizmo, relative to the screen  size
	UPROPERTY(EditAnywhere, Category = Editor)
	float GizmoSize;
	// The radius within hits are considered to be on the gizmo center
	UPROPERTY(EditAnywhere, Category = Editor)
	float GizmoCenterHitRadius;
	// The axial radius above which hits are considered to be on the rotational arrow
	UPROPERTY(EditAnywhere, Category = Editor)
	float GizmoRotationArrowHitRadius;
	// Radius of the sphere to use for gizmo sweeps
	UPROPERTY(EditAnywhere, Category = Editor)
	float GizmoSweepSphereRadius;
	// Post process material used for the outline effect
	UPROPERTY(EditAnywhere, Category = Editor)
	UMaterialInterface* OutlineMaterial;
	// Distance from the camera at which new objects are placed
	UPROPERTY(EditAnywhere, Category = Editor)
	float NewObjectPlacementDistance;
	// Radius to sweep for obstacles when placing a new brick
	UPROPERTY(EditAnywhere, Category = Editor)
	float NewObjectPlacementSweepRadius;

	// Initial location of the camera, rotation will be calculated so the camera points at the origin
	UPROPERTY(EditAnywhere, Category = Camera)
	FVector InitialCameraLocation;
	// Camera speed params used for the view target interface
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FViewTargetCameraSpeedParams CameraSpeedParams;
	// Zoom params to use for the view target interface
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FViewTargetZoomParams ZoomParams;

	// Speed at which objects are being moved at 100% input
	UPROPERTY(EditAnywhere, Category = Move)
	float MoveSpeed;
	// Multiplied with the distance to the camera and added to the move speed
	UPROPERTY(EditAnywhere, Category = Move)
	float MoveSpeedDistanceScale;
	// Speed at which objects are being rotated
	UPROPERTY(EditAnywhere, Category = Move)
	float RotateSpeed;
	// Speed scale for mouse rotation input
	UPROPERTY(EditAnywhere, Category = Move)
	float MouseRotateSpeed;

	// Max number of undo steps that will be saved and can be reverted
	UPROPERTY(EditAnywhere, Category = Saving)
	int32 MaxUndoSteps;
	// Max size of the undo buffer in bytes
	UPROPERTY(EditAnywhere, Category = Saving)
	int32 MaxUndoBufferSize;
	// Time between auto saves
	UPROPERTY(EditAnywhere, Category = Saving)
	float AutoSaveTime;

	// Arrow component used for object visualizations
	UPROPERTY(EditAnywhere, Category = Visualization)
	TSubclassOf<UBrickEditorArrowComponent> VisualizationArrowComponentClass;
	// Used to visualize brick editor
	UPROPERTY(EditDefaultsOnly, Category = ScalableBrick)
	TSubclassOf<UBrickConnectorsISMComponent> ConnectorsISMComponentClass;
	// Mesh used to visualize point light frustums
	UPROPERTY(EditAnywhere, Category = Visualization)
	UStaticMesh* PointLightFrustumMesh;
	// Mesh used to visualize spot light frustums
	UPROPERTY(EditAnywhere, Category = Visualization)
	UStaticMesh* SpotLightFrustumMesh;
	// Mesh to instance for input channels
	UPROPERTY(EditAnywhere, Category = Visualization)
	UStaticMesh* InputChannelMesh;

	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UPopupWidget> UGCBrowserPopupClass;
	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UPopupWidget> UGCMetaDataPopupClass;
	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UPopupWidget> ReuploadPopupClass;
	// ~Properties

	// ~Constructor
	UBrickEditorStaticInfo();
};
