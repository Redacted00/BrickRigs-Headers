#pragma once

#include "Engine/CollisionProfile.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/MeshComponent.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BrickEditorComponentInterface.generated.h"

class UBrickEditorObject;

using FBrickEditorComponentPtr = TWeakObjectPtr<UActorComponent>;

template <class ComponentType>
struct TBrickEditorComponentPtr
{
	TBrickEditorComponentPtr(const TSharedPtr<FBrickEditorComponentPtr>& InComponentPtr = nullptr)
		: ComponentPtr(InComponentPtr)
	{
	}

	template <class OtherType, typename = typename TEnableIf<TPointerIsConvertibleFromTo<OtherType, ComponentType>::Value>::Type>
	void operator=(const TBrickEditorComponentPtr<OtherType>& Other)
	{
		ComponentPtr = Other.GetComponentPtr();
	}

	bool operator==(ComponentType* OtherComp) const
	{
		return Get() == OtherComp;
	}

	bool operator!=(ComponentType* OtherComp) const
	{
		return Get() == OtherComp;
	}

	bool IsValid() const
	{
		return ComponentPtr.IsValid() && ComponentPtr->IsValid();
	}

	ComponentType* operator->() const
	{
		return GetChecked();
	}

	ComponentType* Get() const
	{
		return IsValid() ? GetChecked() : nullptr;
	}

	ComponentType* GetChecked() const
	{
		check(IsValid());
		return CastChecked<ComponentType>(ComponentPtr->Get());
	}

	const TSharedPtr<FBrickEditorComponentPtr>& GetComponentPtr() const
	{
		return ComponentPtr;
	}

	void Reset()
	{
		ComponentPtr.Reset();
	}

private:
	// ~Variables
	TSharedPtr<FBrickEditorComponentPtr> ComponentPtr;
	// ~Variables
};

// This struct is used when creating new component instances to ensure all variables are being set
struct FBrickEditorComponentParams
{
private:
	UClass* ComponentClass;

public:
	FName Name = NAME_None;
	bool bDeferRegistration;
	bool bAutoActivate;
	bool bNetAddressable;

	FBrickEditorComponentParams(const UActorComponent* Default)
		: ComponentClass(Default->GetClass()),
		bDeferRegistration(false),
		bAutoActivate(Default->bAutoActivate),
		bNetAddressable(false)
	{
	}

	// Used to apply the parameters to a new component
	void InitializeComponent(UActorComponent* Comp)
	{
		Comp->bAutoActivate = bAutoActivate;
		if (bNetAddressable)
		{
			Comp->SetNetAddressable();
		}
	}

	UClass* GetComponentClass() const
	{
		return ComponentClass;
	}
};

struct FBrickEditorSceneComponentParams : FBrickEditorComponentParams
{
	USceneComponent* AttachParent;
	FName AttachSocketName;
	FVector RelativeLocation;
	FRotator RelativeRotation;
	FVector RelativeScale3D;
	EComponentMobility::Type Mobility;
	uint8 bAbsoluteLocation : 1;
	uint8 bAbsoluteRotation : 1;
	uint8 bAbsoluteScale : 1;
	uint8 bVisible : 1;
	uint8 bUseAttachParentBound : 1;

	FBrickEditorSceneComponentParams(const USceneComponent* Default)
		: FBrickEditorComponentParams(Default),
		AttachParent(Default->GetAttachParent()),
		AttachSocketName(Default->GetAttachSocketName()),
		RelativeLocation(Default->GetRelativeLocation()),
		RelativeRotation(Default->GetRelativeRotation()),
		RelativeScale3D(Default->GetRelativeScale3D()),
		Mobility(Default->Mobility),
		bAbsoluteLocation(Default->IsUsingAbsoluteLocation()),
		bAbsoluteRotation(Default->IsUsingAbsoluteRotation()),
		bAbsoluteScale(Default->IsUsingAbsoluteScale()),
		bVisible(Default->GetVisibleFlag()),
		bUseAttachParentBound(Default->bUseAttachParentBound)
	{
	}

	void InitializeComponent(USceneComponent* Comp)
	{
		FBrickEditorComponentParams::InitializeComponent(Comp);

		Comp->SetupAttachment(AttachParent, AttachSocketName);
		Comp->SetRelativeLocation_Direct(RelativeLocation);
		Comp->SetRelativeRotation_Direct(RelativeRotation);
		Comp->SetRelativeScale3D_Direct(RelativeScale3D);
		Comp->SetMobility(Mobility);
		Comp->SetUsingAbsoluteLocation(bAbsoluteLocation);
		Comp->SetUsingAbsoluteRotation(bAbsoluteRotation);
		Comp->SetUsingAbsoluteScale(bAbsoluteScale);
		Comp->SetVisibleFlag(bVisible);
		Comp->bUseAttachParentBound = bUseAttachParentBound;
	}

	// Convenience function used to set the relative location, rotation and scale from a transform
	void SetRelativeTransform(const FTransform& InTransform)
	{
		RelativeLocation = InTransform.GetLocation();
		RelativeRotation = InTransform.Rotator();
		RelativeScale3D = InTransform.GetScale3D();
	}
};

struct FBrickEditorPrimitiveComponentParams : FBrickEditorSceneComponentParams
{
	FName CollisionProfileName;
	ECollisionChannel CollisionObjectTypeOverride;
	TOptional<ECollisionEnabled::Type> CollisionEnabledOverride;
	TMap<ECollisionChannel, ECollisionResponse> CollisionResponseOverrides;
	TEnumAsByte<ECollisionTraceFlag> CollisionTraceFlag;
	FVector BodySetupScale3D;
	int32 CollisionDominanceGroup;
	bool bSimulatePhysics;
	float MassScale;
	FVector InertiaTensorScale;
	bool bUseCCD;
	bool bContactModification;
	bool bAutoWeld;
	float LinearDamping;
	float AngularDamping;
	ESleepFamily SleepFamily;
	float CustomSleepThresholdMultiplier;
	bool bGenerateWakeEvents;
	uint8 PositionSolverIterationCount;
	uint8 VelocitySolverIterationCount;
	UPhysicalMaterial* PhysMaterialOverride;
	bool bNotifyRigidBodyCollision;
	bool bGenerateOverlapEvents;
	bool bUpdateMassWhenScaleChanges;
	bool bCanEverAffectNavigation;
	bool bReplicatePhysicsToAutonomousProxy;
	float MaxDrawDist;
	EDetailMode DetailMode;
	bool bRenderCustomDepth;
	uint8 CustomDepthStencil;
	bool bUseAsOccluder;
	bool CastShadow;
	int32 TranslucencySortPriority;

private:
	TArray<float> CustomPrimitiveData;

public:
	FBrickEditorPrimitiveComponentParams(const UPrimitiveComponent* Default)
		: FBrickEditorSceneComponentParams(Default),
		CollisionProfileName(Default->GetCollisionProfileName()),
		CollisionObjectTypeOverride(ECC_MAX),
		bSimulatePhysics(Default->BodyInstance.bSimulatePhysics),
		MassScale(Default->BodyInstance.MassScale),
		InertiaTensorScale(Default->BodyInstance.InertiaTensorScale),
		bUseCCD(Default->BodyInstance.bUseCCD),
		bContactModification(Default->BodyInstance.bContactModification),
		bAutoWeld(Default->BodyInstance.bAutoWeld),
		LinearDamping(Default->BodyInstance.LinearDamping),
		AngularDamping(Default->BodyInstance.AngularDamping),
		SleepFamily(Default->BodyInstance.SleepFamily),
		CustomSleepThresholdMultiplier(Default->BodyInstance.CustomSleepThresholdMultiplier),
		bGenerateWakeEvents(Default->BodyInstance.bGenerateWakeEvents),
		PositionSolverIterationCount(Default->BodyInstance.PositionSolverIterationCount),
		VelocitySolverIterationCount(Default->BodyInstance.VelocitySolverIterationCount),
		PhysMaterialOverride(nullptr),
		bNotifyRigidBodyCollision(Default->BodyInstance.bNotifyRigidBodyCollision),
		bGenerateOverlapEvents(Default->GetGenerateOverlapEvents()),
		bUpdateMassWhenScaleChanges(Default->BodyInstance.bUpdateMassWhenScaleChanges),
		bCanEverAffectNavigation(Default->CanEverAffectNavigation()),
		bReplicatePhysicsToAutonomousProxy(Default->bReplicatePhysicsToAutonomousProxy),
		MaxDrawDist(Default->LDMaxDrawDistance),
		DetailMode(Default->DetailMode),
		bRenderCustomDepth(Default->bRenderCustomDepth),
		CustomDepthStencil(Default->CustomDepthStencilValue),
		bUseAsOccluder(Default->bUseAsOccluder),
		CastShadow(Default->CastShadow),
		TranslucencySortPriority(Default->TranslucencySortPriority)
	{
	}

	void InitializeComponent(UPrimitiveComponent* Comp)
	{
		FBrickEditorSceneComponentParams::InitializeComponent(Comp);

		Comp->BodyInstance.SetCollisionProfileName(CollisionProfileName);
		if (CollisionEnabledOverride.IsSet())
		{
			Comp->BodyInstance.SetCollisionEnabled(CollisionEnabledOverride.GetValue(), false);
		}
		if (CollisionObjectTypeOverride != ECC_MAX)
		{
			Comp->BodyInstance.SetObjectType(CollisionObjectTypeOverride);
		}
		for (const auto& Pair : CollisionResponseOverrides)
		{
			Comp->BodyInstance.SetResponseToChannel(Pair.Key, Pair.Value);
		}
		Comp->BodyInstance.bSimulatePhysics = bSimulatePhysics;
		Comp->BodyInstance.MassScale = MassScale;
		Comp->BodyInstance.InertiaTensorScale = InertiaTensorScale;
		Comp->BodyInstance.bUseCCD = bUseCCD;
		Comp->BodyInstance.bContactModification = bContactModification;
		Comp->BodyInstance.bAutoWeld = bAutoWeld;
		Comp->BodyInstance.bNotifyRigidBodyCollision = bNotifyRigidBodyCollision;
		Comp->BodyInstance.SleepFamily = SleepFamily;
		Comp->BodyInstance.CustomSleepThresholdMultiplier = CustomSleepThresholdMultiplier;
		Comp->BodyInstance.bGenerateWakeEvents = bGenerateWakeEvents;
		Comp->BodyInstance.PositionSolverIterationCount = PositionSolverIterationCount;
		Comp->BodyInstance.VelocitySolverIterationCount = VelocitySolverIterationCount;
		// NOTE: Sleep and wake delegates are special sparse delegates that we can't bind here
		Comp->OnComponentWake.Clear();
		Comp->OnComponentSleep.Clear();
		Comp->BodyInstance.LinearDamping = LinearDamping;
		Comp->BodyInstance.AngularDamping = AngularDamping;
		Comp->BodyInstance.SetPhysMaterialOverride(PhysMaterialOverride);
		Comp->SetGenerateOverlapEvents(bGenerateOverlapEvents);
		Comp->BodyInstance.bUpdateMassWhenScaleChanges = bUpdateMassWhenScaleChanges;
		Comp->SetCanEverAffectNavigation(bCanEverAffectNavigation);
		Comp->bReplicatePhysicsToAutonomousProxy = bReplicatePhysicsToAutonomousProxy;
		Comp->LDMaxDrawDistance = MaxDrawDist;
		Comp->DetailMode = DetailMode;
		Comp->bRenderCustomDepth = bRenderCustomDepth;
		Comp->CustomDepthStencilValue = CustomDepthStencil;
		Comp->bUseAsOccluder = bUseAsOccluder;
		Comp->CastShadow = CastShadow;
		Comp->TranslucencySortPriority = TranslucencySortPriority;

		const_cast<FCustomPrimitiveData&>(Comp->GetCustomPrimitiveData()).Data = CustomPrimitiveData;

	}

	void InitCustomPrimitiveData(int32 MaxData)
	{
		if (LIKELY(CustomPrimitiveData.Num() < MaxData))
		{
			CustomPrimitiveData.SetNumZeroed(MaxData);
		}
	}

	void CopyCustomPrimitiveData(const TArray<float>& Data)
	{
		CustomPrimitiveData = Data;
	}

	// Used to set the primitive data at the given index
	void SetCustomPrimitiveData(int32 DataIndex, const TArray<float>& Data)
	{
		const auto NeededFloats = DataIndex + Data.Num();
		const auto NumValuesToSet = Data.Num();
		check(NeededFloats <= CustomPrimitiveData.Num());
		FMemory::Memcpy(&CustomPrimitiveData[DataIndex], Data.GetData(), NumValuesToSet * sizeof(float));
	}

	// Float version
	void SetCustomPrimitiveDataFloat(int32 DataIndex, float Data)
	{
		SetCustomPrimitiveData(DataIndex, { Data });
	}

	// Vector 2D version
	void SetCustomPrimitiveDataVector2(int32 DataIndex, const FVector2D& Data)
	{
		SetCustomPrimitiveData(DataIndex, { Data.X, Data.Y });
	}

	// Vector version
	void SetCustomPrimitiveDataVector3(int32 DataIndex, const FVector& Data)
	{
		SetCustomPrimitiveData(DataIndex, { Data.X, Data.Y, Data.Z });
	}

	// Vector 4D version
	void SetCustomPrimitiveDataVector4(int32 DataIndex, const FVector4& Data)
	{
		SetCustomPrimitiveData(DataIndex, { Data.X, Data.Y, Data.Z, Data.W });
	}

	// Color version
	void SetCustomPrimitiveDataColor(int32 DataIndex, const FLinearColor& Data)
	{
		SetCustomPrimitiveData(DataIndex, { Data.R, Data.G, Data.B, Data.A });
	}
};

struct FBrickEditorMeshComponentParams : public FBrickEditorPrimitiveComponentParams
{
	TArray<UMaterialInterface*> OverrideMaterials;

	FBrickEditorMeshComponentParams(const UMeshComponent* Default)
		: FBrickEditorPrimitiveComponentParams(Default)
	{
	}

	void InitializeComponent(UMeshComponent* Comp)
	{
		FBrickEditorPrimitiveComponentParams::InitializeComponent(Comp);

		// NOTE: Just copy over the materials instead of calling SetMaterial, since that could cause an unnecessary update to the physics material
		Comp->OverrideMaterials = OverrideMaterials;
	}

	// Used to set the material overrides more easily
	void SetMaterial(int32 ElementIndex, UMaterialInterface* NewMaterial)
	{
		if (OverrideMaterials.Num() <= ElementIndex)
		{
			OverrideMaterials.SetNum(ElementIndex + 1);
		}

		OverrideMaterials[ElementIndex] = NewMaterial;
	}
};

struct FBrickEditorSplineMeshComponentParams : public FBrickEditorMeshComponentParams
{
	UStaticMesh* StaticMesh;

	FBrickEditorSplineMeshComponentParams(const USplineMeshComponent* Default = GetDefault<USplineMeshComponent>())
		: FBrickEditorMeshComponentParams(Default)
		, StaticMesh(Default->GetStaticMesh())
	{
	}

	void InitializeComponent(USplineMeshComponent* Comp)
	{
		FBrickEditorMeshComponentParams::InitializeComponent(Comp);

		Comp->SetStaticMesh(StaticMesh);
	}
};

struct FBrickEditorLightComponentParams : public FBrickEditorSceneComponentParams
{
	bool bCastShadows;
	float MaxDrawDistance;
	float MaxDistanceFadeRange;
	float SpecularScale;
	float Intensity;
	FColor LightColor;
	UMaterialInterface* LightFunctionMaterial;

	FBrickEditorLightComponentParams(const ULightComponent* Default)
		: FBrickEditorSceneComponentParams(Default),
		bCastShadows(Default->CastShadows),
		MaxDrawDistance(Default->MaxDrawDistance),
		MaxDistanceFadeRange(Default->MaxDistanceFadeRange),
		SpecularScale(Default->SpecularScale),
		Intensity(Default->Intensity),
		LightColor(Default->LightColor),
		LightFunctionMaterial(Default->LightFunctionMaterial)
	{
	}

	void InitializeComponent(ULightComponent* Comp)
	{
		FBrickEditorSceneComponentParams::InitializeComponent(Comp);

		Comp->CastShadows = bCastShadows;
		Comp->MaxDrawDistance = MaxDrawDistance;
		Comp->MaxDistanceFadeRange = MaxDistanceFadeRange;
		Comp->SpecularScale = SpecularScale;
		Comp->Intensity = Intensity;
		Comp->LightColor = LightColor;
		Comp->LightFunctionMaterial = LightFunctionMaterial;
	}
};

struct FBrickEditorLocalLightComponentParams : public FBrickEditorLightComponentParams
{
	float AttenuationRadius;

	FBrickEditorLocalLightComponentParams(const ULocalLightComponent* Default = GetDefault<UPointLightComponent>())
		: FBrickEditorLightComponentParams(Default),
		AttenuationRadius(Default->AttenuationRadius)
	{
	}

	void InitializeComponent(ULocalLightComponent* Comp)
	{
		FBrickEditorLightComponentParams::InitializeComponent(Comp);

		Comp->AttenuationRadius = AttenuationRadius;
	}
};

struct FBrickEditorPointLightComponentParams : public FBrickEditorLocalLightComponentParams
{
	float LightFalloffExponent;
	bool bUseInverseSquaredFalloff;

	FBrickEditorPointLightComponentParams(const UPointLightComponent* Default = GetDefault<UPointLightComponent>())
		: FBrickEditorLocalLightComponentParams(Default),
		LightFalloffExponent(Default->LightFalloffExponent), bUseInverseSquaredFalloff(Default->bUseInverseSquaredFalloff)
	{
	}

	void InitializeComponent(UPointLightComponent* Comp)
	{
		FBrickEditorLocalLightComponentParams::InitializeComponent(Comp);

		Comp->LightFalloffExponent = LightFalloffExponent;
		Comp->bUseInverseSquaredFalloff = bUseInverseSquaredFalloff;
	}
};

struct FBrickEditorSpotLightComponentParams : public FBrickEditorPointLightComponentParams
{
	float InnerConeAngle;
	float OuterConeAngle;

	FBrickEditorSpotLightComponentParams(const USpotLightComponent* Default = GetDefault<USpotLightComponent>())
		: FBrickEditorPointLightComponentParams(Default),
		InnerConeAngle(Default->InnerConeAngle),
		OuterConeAngle(Default->OuterConeAngle)
	{
	}

	void InitializeComponent(USpotLightComponent* Comp)
	{
		FBrickEditorPointLightComponentParams::InitializeComponent(Comp);

		Comp->InnerConeAngle = InnerConeAngle;
		Comp->OuterConeAngle = OuterConeAngle;
	}
};

UINTERFACE()
class BRICKRIGS_API UBrickEditorComponentInterface : public UInterface
{
	GENERATED_BODY()
};

class BRICKRIGS_API IBrickEditorComponentInterface
{
	GENERATED_IINTERFACE_BODY()

protected:
	// ~Variables
	// The object this component belongs to
	TWeakObjectPtr<UBrickEditorObject> BrickEditorObject;
	// ~Variables

public:
	// Sets the owning editor object
	void SetBrickEditorObject(UBrickEditorObject* InObject);

	// Get the outer brick editor object, may be null
	UBrickEditorObject* GetBrickEditorObject() const
	{
		return BrickEditorObject.Get();
	}

	template <class T>
	T* GetBrickEditorObject() const
	{
		return Cast<T>(BrickEditorObject.Get());
	}

	template <class T>
	T* GetBrickEditorObjectChecked() const
	{
		return CastChecked<T>(BrickEditorObject.Get());
	}
};
