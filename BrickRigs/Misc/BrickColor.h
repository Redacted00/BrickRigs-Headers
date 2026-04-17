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

	void SetSaturation(float NewSatuation)
	{
		Saturation = FMath::RoundToInt(NewSatuation * 255.f);
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

	bool operator==(const FBrickColor& Other) const
	{
		return Hue == Other.Hue && Saturation == Other.Saturation && Value == Other.Value;
	}

	bool Serialize(FArchive& Ar)
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

private:
	virtual uint8 GetAlphaInternal() const
	{
		return 255;
	}
};

template <>
struct TStructOpsTypeTraits<FBrickColor> : public TStructOpsTypeTraitsBase2<FBrickColor>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true
	};
};

USTRUCT(BlueprintType)
struct FBrickColorWithAlpha : public FBrickColor
{
	GENERATED_BODY()
	;

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
		: FBrickColor(), Alpha(255)
	{
	}

	void SetHVS(const FBrickColor& InColor)
	{
		SetHue(InColor.GetHue());
		SetValue(InColor.GetValue());
		SetSaturation(InColor.GetSaturation());
	}

	void SetAlpha(float NewAlpha)
	{
		Alpha = FMath::RoundToInt(NewAlpha * 255.f);
	}

	bool operator==(const FBrickColorWithAlpha& Other) const
	{
		return FBrickColor::operator==(Other) && Alpha == Other.Alpha;
	}

	bool Serialize(FArchive& Ar)
	{
		FBrickColor::Serialize(Ar);
		Ar << Alpha;
		return true;
	}

	bool ExportTextItem(FString& ValueStr, const FBrickColorWithAlpha& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
	{
		ValueStr = ToHSV().ToString();
		return true;
	}

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, class UObject* OwnerObject, FOutputDevice* ErrorText)
	{
		auto NewValue = FLinearColor();
		NewValue.InitFromString(Buffer);
		*this = FromHSV(NewValue);
		return true;
	}

private:
	// ~Super Interface
	virtual uint8 GetAlphaInternal() const override
	{
		return Alpha;
	}

	// ~Super Interface
};

template <>
struct TStructOpsTypeTraits<FBrickColorWithAlpha> : public TStructOpsTypeTraitsBase2<FBrickColorWithAlpha>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithExportTextItem = true,
		WithImportTextItem = true
	};
};
