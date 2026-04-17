// Copyright Fluppisoft, 2017

#pragma once

#include "Player/PlayerViewPoint.h"
#include "CharacterDamageInfo.h"
#include "Components/CharacterInventoryComponent.h"
#include "Interaction/InteractionComponent.h"
#include "Actions/CharacterAction.h"
#include "CharacterHealth.h"
#include "CharacterStaticInfo.h"
#include "World/FireInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Player/ViewTargetInterface.h"
#include "Player/BrickPawnInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

// Actors
class ABrickPlayerState;
class ABrickVehicle;
class AFirstAidKit;
// Components
class UBaseCharacterMovementComponent;
class UHUDIconComponent;
class UInteractionComponent;
class UCharacterInventoryComponent;
class UParticleSystemComponent;
class UAudioComponent;
// Bricks
class UBrick;
// Animation
class UCharacterAnimInstance;
// Sound
class USoundWave;
// Materials
class UMaterialInstanceDynamic;
// UI
class UHUDIconComponent;

UCLASS(Abstract)
class BRICKRIGS_API ABaseCharacter : public ACharacter, public IBrickPawnInterface, public IViewTargetInterface, public IGenericTeamAgentInterface, public IFireInterface
{
	GENERATED_BODY()

protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, ABrickPlayerState*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnClientReceivedDamage, const FCharacterDamageInfo&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateOfHealthChanged, ECharacterStateOfHealth);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealingStateChanged, ECharacterHealingState);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRestrictedAreaChanged, bool);

	// ~Variables
	FFluAsyncAssetLoader Loader_BadgeTexture;
	// Used to block aiming and firing while the animation is still blending out from sprinting
	FTimerHandle TimerHandle_SprintTransition;
	// Timer to forcefully cache the ragdoll pose after a certain time
	FTimerHandle TimerHandle_CacheRagdoll;
	// Used to delay damage events on owning clients
	FTimerHandle TimerHandle_DelayDamage;
	// Used to kill the character after entering a restricted area
	FTimerHandle TimerHandle_RestrictedArea;

	// ~Character
	// The current team of the character
	// NOTE: This can't be derived from the player state since the character might be unpossessed
	UPROPERTY(ReplicatedUsing = OnRep_TeamId, Transient)
	FGenericTeamId TeamId;
	UFUNCTION()
	void OnRep_TeamId();
	// Replicated yaw view rotation
	UPROPERTY(Replicated, Transient)
	uint8 RemoteViewYaw;
	// Whether the character is currently aiming
	UPROPERTY(Replicated, Transient)
	uint8 bIsAiming : 1;
	// Whether the character is currently sprinting
	UPROPERTY(Replicated, Transient)
	uint8 bIsSprinting : 1;
	// Whether the character is currently inside a restricted area volume
	uint8 bIsInRestrictedArea : 1;
	// Whether we are currently in the PossessedBy function
	uint8 bIsInPossessedBy : 1;
	// Whether we are currently in the UnPossessed function
	uint8 bIsInUnPossessed : 1;
	// ~Character

	// ~Damage
	// Current health in the range of 0-1
	float Health = 1.f;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RepHealth)
	uint8 RepHealth = MAX_uint8;
	UFUNCTION()
	void OnRep_RepHealth();
	/**
	* Indicates if the character is conscious, unconscious or dead., independent of health.
	* Since the health is compressed we can't rely on it being accurate.
	 */
	UPROPERTY(Transient, ReplicatedUsing = OnStateOfHealthChanged)
	ECharacterStateOfHealth StateOfHealth = ECharacterStateOfHealth::Conscious;
	// Indicates if and how the character is healing
	UPROPERTY(Transient, ReplicatedUsing = OnHealingStateChanged)
	ECharacterHealingState HealingState = ECharacterHealingState::NotHealing;
	// Last bone that was hit before death
	UPROPERTY(Transient, Replicated)
	FName LastHitBone;
	// Damage impulse vector applied to the bone that was last hit
	UPROPERTY(Transient, Replicated)
	FVector_NetQuantize100 LastDamageImpulse = FVector::ZeroVector;
	// Last time damage has been taken
	float LastDamageTime;
	// Last time a collision sound has been played
	float LastCollisionSoundTime;
	// Last frame where a collision has been registered
	uint32 LastCollisionFrame;
	// Whether the character is currently ragdolling
	uint8 bIsRagdoll : 1;
	// Whether the current ragdoll pose is cached (frozen)
	uint8 bIsRagdollCached : 1;
	// Maps bones to the target flinch rotations, consumed by the anim proxy
	TMap<FName, FQuat> BoneFlinchMap;
	// This is an array of damage info structs that are pending to be displayed one by one
	TArray<FCharacterDamageInfo> PendingDamageInfos;
	// ~Damage

	// ~Fire
	// Whether the character is currently on fire
	UPROPERTY(Transient, ReplicatedUsing = OnIsOnFireChanged)
	bool bIsOnFire;
	// The total time the character has been on fire
	float TimeOnFire;
	// The pawn that has set the character on fire
	UPROPERTY(Transient)
	APawn* FireInstigator;
	// ~Fire

protected:
	// ~Actions
	// Array of currently performing actions
	UPROPERTY(Transient)
	TArray<UCharacterAction*> ActiveActions;

private:
	// List of currently executing or finished actions, useful for validating client side action results
	TMap<UClass*, float> PerformedActionsMap;
	// ~Actions

protected:
	// ~Restart
	// The location and rotation we were respawned at or exited the vehicle at, required because in those cases the initial transform is not replicated and replicated movement neither for the owning client
	UPROPERTY(ReplicatedUsing = OnRep_RestartTransform)
	FPawnRestartTransform RestartTransform;
	UFUNCTION()
	void OnRep_RestartTransform();
	// Whether the initial restart after possession has been called yet, used to prevent further calls when exiting vehicles
	uint8 bHadInitialRestart : 1;
	// Whether the owning client has received the restart transform
	uint8 bAcknowledgedRestartTransform : 1;
	// ~Restart

	// ~Material
	// Our optional dynamic material
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MID;
	// Last time the face has been updated
	float LastFaceUpdateTime;
	// Index of the face currently being displayed
	int32 CurrentFaceIndex;
	// Index of the MID parameter used for the face
	int32 CurrentFaceParameterIndex;
	// ~Material
	// ~Variables

	// ~Components
	// Optional inventory component
	UPROPERTY(Transient, BlueprintReadOnly)
	UCharacterInventoryComponent* InventoryComponent;
	// Audio components used for footstep sounds, recycled
	UPROPERTY(Transient)
	TArray<UAudioComponent*> FootstepACs;
	// List of foley audio components used for actions etc.
	UPROPERTY(Transient)
	TArray<UAudioComponent*> FoleyACs;
	// Used to play ragdoll collision sounds
	UPROPERTY(Transient)
	UAudioComponent* CollisionAC;
	// Player icon component
	UPROPERTY(Transient)
	UHUDIconComponent* HUDIconComponent;
	// The interaction component for other characters to interact with us
	UPROPERTY(Transient)
	UInteractionComponent* InteractionComponent;
	UPROPERTY(Transient)
	UParticleSystemComponent* FirePSC;
	UPROPERTY(Transient)
	UAudioComponent* FireAC;
	// ~Components

public:
	// ~Delegates
	FOnPlayerStateChanged OnOwningPlayerStateChangedDelegate;
	FOnClientReceivedDamage OnClientReceivedDamageDelegate;
	FOnHealthChanged OnHealthChangedDelegate;
	FOnStateOfHealthChanged OnStateOfHealthChangedDelegate;
	FOnHealingStateChanged OnHealingStateChangedDelegate;
	FOnRestrictedAreaChanged OnRestrictedAreaChangedDelegate;
	// ~Delegates

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<UCharacterStaticInfo> StaticInfoClass;
	// ~Properties

	// ~Statics
	// Collision channels that are set to overlap only but are supposed to block
	static const TArray<ECollisionChannel> FakeBlockCollisionChannels;
	// Returns the collision channel and response params used by the character
	static void GetCharacterCollisionResponseParams(ECollisionChannel& OutChannel, FCollisionResponseParams& OutResponseParams);
	// Overrides the fake blocked collision channels
	static void OverrideCharacterCollisionResponseContainer(FCollisionResponseContainer& OutContainer);
	// ~Statics

	// ~Constructor
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	virtual void PostRegisterAllComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Reset() override;
	virtual void Restart() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void DisableInput(APlayerController* PlayerController) override;
	virtual void DestroyPlayerInputComponent() override;
	virtual void OnRep_PlayerState() override;
	virtual UInputComponent* CreatePlayerInputComponent() override;
	virtual bool IsLocallyControlled() const override;
	virtual void SetOwner(AActor* NewOwner) override;
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	virtual void PreNetReceive() override;
	virtual void PostNetReceive() override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void OnSubobjectCreatedFromReplication(UObject* NewSubobject) override;
	virtual void OnSubobjectDestroyFromReplication(UObject* Subobject) override;
	virtual void OnRep_ReplicatedMovement() override;
	virtual void OnRep_ReplicatedBasedMovement() override;
	virtual bool ShouldTakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float InternalTakePointDamage(float Damage, const FPointDamageEvent& PointDamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float InternalTakeRadialDamage(float Damage, const FRadialDamageEvent& RadialDamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;
	virtual void LifeSpanExpired() override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime) override;
	virtual void RecalculateBaseEyeHeight() override;
	virtual FVector GetPawnViewLocation() const override;
	virtual FRotator GetViewRotation() const override;
	virtual FRotator GetBaseAimRotation() const override;
	virtual FVector GetTargetLocation(AActor* RequestedBy) const override;
	// ~Super Interface

	// ~Static Info
	// Get our static info CDO, always valid
	const UCharacterStaticInfo* GetStaticInfo() const
	{
#if !UE_BUILD_SHIPPING
		if (!StaticInfoClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("StaticInfo for character class %s is null!"), *GetClass()->GetName());
			return GetDefault<UCharacterStaticInfo>();
		}
#endif

		return StaticInfoClass.GetDefaultObject();
	}

	// Templated version
	template <class T>
	const T* GetStaticInfo() const
	{
#if !UE_BUILD_SHIPPING || !UE_BUILD_TEST
		const auto StaticInfoCDO = GetStaticInfo();
		if (!StaticInfoCDO->IsA<T>())
		{
			UE_LOG(LogTemp, Error, TEXT("Tried to access static info of character class %s as %s, did not match!"), *GetClass()->GetName(), *T::StaticClass()->GetName());
			return GetDefault<T>();
		}
#endif

		return CastChecked<T>(GetStaticInfo());
	}

	// ~Static Info

	// ~Character
	// Public accessor to the inventory component
	UCharacterInventoryComponent* GetInventoryComponent() const;
	// Resets the health, inventory etc., server only
	virtual void ResetCharacter();

protected:
	// Creates the MID if needed, returns true if it can be used
	bool InitializeMID();
	// Called when the team id has been changed
	void OnTeamIdChanged();
	// Updates the appearance of the face
	void TickFace();
	// Get the face that should currently be displayed
	virtual int32 GetDesiredFaceIndex() const;
	// Whether the character should currently be hidden
	virtual bool ShouldHideCharacter() const;
	// Hides or unhides the character as needed
	void UpdateCharacterHidden();

public:
	// Whether the character is currently hidden
	bool IsCharacterHidden() const
	{
		return IsHidden();
	}

	// Whether the character is possessed by a player
	bool IsPossessed() const
	{
		// NOTE: Check the player state rather than the controlled, so this also works on clients
		return GetOwningPlayerState() != nullptr;
	}

	// Whether this character can currently be possessed
	virtual bool CanBePossessed() const
	{
		return !IsPossessed();
	}

	// Returns the linear velocity for cosmetic purposes
	FVector GetVisualVelocity() const;
	// ~Character

	// ~Pawn
	// Destroys and recreates the input component
	void RecreateInputComponent();
	// Called during the first Restart
	virtual void InitialRestart();
	// Resets the player input variables
	virtual void ResetInput();
	// Resets view target related features
	virtual void ResetViewTargetState();
	// ~Pawn

	// ~Vehicle
	// Get the vehicle this character is sitting in
	ABrickVehicle* GetVehicle() const;
	// Returns true if the character is sitting in a vehicle
	bool IsInVehicle() const;
	// ~Vehicle

	// ~View
	// Convert our compressed view rotation to an actual rotation
	FRotator GetRemoteViewRotation() const;

	// Return the maximum allowed pitch rotation delta between the body and the view (head)
	FFloatInterval GetViewPitchRange() const;
	// Return the maximum allowed yaw rotation delta between the body and the view (head)
	FFloatInterval GetViewYawRange() const;
	// Can be used to limit the yaw rotation rate, for example while prone
	bool LimitViewYawRotationRate(float DeltaTime, float Rotation, float RotationRate, float& OutMin, float& OutMax) const;
	// Changes the aiming state
	void SetIsAiming(bool bNewAiming);
	// Whether the player is currently aiming
	bool IsAiming() const
	{
		return bIsAiming;
	}

	// Whether the character is currently allowed to aim
	virtual bool CanAim() const;
	// Helper function to get the aim in and out times
	void GetAimTime(float& OutAimInTime, float& OutAimOutTime) const;

protected:
	// Helper function to play a new camera shake
	void PlayCameraShake(UClass* ShakeClass, float Strength = 1.f);
	// Helper function to get the transform of the camera socket
	FTransform GetCameraSocketTransform() const;
	// Helper function for construction the view point for this character
	FPlayerViewPoint GetPlayerViewPoint() const;
	// Helper function for validating/adjusting the location of the replicated view
	FPlayerViewPoint ValidateClientViewPoint(const FPlayerViewPoint& InViewPoint);
	// ~View

public:
	// ~Movement
	// Get the movement component casted to our custom class
	UBaseCharacterMovementComponent* GetBaseCharacterMovement() const;
	// Whether movement should currently be enabled
	virtual bool ShouldEnableMovement() const;
	// Should be called whenever a parameter in ShouldEnableMovement changes
	void UpdateMovementEnabled();
	// Return whether movement and collision is currently enabled
	bool IsMovementEnabled() const;
	// Updates the collision profile settings of the capsule component
	void UpdateCapsuleCollisionProfile();
	// Returns the collision shape used when standing
	FCollisionShape GetDefaultCapsuleShape() const;
	// Called from character movement whenever a footstep occurs, to spawn footsstep fx, make AI noise etc.
	virtual void SpawnFootstepEffect(const FHitResult& FloorHit, float FootstepVolume);
	// Called from the movement component when this character has overlapped a blocking component
	void OnOverlapHit(bool bForceKill, float ImpactSpeed, AActor* OtherActor);
	// Called from the movement componet to update the sprinting flag
	void SetIsSprinting(bool bNewSprinting);
	// Called when the character has started or stopped sprinting
	void OnIsSprintingChanged();
	// Returns true if the character is currently sprinting
	bool IsSprinting() const
	{
		return bIsSprinting;
	}

	// Return whether the character can sprint in the current state
	bool CanSprint() const;
	// These parameters are used for the animation and for delaying the first shot after sprinting
	void GetSprintTransitionParams(float& OutBlendInTime, float& OutBlendOutTime) const;
	// Modifies the desired spawn transform so the character would be placed above the floor in a valid rotation
	void AdjustSpawnTransform(FTransform& InOutTransform) const;
	// Sets the restart transform and moves the character to it, server only
	virtual void SetRestartTransform(const FVector& InLocation, const FRotator& InRotation);
	// Returns whether the client has received the restart transform yet
	bool HasClientAcknowledgedRestartTransform() const
	{
		return bAcknowledgedRestartTransform;
	}

private:
	// Helper function to apply the new spawn location and rotation
	void ApplyRestartTransform(const FVector& InLocation, const FRotator& InRotation);
	// Lets the server know the owning client has received the restart transform
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAcknowledgeRestartTransform(const FRestartTransformVersion& ComparisonIndex);
	// ~Movement

public:
	// ~Health
	// Get the current health, 0-1
	auto GetHealth() const
	{
		return Health;
	}

	// Get the current state of health
	auto GetStateOfHealth() const
	{
		return StateOfHealth;
	}

	// Get the current healing state
	auto GetHealingState() const
	{
		return HealingState;
	}

	// Return true if this character is dead
	bool IsDead() const
	{
		return GetStateOfHealth() == ECharacterStateOfHealth::Dead;
	}

	// Return true if this character is alive (not dead)
	bool IsAlive() const
	{
		return !IsDead();
	}

	// Whether the character is currently unconscious (but not dead)
	bool IsUnconscious() const
	{
		return GetStateOfHealth() == ECharacterStateOfHealth::Unconscious;
	}

	// Return whether the character is alive and conscious
	// NOTE: BlueprintCallable for the zombie AI tasks
	UFUNCTION(BlueprintPure)
	bool IsConscious() const
	{
		return GetStateOfHealth() == ECharacterStateOfHealth::Conscious;
	}

	// Return the bone flinch map andclear it
	const TMap<FName, FQuat> ConsumeBoneFlinchMap();

protected:
	// Used to notify clients about damage received on the server
	UFUNCTION(Client, Reliable)
	void ClientOnReceivedDamage(const FCharacterDamageInfo& DamageInfo);
	// Called from the timer to process the next damage info
	void ProcessNextDamageInfo();
	// Called when the character has received point or radial damage
	void TakePointOrRadialDamage(float DamageAmount, const FDamageEvent& DamageEvent, const FName& BoneName, const FVector& HitLocation, const FVector& ImpulseDirection, float Scale, AController* EventInstigator);
	// Set the current health, server only
	void SetHealth(const float NewHealth);
	// Called when the 'life' percentage changes
	UFUNCTION()
	virtual void OnHealthChanged();
	// Sets the state of health, server only
	void SetStateOfHealth(ECharacterStateOfHealth NewState);
	// Called when the state of health has changed, server and clients
	UFUNCTION()
	virtual void OnStateOfHealthChanged();
	// Sets the curret healing state, server only
	void SetHealingState(ECharacterHealingState NewState);
	// Called whenever the healing state has changed
	UFUNCTION()
	virtual void OnHealingStateChanged();
	// Checks the point damage factor array on the static info for partly increased or decreased damage
	float GetPointDamageScale(const FHitResult& Hit) const;

public:
	// Force kill the character, server only
	void Kill();
	// Heal the character to full health
	virtual void Heal();
	// Revive a character from death
	void Revive(AController* EventInstigator);

protected:
	// Kill the character, this should only be called when cheating or after taking damage as it won't show impact effects
	virtual void Kill(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	// Sets the on fire flag, server only
	bool SetIsOnFire(bool bNewOnFire, APawn* DamageInstigator = nullptr);
	// Called whenever the fire flag has changed
	UFUNCTION()
	void OnIsOnFireChanged();
	// ~Health

	// ~Ragdoll
	// Starts or stops ragdolling as needed
	void UpdateRagdoll(bool bInCacheRagdoll);
	// Updates the collison profile of the mesh
	void UpdateMeshCollisionProfile();
	// Called periodically to cache the ragdoll if possible
	void OnCacheRagdollTimer();
	// Cached the current ragdoll pose if ragdolling
	void CacheRagdoll();
	// Return true the mesh is in ragdoll mde
	bool IsRagdoll() const
	{
		return bIsRagdoll;
	}

	// Whether the current ragdoll pose is cached
	bool IsRagdollCached() const
	{
		return bIsRagdollCached;
	}

	// Whether the character is ragdolling and the pose isn't cached
	bool IsRagdollSimulating() const
	{
		return bIsRagdoll && !bIsRagdollCached;
	}

	// Set the mesh to simulate physics, can be overridden for vehicles etc.
	void ActivateRagdoll();
	// ~Health
	// ~Ragdoll

	// ~Mesh
	// Gets the anim instance from the mesh
	UAnimInstance* GetAnimInstance() const;
	// Get the anim instance as a character anim instance
	UCharacterAnimInstance* GetCharacterAnimInstance() const;
	// Callbacks for the skeletal mesh component
	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	// Called to initialize the URO params
	void OnMeshAnimUpdateRateParamsCreated(FAnimUpdateRateParameters* Params);
	// Updates the visibility based anim tick option on the mesh
	void UpdateAnimTickOption();
	// Get the anim tick option that should be used on the mesh
	virtual EVisibilityBasedAnimTickOption GetDesiredAnimTickOption() const;
	// Returns the relative location the mesh should currently have
	virtual FVector GetDesiredMeshLocation() const;
	// ~Mesh

public:
	// ~Actions
	// Returns true if the given action has been performed on the server and the effects can be applied
	template <class T>
	bool ValidateClientAction() const
	{
		// Search through performed actions
		for (const auto& Pair : PerformedActionsMap)
		{
			if (Pair.Value >= 0.f)
			{
				const auto Age = GetWorld()->TimeSince(Pair.Value);
				if (Age <= 5.f)
				{
					return true;
				}
			}
		}

		return false;
	}

protected:
	// Should be called on owning clients to start client authoritative actions
	template <class T, class U, typename... P>
	bool PerformClientAction(void (U::*ServerFunction)(const P&...), UClass* ActionClass = T::StaticClass(), const P&... Params)
	{
		if (PerformNewAction<T>(ActionClass, Params..., true))
		{
			// If character is a client, send the action to the server
			if (IsNetMode(NM_Client))
			{
				(static_cast<U*>(this)->*ServerFunction)(Params...);
			}

			return true;
		}

		return false;
	}

	// Perform an action locally on any client
	template <class T, typename... P>
	bool PerformLocalAction(UClass* ActionClass = T::StaticClass(), const P&... Params)
	{
		return PerformNewAction<T>(ActionClass, true, Params...);
	}

	// Performs an action that was replicated
	template <class T, typename... P>
	bool PerformRemoteAction(UClass* ActionClass = T::StaticClass(), const P&... Params)
	{
		return PerformNewAction<T>(ActionClass, false, Params...);
	}

	// Return whether the new action may be started, valid on owning client or server (depending on the action)
	template <class T, typename... P>
	bool CanPerformAction(bool bInvokedLocally, UClass* ActionClass = T::StaticClass(), const P&... Params) const
	{
		if (!ActionClass || IsPendingKill())
		{
			return false;
		}

		check(ActionClass == T::StaticClass() || ActionClass->IsChildOf<T>());

		// If we are not alive and conscious, we cannot start the action
		if (!IsConscious())
		{
			return false;
		}

		T* ActionCDO = ActionClass->GetDefaultObject<T>();

		// Check the authority setting
		switch (ActionCDO->GetActionAuthority())
		{
		case ECharacterActionAuth::ServerAuth:
			if (bInvokedLocally == IsNetMode(NM_Client))
			{
				return false;
			}
			break;
		case ECharacterActionAuth::ClientAuth:
			if (bInvokedLocally != IsLocallyControlled())
			{
				return false;
			}
			break;
		case ECharacterActionAuth::Local:
			if (!bInvokedLocally)
			{
				return false;
			}
		// Allow anybody to start
			break;
		}

		// Setting values on the CDO is a bit hacky, but we need to do this for CanStartAction and CanStartOtherAction
		ActionCDO->SetActionParams(Params...);
		ActionCDO->InitializeAction((ABaseCharacter*)this, bInvokedLocally);

		// Check whether the other actions allow execution
		for (auto OtherAction : ActiveActions)
		{
			if (!OtherAction->CanStartOtherAction(ActionCDO))
			{
				return false;
			}
		}

		// Lastly ask the action if a new instance of it can be started
		return ActionCDO->CanStartAction();
	}

private:
	// Creates and performs a new action
	template <class T, typename... P>
	bool PerformNewAction(UClass* ActionClass, bool bInvokedLocally, const P&... Params)
	{
		if (CanPerformAction<T>(bInvokedLocally, ActionClass, Params...))
		{
			T* NewAction = NewObject<T>(this, ActionClass, NAME_None, RF_Transient);
			check(NewAction);

			// Assign the action parameters
			NewAction->SetActionParams(Params...);

			if (PerformActionInternal(NewAction, bInvokedLocally))
			{
				return true;
			}
		}

		return false;
	}

	// Performs an action that has already been created, does NOT check if the action may actually be performed
	bool PerformActionInternal(UCharacterAction* NewAction, bool bInvokedLocally);

public:
	// Helper function to validate that the given action is being or has been performed on the server
	bool ConsumeClientAction(UClass* ActionClass);
	// Finish a latent action
	void FinishAction(UCharacterAction* Action);
	// Cancel the specified action
	void CancelAction(UCharacterAction* Action);
	// Cancel all active actions
	void CancelAllActions();
	// Return whether the character is already performing an action of the given type
	template <class T = UCharacterAction>
	bool IsPerformingAction(UClass* ActionClass = T::StaticClass()) const
	{
		return ActiveActions.ContainsByPredicate([&](UCharacterAction* Action)
		{
			return Action->GetClass() == ActionClass;
		});
	}

	// Return whether the character is performing any action
	bool IsPerformingAnyAction() const;
	// Return whether any action of the given layer is playing
	bool IsPerformingActionInLayer(ECharacterActionLayer Layer) const;
	// Return the array of active actions
	const TArray<UCharacterAction*>& GetActiveActions() const
	{
		return ActiveActions;
	}

	// Return the first action instance of the given type, or nullptr
	template <class T = UCharacterAction>
	T* GetActionOfClass() const
	{
		for (auto Action : ActiveActions)
		{
			if (Action && Action->IsA<T>())
			{
				return CastChecked<T>(Action);
			}
		}

		return nullptr;
	}

	// Allow actions to play animations on the character
	virtual void PlayActionMontage(UAnimMontage* CharacterMontage, UAnimMontage* ItemMontage, float PlayRate);
	// Stop the montage currently playing for an action
	virtual void StopActionMontage(UAnimMontage* CharacterMontage, UAnimMontage* ItemMontage, float BlendTime);

protected:
	// Called when a new action has been started
	void OnActionStarted(UCharacterAction* Action);
	// Helper function called when an action was finished or cancelled
	void OnActionFinished(UCharacterAction* Action, bool bSuccess);
	// ~Actions

public:
	// ~Melee
	// NOTE: These are BlueprintCallable so it can be called by the zombie AI controller
	UFUNCTION(BlueprintCallable)
	bool PerformMeleeAction();
	UFUNCTION(BlueprintPure)
	bool IsPerformingMeleeAction();
	// Called by the melee action to apply the damage
	void ApplyMeleeDamage(float SweepDistance, float SweepRadius);
	// Called to actually apply damage to the hit
	void ApplyMeleeDamage(const FHitResult& Hit, const FVector& ShotDirection);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPerformMeleeAction();
	// ~Melee

public:
	// ~Sound
	// Play any foley sound on the character, from movement foley to weapon handling etc.
	UAudioComponent* PlayFoleySound(USoundBase* Sound, float Volume = 1.f, float Pitch = 1.f);
	// Fade out or stop a specific foley sound
	void StopFoleySound(UAudioComponent* FoleyAC, float FadeTime = 0.f);
	// ~Sound

protected:
	// ~Interaction
	// Creates the interaction component and marks it as active/inactive if needed
	void UpdateInteractionComponent();
	// Whether the interaction component should be active
	bool ShouldShowInteractionComponent() const;
	// This function returns the interaction options for when somebody else wants to interact with us
	void GetInteractionOptions(ABrickPlayerController* OtherPC, FInteractionOptions& OutOptions);

public:
	// Sets up the possess character interaction option
	FInteractionOption MakePossessOption(ABrickPlayerController* OtherPC);

protected:
	// Get the world location of the interactive point
	FVector GetInteractionLocation() const;
	UFUNCTION()
	void Interact_Possess(ABrickPlayerController* OtherPC);
	// ~Interaction

public:
	// ~Inventory
	// Whether the own inventory can generally be accessed
	bool CanOwnInventoryBeAccessed() const;
	// Callback for the inventory
	bool CanOwnInventoryBeAccessed(ABaseCharacter* OtherCharacter) const;
	// Whether the given inventory is directly or indirectly owned by us
	bool IsOwnedInventory(UInventoryComponent* OtherInventory) const;
	// Whether the player can access their own or another inventory
	bool CanModifyInventory() const;
	// Whether the given item can currently be picked up
	bool CanPickUpItem(AInventoryItem* InItem)
	{
		return PickUpItemInternal(InItem, true);
	}

	// Pick up an item and add it to the inventory, owning client only
	bool PickUpItem(AInventoryItem* InItem)
	{
		return PickUpItemInternal(InItem, false);
	}

protected:
	// Internal version that is shared between CanPickUpItem and PickUpItem
	virtual bool PickUpItemInternal(AInventoryItem* InItem, bool bTestOnly);
	// Constructs the parameters used for picking up items
	FMoveInventoryItemParams MakePickUpItemParams(UInventoryComponent* InInventory, AInventoryItem* InItem, bool bTryToReplace) const;

public:
	// Get the number of items that could actually be moved
	int32 CanMoveInventoryItems(FMoveInventoryItemParams& Params, FInventoryItemRef* OutItemRef = nullptr) const;
	// Move a slot from one inventory to another
	int32 MoveInventoryItems(const FMoveInventoryItemParams& Params);

protected:
	// Internal version that takes the view point
	int32 MoveInventoryItems(FMoveInventoryItemParams Params, const FPlayerViewPoint& ViewPoint);
	// Move items between inventories or drop them, server only
	int32 ForceMoveInventoryItems(const FMoveInventoryItemParams& Params, const FPlayerViewPoint& ViewPoint, float ThrowSpeed = -1.f, bool bPrimeExplosives = false);
	// Verify the validity of inventory input parameters
	bool VerifyInventoryParameter(UInventoryComponent* InInventory) const;
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveInventoryItems(const FMoveInventoryItemParams& Params, const FPlayerViewPoint& ViewPoint);
	// Callbacks for the inventory
	virtual void OnInventorySlotModified(const FInventorySlot& InSlot, EInventorySlotEvent Event);
	// Used to share code between adding and removing
	void OnInventorySotAddedOrRemoved(const FInventorySlot& InSlot, bool bAddedSlot);
	// Can be implemented to spawn the default loadout on the character
	UFUNCTION(Exec, BlueprintImplementableEvent)
	void GiveTestLoadout();

public:
	// Spawns the default team loadout
	void SpawnDefaultLoadout();
	// Returns the first first aid kit slot from the inventory, if there is any
	const FInventorySlot* GetFirstAidKitSlot() const;
	// Returns true if the player has any first aid kit
	bool HasFirstAidKit() const
	{
		return GetFirstAidKitSlot() != nullptr;
	}

	// ~Inventory

	// ~Input
	void SetWantsToSprint(bool bNewSprint);
	bool GetWantsToSprint() const;
	void SetWantsToAim(bool bNewAim);
	bool GetWantsToAim() const;
	// ~Input

protected:
	// ~UI
	// HUD icon component callbacks
	virtual bool ShouldIconBeVisible(ABrickPlayerController* PC, const FHUDIconContext& Context) const;
	float GetIconMaxDrawDist(ABrickPlayerController* PC) const;
	FVector GetIconLocation() const;
	// ~UI

public:
	// ~Restricted Areas
	// These are called from the restricted area volumes
	void OnEnteredRestrictedArea(float TimeUntilDeath);
	void OnLeftRestrictedArea();
	// Get the time remaining until death
	float GetRestrictedAreaTimerRemaining() const;
	// Whether the character is currently in a restricted area
	bool IsInRestrictedArea() const;

private:
	// Sets the flag
	void SetIsInRestrictedArea(bool bInIsInArea);
	// Callback for the kill timer
	void OnRestrictedAreaTimer();
	// ~Restricted Areas

public:
	// ~IBrickPawnInterface
	virtual void OnOwningPlayerStateChanged(ABrickPlayerState* OldPlayerState) override;
	virtual void GetTeamAffiliation(TArray<FGenericTeamId>& OutTeams) const override;
	virtual void GetPawnBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const override;
	virtual FTransform GetPawnRestartTransform() const override;
	virtual void OverridePawnFreeCamPlacement(const FHitResult& Hit, FVector& OutLocation, FRotator& OutRotation) const override;
	virtual FVector GetViewTargetAirVelocity() const override;
	// ~IBrickPawnInterface

	// ~IViewTargetInterface
	virtual bool CanSpectatorControlView() const override
	{
		return false;
	}

	virtual void OnIsViewTargetChanged(bool bNewIsViewTarget) override;
	virtual void GetViewRotationLimit(FRotator& OutMinRotation, FRotator& OutMaxRotation) const override;
	virtual FTransform GetFreeCamFollowTransform() const override;
	virtual void GetCameraViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	// ~IViewTargetInterface

	// ~IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	// ~IGenericTeamAgentInterface

	// ~IFireInterface
	virtual bool IsOnFire(const FHitResult& Hit) const override;
	virtual bool SetOnFire(const FHitResult& Hit, APawn* DamageInstigator, AActor* DamageCauser) override;
	virtual bool ExtinguishFire(const FHitResult& Hit) override;
	// ~IFireInterface
};
