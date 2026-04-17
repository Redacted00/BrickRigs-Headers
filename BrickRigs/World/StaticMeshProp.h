// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticMeshProp.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;

UCLASS(HideCategories = ("Actor Tick", "Rendering", "Replication", "Input", "Actor", "LOD", "Cooking", "Collision"))
class BRICKRIGS_API AStaticMeshProp : public AActor
{
	GENERATED_BODY()

protected:
	// ~Variables
	// This pointer is required so the material will be referenced cooked
	UPROPERTY()
	UMaterialInterface* SelectedMaterial;
	// ~Variables

	// ~Components
	UPROPERTY(Transient, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;
	// ~Components

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Prop)
	UStaticMesh* StaticMesh;
	UPROPERTY(EditDefaultsOnly, Category = Prop)
	float MaxDrawDistance;
	// Whether players should be able to climb this prop
	UPROPERTY(EditDefaultsOnly, Category = Prop)
	bool bIsClimbable;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = Prop)
	TArray<TSoftObjectPtr<UMaterialInterface>> MaterialVariants;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prop)
	FRandomStream RandStream;

public:
	// Visualized manually through detail customization
	UPROPERTY(EditAnywhere, Category = Prop)
	FName SelectedMaterialName;
#endif
	// ~Properties

	// ~Constructor
	AStaticMeshProp();

	// ~Super Interface
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PreRegisterAllComponents() override;
	// ~Super Interface

#if WITH_EDITOR
	// Return the material names that can be selected
	TArray<FName> GetMaterialOptions() const;
	// Get the short version of the material variant
	FString GetShortMaterialName(const TSoftObjectPtr<UMaterialInterface>& Material) const;
#endif
};
