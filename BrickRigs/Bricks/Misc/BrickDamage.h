#pragma once

#include "CoreMinimal.h"

enum class EBrickDamageState : uint8
{
	None,
	Burnt,
	Damaged,
	Max
};

struct FBrickDamage
{
private:
	// ~Variables
	// The damage info compressed into one byte
	uint8 Data;
	// ~Variables

public:
	// ~Constructor
	FBrickDamage()
		: Data(0)
	{
		check(static_cast<int32>(EBrickDamageState::Max) <= 4);
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
		const EBrickDamageState DamageState = GetDamageState();
		return DamageState == EBrickDamageState::Burnt || DamageState == EBrickDamageState::Damaged;
	}

	EBrickDamageState GetDamageState() const
	{
		return static_cast<EBrickDamageState>(Data & 3);
	}

	void SetDamageState(EBrickDamageState NewState)
	{
		Data &= ~3;
		Data |= static_cast<uint8>(NewState);
	}

	bool IsOnFire() const
	{
		return (Data & (1 << 2)) != 0;
	}

	void SetIsOnFire(bool bNewOnFire)
	{
		Data &= ~(1 << 2);
		Data |= bNewOnFire << 2;
	}

	void SetCompressed(uint8 InData)
	{
		Data = InData;
	}

	uint8 GetCompressed() const
	{
		return Data;
	}
};
