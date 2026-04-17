// Copyright Fluppisoft, 2020

#pragma once

#include "Engine/StreamableManager.h"
#include "Interfaces/IHttpRequest.h"
#include "Brushes/SlateDynamicImageBrush.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BrickThumbnailSubsystem.generated.h"

class USceneCaptureComponent2D;
class UStaticMeshComponent;
class UBrickEditorInterfaceComponent;
class UMaterialInstanceDynamic;
class FPrimitiveComponentId;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnThumbnailRendered, const TSharedPtr<FSlateBrush>&);
using FThumbnailDelegate = FOnThumbnailRendered::FDelegate;

// Describes all parameters needed for a thumbnail
struct FBrickThumbnailRequest
{
	// Local filename or URL of an image to kiad
	FString Path;
	// Whether the path represents a URL
	uint8 bIsURL : 1;
	// Whether the thumbnail should be rendered with a disabled effect
	uint8 bRenderDisabled : 1;

	// Brick editor object class a thumbnail should be rendered for
	FSoftClassPath ObjectClass;

	// The brick material to render, only used if a brick class is set for ObjectClass
	FSoftObjectPath BrickMaterial;

	// A texture asset that should be rendered
	TSoftObjectPtr<UTexture2D> Texture;
	// The color to apply to the texture
	FLinearColor Color;

	// The desired size of the thumbnail
	FIntPoint Size;
	// Optionally add rounded edges to the thumbnail
	float RoundedEdgeRadius;
	// Optional offset of the drop shadow relative to the image size
	FVector2D DropShadowOffset;

	FBrickThumbnailRequest(const FIntPoint& InSize = FIntPoint(0, 0))
		: bIsURL(false), bRenderDisabled(false), Color(FLinearColor::White), Size(InSize), RoundedEdgeRadius(0.f), DropShadowOffset(FVector2D::ZeroVector)
	{
	}

	bool operator==(const FBrickThumbnailRequest& Other) const
	{
		return Path == Other.Path && bIsURL == Other.bIsURL && bRenderDisabled == Other.bRenderDisabled && ObjectClass == Other.ObjectClass && BrickMaterial == Other.BrickMaterial && Texture == Other.Texture && Color == Other.Color && Size == Other.Size;
	}

	bool IsValid() const
	{
		return !Path.IsEmpty() || !ObjectClass.IsNull() || !BrickMaterial.IsNull() || !Texture.IsNull();
	}
};

// ID used to uniquely identify a thumbnail
using FBrickThumbnailID = uint32;

// A handle is used to save a reference to a loaded thumbnail
struct FBrickThumbnailHandle
{
private:
	// The thumbnail ID associated with the handle
	FBrickThumbnailID ID;

public:
	// Handle bound for the completion delegate
	FDelegateHandle DelegateHandle;

	FBrickThumbnailHandle(const FBrickThumbnailID& InID = 0)
		: ID(InID)
	{
	}

	bool IsValid() const
	{
		return ID > 0;
	}

	const FBrickThumbnailID& GetID() const
	{
		return ID;
	}
};

UCLASS()
class BRICKRIGS_API UBrickThumbnailSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	enum class ELoadingState : uint8
	{
		NotStarted,
		InProgress,
		PossibleRender,
		PendingRender,
		Finished,
		Cancelled,
		Reload
	};

	struct FBrickThumbnail : TSharedFromThis<FBrickThumbnail, ESPMode::ThreadSafe>
	{
		// The unique ID
		const FBrickThumbnailID ID;
		// The request associated with this Thumbnail
		const FBrickThumbnailRequest Request;
		// Delegate broadcast when the thumbnail has been loaded
		FOnThumbnailRendered Delegate;
		// The dynamic slate brush containing the loaded image
		TSharedPtr<FSlateBrush> Brush;
		// Http request used to download images
		FHttpRequestPtr HttpRequest;
		// Streamable handle used to load the texture
		TSharedPtr<FStreamableHandle> StreamableHandle_Texture;
		// Number of references to this thumbnail
		uint32 RefCount;
		// Last time the thumbnail has been used
		float LastUsageTime;
		// Current state of the loading process
		ELoadingState LoadingState;

		// ~Constructor
		FBrickThumbnail(const FBrickThumbnailID& InID, const FBrickThumbnailRequest& InRequest)
			: ID(InID), Request(InRequest), RefCount(0), LastUsageTime(0.f), LoadingState(ELoadingState::NotStarted)
		{
		}

		// ~Destructor
		~FBrickThumbnail();

		// Starts the loading process
		void LoadThumbnail();
		// Tries to cancel loading
		void CancelLoading();
		// Invalidates and reloads the thumbnail
		void ReloadThumbnail();
		// Called when the raw image data has been loaded, async
		void OnThumbnailLoadedAsync(const TArray<uint8>* LoadedData);
		// To be called before ProcessThumbnail
		void PreProcessThumbnail(TArray<uint8>& ImageData);
		// Called to process the uncompressed image data or texture
		void ProcessThumbnail(const TArray<uint8>* ResizedImageData, UTexture2D* Texture);

	private:
		// To be called when the thumbnail has finished loading/rendering
		void OnLoadingFinished();
		// HTTP callback
		void HandleHTTPResponse(FHttpRequestPtr InHttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
		// Callback for async loading
		void OnTextureLoaded();
		void OnTextureLoaded(UTexture2D* LoadedTexture);
	};

	struct FCaptureComponentParams
	{
		FVector Location;
		FRotator Rotation;
		FIntPoint Resolution;
		float SuperSampling;
		ESceneCaptureSource CaptureSource;
		ECameraProjectionMode::Type ProjectionType;
		float FOVAngle;
		float OrthoWidth;
		float NearClipPlane;
		bool bUseShowOnlyList;
		TArray<UPrimitiveComponent*> ShowOnlyComponents;
		FEngineShowFlags ShowFlags;
		FPostProcessSettings PostProcessSettings;
		bool bFlipAlpha;

		// ~Constructor
		FCaptureComponentParams(const FVector& InLocation, const FRotator& InRotation, const FIntPoint& InResolution, EShowFlagInitMode ShowFlagMode)
			: Location(InLocation),
			  Rotation(InRotation),
			  Resolution(InResolution),
			  SuperSampling(1.f),
			  CaptureSource(SCS_SceneColorHDR),
			  ProjectionType(ECameraProjectionMode::Perspective),
			  FOVAngle(90.f),
			  OrthoWidth(100.f),
			  NearClipPlane(10.f),
			  bUseShowOnlyList(false),
			  ShowFlags(ShowFlagMode),
			  bFlipAlpha(false)
		{
		}
	};

	// ~Variables
	// A list of all thumbnails already loaded or still pending to be loaded
	TArray<TSharedRef<FBrickThumbnail, ESPMode::ThreadSafe>> Thumbnails;
	// Render target being used to render thumbnails
	UPROPERTY(Transient)
	UTextureRenderTarget2D* ThumbnailRenderTarget;
	// MID used to render textures
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* TextureRenderMID;
	// MID used for inventory item captures
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* InventoryItemMID;
	// Whether we currently have to tick
	bool bNeedsTick;
	// Actor that contains all the capture components
	TWeakObjectPtr<AActor> CaptureActor;
	// ~Variables

	// ~Components
	UPROPERTY(Transient)
	USceneCaptureComponent2D* CaptureComponent;
	UPROPERTY(Transient)
	UStaticMeshComponent* CaptureStaticMeshComponent;
	UPROPERTY(Transient)
	UBrickEditorInterfaceComponent* EditorInterfaceComponent;
	// ~Components

public:
	// Static accessor to the subsystem
	static UBrickThumbnailSubsystem* Get(const UWorld* World)
	{
		return World->GetSubsystem<UBrickThumbnailSubsystem>();
	}

	// ~Super Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		// Disable on dedicated servers
		return !IsRunningDedicatedServer();
	}

	virtual bool IsTickableWhenPaused() const override
	{
		return true;
	}

	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UBrickThumbnailSubsystem, STATGROUP_Tickables);
	}

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	// ~Super Interface

	// Used by the player controller to get all thumbnail render associated components
	void UpdateHiddenComponents(TArray<TWeakObjectPtr<UPrimitiveComponent>>& OutHiddenComponents) const;
	// Requests a thumbnail for a specific object class, delegate is invoked once it is ready
	void RequestThumbnail(FBrickThumbnailHandle& InHandle, const FBrickThumbnailRequest& InRequest, const FThumbnailDelegate& InDelegate);
	// Called to clear the reference to a thumbnail
	void ReleaseThumbnail(FBrickThumbnailHandle& InHandle);
	// Used to Reload thumbnails after they have been overridden for example
	void ReloadThumbnail(const FString& ThumbnailPath);
	// Renders a thumbnail for a UGC item
	void RenderUGCThumbnail(const FString& ThumbnailPath, const FMinimalViewInfo& ViewInfo, const FIntPoint& Resolution, bool bUseShowOnlyList, const TArray<UPrimitiveComponent*>* ShowOnlyComponents = nullptr);

private:
	// Sets up the render target texture
	void InitializeRenderTarget(const FIntPoint& Resolution, bool bForceClear = false);
	// Get the rendered image from the render target
	void GetImageFromRenderTarget(const FIntPoint& Resolution, TArray<uint8>& OutImageData, bool bFlipAlpha = false);
	// Renders a new thumbnail
	void RenderThumbnail(const FCaptureComponentParams& Params, TArray<uint8>& OutImageData);
	// Should be called after a thumbnail has started loading or reloading
	void PostLoadThumbnail(const TSharedRef<FBrickThumbnail, ESPMode::ThreadSafe>& InThumbnail);
	// Renders an object thumbnail
	void RenderObjectThumbnail(const TSharedRef<FBrickThumbnail, ESPMode::ThreadSafe>& InThumbnail);
	// Renders a texture thumbnail
	void RenderTextureThumbnail(const TSharedRef<FBrickThumbnail, ESPMode::ThreadSafe>& InThumbnail);
};
