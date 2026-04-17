#pragma once

#include "CoreMinimal.h"
#include "BrickColor.generated.h"

USTRUCT(BlueprintType)
struct FBrickColor
{
	GENERATED_BODY()
	;

protected:
	// ~Properties
	// NOTE: Colors are stored in the HSV format so the hue isn't lost at 0 saturation or value
	UPROPERTY(EditAnywhere)
	uint8 Hue;
	UPROPERTY(EditAnywhere)
	uint8 Value;
	UPROPERTY(EditAnywhere)
	uint8 Saturation;
	// ~Properties

public:
	static FBrickColor FromHSV(const FLinearColor& InColor)
	{
		FBrickColor NewColor;
		NewColor.SetHue(InColor.R);
		NewColor.SetSaturation(InColor.G);
		NewColor.SetValue(InColor.B);
		return NewColor;
	}

	static FBrickColor FromLinearRGB(const FLinearColor& InColor)
	{
		return FromHSV(InColor.LinearRGBToHSV());
	}

	// ~Constructor
	FBrickColor()
		: Hue(0), Value(0), Saturation(0)
	{
	}

	virtual ~FBrickColor() = default;

	virtual FString ToString() const
	{
		auto Result = FString();
		ByteToHex(Hue, Result);
		ByteToHex(Saturation, Result);
		ByteToHex(Value, Result);
		ByteToHex(GetAlphaInternal(), Result);
		return Result;
	}

	void FromString(const FString& Str)
	{
		auto Bytes = TArray<uint8>();
		Bytes.SetNumUninitialized(FMath::DivideAndRoundUp(Str.Len(), 2));
		const auto NumBytes = HexToBytes(Str, Bytes.GetData());

		Hue = NumBytes > 0 ? Bytes[0] : 0;
		Saturation = NumBytes > 1 ? Bytes[1] : 0;
		Value = NumBytes > 2 ? Bytes[2] : 0;
		const auto Alpha = NumBytes > 3 ? Bytes[3] : 0;
		SetAlphaInternal(Alpha);
	}

	// Convert to a linear color struct in HSV space
	FLinearColor ToHSV() const
	{
		return FLinearColor(GetHue(), GetSaturation(), GetValue(), GetAlpha());
	}

	// Convert to linear color RGB space
	FLinearColor ToLinearRGB() const
	{
		return ToHSV().HSVToLinearRGB();
	}

	FColor ToFColor() const
	{
		return ToLinearRGB().ToFColor(false);
	}

	float GetHue() const
	{
		return Hue / 255.f * 360.f;
	}

	void SetHue(float NewHue)
	{
		Hue = FMath::RoundToInt(NewHue / 360.f * 255.f);
	}

	float GetSaturation() const
	{
		return Saturation / 255.f;
	}

	void SetSaturation(float NewSaturation)
	{
		Saturation = FMath::RoundToInt(NewSaturation * 255.f);
	}

	float GetValue() const
	{
		return Value / 255.f;
	}

	void SetValue(float NewValue)
	{
		Value = FMath::RoundToInt(NewValue * 255.f);
	}

	float GetAlpha() const
	{
		return GetAlphaInternal() / 255.f;
	}

	// NOTE: This is only functional on the FBrickColorWithAlpha child struct
	void SetAlpha(const float NewAlpha)
	{
		SetAlphaInternal(FMath::RoundToInt(NewAlpha * 255.f));
	}

	bool operator==(const FBrickColor& Other) const
	{
		return Hue == Other.Hue && Saturation == Other.Saturation && Value == Other.Value;
	}

	virtual bool Serialize(FArchive& Ar)
	{
		Ar << Hue;
		Ar << Saturation;
		Ar << Value;
		return true;
	}

	// Used for legacy properties that were saved as RGB
	bool LoadFromRGB(FArchive& Ar)
	{
		FColor ColorRGB;
		Ar << ColorRGB.R;
		Ar << ColorRGB.G;
		Ar << ColorRGB.B;
		// NOTE: Do not pass FColor into the FLinearColor constructor, as that does srgb conversion
		*this = FromLinearRGB(FLinearColor(ColorRGB.R / 255.f, ColorRGB.G / 255.f, ColorRGB.B / 255.f));
		return true;
	}

	bool ExportTextItem(FString& ValueStr, const FBrickColor& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
	{
		ValueStr += ToString();
		return true;
	}

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* OwnerObject, FOutputDevice* ErrorText)
	{
		auto ImportedString = FString();
		const auto* NewBuffer = FPropertyHelpers::ReadToken(Buffer, ImportedString, false);
		if (!NewBuffer)
		{
			return false;
		}

		FromString(ImportedString);
		Buffer = NewBuffer;

		return true;
	}

private:
	virtual uint8 GetAlphaInternal() const
	{
		return 255;
	}

	virtual void SetAlphaInternal(const uint8 NewAlpha)
	{
	}
};

template <>
struct TStructOpsTypeTraits<FBrickColor> : TStructOpsTypeTraitsBase2<FBrickColor>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithExportTextItem = true,
		WithImportTextItem = true
	};
};

USTRUCT(BlueprintType)
struct FBrickColorWithAlpha : public FBrickColor
{
	GENERATED_BODY()

protected:
	// ~Properties
	UPROPERTY(EditAnywhere)
	uint8 Alpha;
	// ~Properties

public:
	static FBrickColorWithAlpha FromHSV(const FLinearColor& InColor)
	{
		FBrickColorWithAlpha NewColor;
		NewColor.SetHue(InColor.R);
		NewColor.SetSaturation(InColor.G);
		NewColor.SetValue(InColor.B);
		NewColor.SetAlpha(InColor.A);
		return NewColor;
	}

	static FBrickColorWithAlpha FromLinearRGB(const FLinearColor& InColor)
	{
		return FromHSV(InColor.LinearRGBToHSV());
	}

	// ~Constructor
	FBrickColorWithAlpha()
		: Alpha(255)
	{
	}

	void SetHVS(const FBrickColor& InColor)
	{
		SetHue(InColor.GetHue());
		SetValue(InColor.GetValue());
		SetSaturation(InColor.GetSaturation());
	}

	bool operator==(const FBrickColorWithAlpha& Other) const
	{
		return FBrickColor::operator==(Other) && Alpha == Other.Alpha;
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		FBrickColor::Serialize(Ar);
		Ar << Alpha;
		return true;
	}

private:
	// ~Super Interface
	virtual uint8 GetAlphaInternal() const override
	{
		return Alpha;
	}

	virtual void SetAlphaInternal(const uint8 NewAlpha) override
	{
		Alpha = NewAlpha;
	}

	// ~Super Interface
};

template <>
struct TStructOpsTypeTraits<FBrickColorWithAlpha> : TStructOpsTypeTraitsBase2<FBrickColorWithAlpha>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithExportTextItem = true,
		WithImportTextItem = true
	};
};
