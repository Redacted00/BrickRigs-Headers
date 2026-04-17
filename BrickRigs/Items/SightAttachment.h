// Copyright Fluppisoft, 2020

#pragma once

#include "Engine/StreamableManager.h"
#include "CoreMinimal.h"
#include "Items/Attachment.h"
#include "SightAttachment.generated.h"

class UMaterialInterface;
class UMaterialInstanceDynamic;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API ASightAttachment : public AAttachment
{
	GENERATED_BODY()

private:
	// ~Variables
	TSharedPtr<FStreamableHandle> StreamableHandle_OcclusionMaterial;
	TSharedPtr<FStreamableHandle> StreamableHandle_ReticleMaterial;
	// Material instance applied to the mesh in order to occlude it
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* OcclusionMID;
	// Cached pointer to the loaded reticle material, used to prevent garbage collection
	UPROPERTY(Transient)
	UMaterialInterface* ReticleMaterial;
	// Cached zoom ratio to skip updates if it didn't change
	float LastZoomRatio;
	// Cached parameters for the MID
	int32 LenseLocationParamIndex;
	int32 LenseForwardVectorParamIndex;
	int32 LenseRightVectorParamIndex;
	int32 OcclusionAlphaParamIndex;
	// ~Variables

public:
	// ~Constructor
	ASightAttachment();

	// ~Super Interface
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	virtual void BecomeViewTarget(APlayerController* PC) override;
	virtual void EndViewTarget(APlayerController* PC) override;
	virtual void GetAimParams(FItemAimParams& OutParams) const override;
	virtual void RemoveFirstPersonOnlyAssets() override;
	// ~Super Interface

	// Update the lense occlusion
	void UpdateLenseOcclusion(float ZoomRatio, bool bForceUpdateMaterials = false);

public:
	// Whether the sight is currently zoomed in
	bool IsZoomedIn() const;
	// Cancel the async loading processes of the sight materials
	void CancelSightMaterialAsyncLoad();
	// To be called when any sight material has been loaded
	void OnSightMaterialLoaded();
	// Callback for the async load of the occlusion material
	void OnOcclusionMaterialLoaded();
	void OnOcclusionMaterialLoaded(UMaterialInterface* LoadedMaterial);
	// Callback for the load of the reticle material
	void OnReticleMaterialLoaded();
	void OnReticleMaterialLoaded(UMaterialInterface* LoadedMaterial);
};
