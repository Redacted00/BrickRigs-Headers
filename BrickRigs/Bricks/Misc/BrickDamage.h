#pragma once

#include "CoreMinimal.h"
#include "Misc/ConstExpressions.h"

enum class EBrickDamageState : uint8
{
	None,
	Burnt,
	Damaged,
	Max
};

// Stores brick damage info in a very compact format
struct FBrickDamage
{
private:
	constexpr static auto NumDamageStateBits = 2;
	constexpr static auto DamageStateBitmask = FConstExpressions::GenerateIntWithLeadingOnes<uint8>(NumDamageStateBits);
	constexpr static auto FireBitmask = static_cast<uint8>(1 << NumDamageStateBits);

	// Make sure the damage state can be contained within 2 bits
	static_assert(static_cast<int32>(EBrickDamageState::Max) <= 4);

	// ~Variables
	// The damage info compressed into one byte
	uint8 Data;
	// ~Variables

public:
	// ~Constructor
	explicit FBrickDamage(const uint8 Data = 0)
		: Data(Data)
	{
	}

	explicit operator uint64() const
	{
		return Data;
	}

	bool operator==(const FBrickDamage& Other) const
	{
		return Data == Other.Data;
	}

	bool operator!=(const FBrickDamage& Other) const
	{
		return Data != Other.Data;
	}

	bool IsBurnt() const
	{
		return GetDamageState() == EBrickDamageState::Burnt;
	}

	bool IsDamaged() const
	{
		const auto DamageState = GetDamageState();
		return DamageState == EBrickDamageState::Burnt || DamageState == EBrickDamageState::Damaged;
	}

	EBrickDamageState GetDamageState() const
	{
		return static_cast<EBrickDamageState>(Data & DamageStateBitmask);
	}

	void SetDamageState(EBrickDamageState NewState)
	{
		Data &= ~DamageStateBitmask;
		Data |= static_cast<uint8>(NewState) & DamageStateBitmask;
	}

	bool IsOnFire() const
	{
		return (Data & FireBitmask) != 0;
	}

	void SetIsOnFire(const bool bNewOnFire)
	{
		Data &= ~FireBitmask;
		Data |= bNewOnFire << NumDamageStateBits;
	}
};
