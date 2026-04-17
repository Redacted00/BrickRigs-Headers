#pragma once

#include "GenericTeamAgentInterface.h"
#include "CoreMinimal.h"

namespace FSerializationHelper
{
	FORCEINLINE void SerializeFloatCompressed(FArchive& Ar, float& Value, float MinValue, float MaxValue, int32 NumBits, bool bRoundUp = true)
	{
		ensureMsgf(Value >= MinValue && Value <= MaxValue, TEXT("SerializeFloatCompressed: Tried to serialize a value outside of the range!"));

		// NOTE: Remove two so we have an accurate mid point, so we just use a range of 0-254 instead of 0-255 for example
		const auto MaxCompressedValue = static_cast<uint32>((1 << NumBits) - 2);
		if (Ar.IsLoading())
		{
			auto CompressedValue = static_cast<uint32>(0);
			Ar.SerializeBits(&CompressedValue, NumBits);
			Value = FMath::Clamp(CompressedValue / static_cast<float>(MaxCompressedValue), 0.f, 1.f) * (MaxValue - MinValue) + MinValue;
		}
		else
		{
			// Use the ceil, so the minimum value is always accurate
			// This is important for the fuel level replication for example, so when there is any fuel left the clients know it
			const auto ClampedValue = FMath::Clamp((Value - MinValue) / (MaxValue - MinValue), 0.f, 1.f) * MaxCompressedValue;
			auto CompressedValue = static_cast<uint32>(bRoundUp ? FMath::CeilToInt(ClampedValue) : FMath::FloorToInt(ClampedValue));
			//checkf(CompressedValue >= 0 && CompressedValue <= MaxCompressedValue, TEXT("SerializeFloatCompressed: CompressedValue was out of range!"));
			Ar.SerializeBits(&CompressedValue, NumBits);
		}
	}

	FORCEINLINE void SerializeTeamId(FArchive& Ar, FGenericTeamId& TeamId)
	{
		uint8 TeamByte;
		if (Ar.IsLoading())
		{
			Ar << TeamByte;
			TeamId = TeamByte;
		}
		else
		{
			TeamByte = TeamId.GetId();
			Ar << TeamByte;
		}
	}

	// This private function is used to handle all string serialization
	template <typename CounterType, bool bCanContainUnicode>
	FORCEINLINE void SerializeString(FArchive& Ar, FString& String)
	{
		// NOTE: This is mostly copied from String.cpp: FArchive& operator<<( FArchive& Ar, FString& A )

		if (Ar.IsLoading())
		{
			CounterType SavedNum;
			Ar << SavedNum;
			const bool bUseUnicode = bCanContainUnicode && SavedNum < 0;
			SavedNum = FMath::Abs(SavedNum);

			// Empty the previous value and make space for the new one, also include the null terminator
			const int32 NumChars = SavedNum > 0 ? SavedNum + 1 : 0;
			String.GetCharArray().SetNumUninitialized(NumChars);

			if (NumChars)
			{
				if (bUseUnicode)
				{
					// Read in the unicode string
					const auto Passthru = StringMemoryPassthru<UCS2CHAR>(String.GetCharArray().GetData(), NumChars, NumChars);
					Ar.Serialize(Passthru.Get(), SavedNum * sizeof(UCS2CHAR));
					// Add the null terminator
					Passthru.Get()[NumChars - 1] = '\0';
					Passthru.Apply();

					// Inline combine any surrogate pairs in the data when loading into a UTF-32 string
					StringConv::InlineCombineSurrogates(String);

					// Since Microsoft's vsnwprintf implementation raises an invalid parameter warning
					// with a character of 0xffff, scan for it and terminate the string there.
					// 0xffff isn't an actual Unicode character anyway.
					auto Index = 0;
					if (String.FindChar(0xffff, Index))
					{
						String[Index] = '\0';
						String.TrimToNullTerminator();
					}
				}
				else
				{
					auto Passthru = StringMemoryPassthru<ANSICHAR>(String.GetCharArray().GetData(), NumChars, NumChars);
					Ar.Serialize(Passthru.Get(), SavedNum * sizeof(ANSICHAR));
					// Add the null terminator
					Passthru.Get()[NumChars - 1] = '\0';
					Passthru.Apply();
				}
			}
		}
		else
		{
			check(bCanContainUnicode || FCString::IsPureAnsi(*String));
			if (bCanContainUnicode && !FCString::IsPureAnsi(*String))
			{
				// NOTE: Exclude the null terminator
				const auto UTF16String = FTCHARToUTF16(*String, String.Len());
				// Store unicode strings as a negative number
				auto SavedNum = static_cast<CounterType>(-UTF16String.Length());
				Ar << SavedNum;
				SavedNum = FMath::Abs(SavedNum);

				if (SavedNum)
				{
					Ar.Serialize(const_cast<void*>(reinterpret_cast<const void*>(UTF16String.Get())), sizeof(UTF16CHAR) * SavedNum);
				}
			}
			else
			{
				auto SavedNum = static_cast<CounterType>(String.Len());
				Ar << SavedNum;

				if (SavedNum)
				{
					Ar.Serialize(const_cast<void*>(reinterpret_cast<const void*>(StringCast<ANSICHAR>(*String, SavedNum).Get())), sizeof(ANSICHAR) * SavedNum);
				}
			}
		}
	}

	// Serializes a CPP or object name, without unicode characters
	FORCEINLINE void SerializeName(FArchive& Ar, FString& String)
	{
		SerializeString<int8, false>(Ar, String);
	}

	// Serializes user entered text that could potentially include unicode characters
	FORCEINLINE void SerializeUserString(FArchive& Ar, FString& String)
	{
		SerializeString<int16, true>(Ar, String);
	}

	FORCEINLINE void LoadLegacyString(FArchive& Ar, FString& String)
	{
		Ar << String;
	}

	FORCEINLINE void SaveObjectByName(FArchive& Ar, FString ObjectName, bool bMayContainClass = false)
	{
		if (bMayContainClass)
		{
			// Remove the blueprint class prefix and suffix
			ObjectName.RemoveFromStart(TEXT("BP_"), ESearchCase::CaseSensitive);
			ObjectName.RemoveFromEnd(TEXT("_C"), ESearchCase::CaseSensitive);
		}

		SerializeName(Ar, ObjectName);
	}

	FORCEINLINE void SaveObject(FArchive& Ar, const UObject* Object, const bool bMayContainClass = false)
	{
		SaveObjectByName(Ar, Object ? Object->GetName() : FString(), bMayContainClass);
	}

	FORCEINLINE void LoadObjectName(FArchive& Ar, FString& OutName)
	{
		SerializeName(Ar, OutName);
	}

	template <typename T>
	FORCEINLINE T* LoadObjectFromName(FString ObjectName, const TArray<T*>& Objects, bool bMayContainClass = false)
	{
		if (!ObjectName.IsEmpty())
		{
			// Remove the class prefix from the loaded name
			if (bMayContainClass)
			{
				ObjectName.RemoveFromStart(TEXT("BP_"));
			}

			for (T* Object : Objects)
			{
				if (Object)
				{
					FString OtherName = Object->GetName();

					// Remove the class prefix from the other class name
					if (bMayContainClass)
					{
						OtherName.RemoveFromStart(TEXT("BP_"));
					}

					if (OtherName.StartsWith(ObjectName, ESearchCase::CaseSensitive))
					{
						// Since the generated class suffix isn't saved with the name, optionally append it here
						if (OtherName.Len() == ObjectName.Len() || bMayContainClass && OtherName.RightChop(ObjectName.Len()) == TEXT("_C"))
						{
							return Object;
						}
					}
				}
			}

			//UE_LOG(LogTemp, Warning, TEXT("FSerializationHelpers::LoadObject: Failed to load object %s"), *ObjectName);
		}

		return nullptr;
	}

	template <typename T>
	FORCEINLINE T* LoadObject(FArchive& Ar, const TArray<T*>& Objects, bool bMayContainClass = false)
	{
		FString ObjectName;
		LoadObjectName(Ar, ObjectName);

		return LoadObjectFromName(ObjectName, Objects, bMayContainClass);
	}

	FORCEINLINE void SaveClass(FArchive& Ar, const UClass* Class)
	{
		SaveObject(Ar, Class, true);
	}

	FORCEINLINE UClass* LoadClass(FArchive& Ar, const TArray<UClass*>& Classes)
	{
		return LoadObject(Ar, Classes, true);
	}

	FORCEINLINE UClass* LoadClassFromName(const FString& ClassName, const TArray<UClass*>& Classes)
	{
		return LoadObjectFromName(ClassName, Classes, true);
	}

	// Serializes an arbitrary array with a custom counter type
	template <typename CounterType, typename ElementType>
	FORCEINLINE bool NetSerializeArray(FArchive& Ar, UPackageMap* Map, bool& bSuccess, TArray<ElementType>& Array, const CounterType MaxNum = -1)
	{
		ensure(MaxNum < 0 || Array.Num() < MaxNum);

		auto Num = static_cast<CounterType>(Array.Num());
		Ar << Num;

		// Abort if the array isn't larger than the user defined max
		if (MaxNum >= 0 && Num > MaxNum)
		{
			if (Ar.IsLoading())
			{
				Array.Empty();
			}

			return false;
		}

		// If we don't need to perform per-item serialization, just read it in bulk
		if (sizeof(ElementType) == 1 || TCanBulkSerialize<ElementType>::Value)
		{
			Array.SetNumUninitialized(Num);
			Ar.Serialize(Array.GetData(), Array.Num() * sizeof(ElementType));
		}
		else
		{
			// Net serialize elements individually
			Array.SetNum(Num);
			for (auto i = 0; i < Array.Num(); i++)
			{
				Array[i].NetSerialize(Ar, Map, bSuccess);
			}
		}

		// Abort if there was an error while loading the elements
		if (Ar.IsLoading() && Ar.IsError())
		{
			Array.Empty();
			return false;
		}

		return !Ar.IsError();
	}
};
