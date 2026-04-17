#pragma once

#include "Engine/Texture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "CoreMinimal.h"

struct FBrickImageUtils
{
	// Returns whether the given coord is within the image
	static bool IsValidPixelCoord(const FIntPoint& Size, const FIntPoint& Coord)
	{
		return Coord.X >= 0 && Coord.X < Size.X && Coord.Y >= 0 && Coord.Y < Size.Y;
	}

	// Returns the index of a pixel in an image of the given size at the given coord
	static int32 GetPixelIndex(const FIntPoint& Size, const FIntPoint& Coord)
	{
		return Coord.X + Coord.Y * Size.X;
	}

	// Returns the index of the RGB channel bytes
	static int32 GetPixelRGBIndex(const FIntPoint& Size, const FIntPoint& Coord)
	{
		return GetPixelIndex(Size, Coord) * sizeof(FColor);
	}

	// Returns the index of the alpha channel byte
	static int32 GetPixelAlphaIndex(const FIntPoint& Size, const FIntPoint& Coord)
	{
		return GetPixelRGBIndex(Size, Coord) + 3;
	}

	// Version that takes FColor arrays
	static void ResizeImage(const TArray<FColor>& SrcData, const FIntPoint& SrcSize, TArray<FColor>& DstData, const FIntPoint& DstSize)
	{
		DstData.SetNumUninitialized(DstSize.X * DstSize.Y);
		ResizeImage((const uint8*)SrcData.GetData(), SrcSize, (uint8*)DstData.GetData(), DstSize);
	}

	// Byte array version
	static void ResizeImage(const TArray<uint8>& SrcData, const FIntPoint& SrcSize, TArray<uint8>& DstData, const FIntPoint& DstSize)
	{
		DstData.SetNumUninitialized(DstSize.X * DstSize.Y * sizeof(FColor));
		ResizeImage(SrcData.GetData(), SrcSize, DstData.GetData(), DstSize);
	}

	// Version that takes raw data
	// IMPORTANT: DstData has to be preallocated to the correct size!
	static void ResizeImage(const uint8* SrcData, const FIntPoint& SrcSize, uint8* DstData, const FIntPoint& DstSize)
	{
		if (SrcSize == DstSize)
		{
			// If the size is identical, just copy the pixels over
			const int32 NumBytes = SrcSize.X * SrcSize.Y * sizeof(FColor);
			FMemory::Memcpy(DstData, SrcData, NumBytes);
			return;
		}

		float SrcOffsetX = 0.f;
		float SrcOffsetY = 0.f;
		float StepSizeX = SrcSize.X / static_cast<float>(DstSize.X);
		float StepSizeY = SrcSize.Y / static_cast<float>(DstSize.Y);

		// Calculate the uniform scale needed to place the source image entirely in the destination image
		// This is needed for cases where the aspect ratio is different, so the image is scaled uniformally instead of stretched
		const float SrcAspectRatio = SrcSize.X / static_cast<float>(SrcSize.Y);
		const float DstAspectRatio = DstSize.X / static_cast<float>(DstSize.Y);
		if (SrcAspectRatio > DstAspectRatio)
		{
			// If Src is wider than Dst
			const float SrcScale = DstAspectRatio / SrcAspectRatio;
			SrcOffsetY = (1.f - SrcScale) * SrcSize.Y * 0.5f;
			StepSizeY /= SrcScale;
		}
		else
		{
			// If Dst is wider than Src
			const float SrcScale = SrcAspectRatio / DstAspectRatio;
			SrcOffsetX = (1.f - SrcScale) * SrcSize.X * 0.5f;
			StepSizeX /= SrcScale;
		}

		for (int32 Y = 0; Y < DstSize.Y; Y++)
		{
			const float SrcY = (Y - SrcOffsetY) * StepSizeY;
			for (int32 X = 0; X < DstSize.X; X++)
			{
				const float SrcX = (X - SrcOffsetX) * StepSizeX;
				const float EndX = SrcX + StepSizeX;
				const float EndY = SrcY + StepSizeY;

				// Generate a rectangular region of pixels and then find the average color of the region.
				const int32 PosY = FMath::TruncToInt(SrcY + 0.5f);
				const int32 PosX = FMath::TruncToInt(SrcX + 0.5f);
				const int32 EndPosY = FMath::TruncToInt(EndY + 0.5f);
				const int32 EndPosX = FMath::TruncToInt(EndX + 0.5f);

				FLinearColor StepColor(0.f, 0.f, 0.f, 0.f);
				// We need to count the color and alpha value separately, since color is only considered when alpha is > 0
				int32 ColorCount = 0;
				int32 AlphaCount = 0;

				for (int32 PixelX = PosX; PixelX <= EndPosX; PixelX++)
				{
					for (int32 PixelY = PosY; PixelY <= EndPosY; PixelY++)
					{
						if (PixelX >= 0 && PixelX < SrcSize.X && PixelY >= 0 && PixelY < SrcSize.Y)
						{
							const int32 SrcIndex = (PixelX + PixelY * SrcSize.X) * sizeof(FColor);
							const uint8 SrcAlpha = SrcData[SrcIndex + 3];
							StepColor.A += static_cast<float>(SrcAlpha);
							AlphaCount++;

							// Only consider the pixel for the color when it has any opacity, otherwise the color value might not be correct
							if (SrcAlpha > 0)
							{
								StepColor.R += static_cast<float>(SrcData[SrcIndex]);
								StepColor.G += static_cast<float>(SrcData[SrcIndex + 1]);
								StepColor.B += static_cast<float>(SrcData[SrcIndex + 2]);
								ColorCount++;
							}
						}
					}
				}

				// Store the final averaged pixel color value
				const int32 DstIndex = (X + Y * DstSize.X) * sizeof(FColor);

				if (AlphaCount > 0)
				{
					DstData[DstIndex + 3] = FMath::Clamp(FMath::TruncToInt(StepColor.A / static_cast<float>(AlphaCount)), 0, 255);
				}
				else
				{
					// The pixel is out of bounds, make it transparent
					DstData[DstIndex + 3] = 0;
				}

				if (ColorCount > 0)
				{
					DstData[DstIndex] = FMath::Clamp(FMath::TruncToInt(StepColor.R / static_cast<float>(ColorCount)), 0, 255);
					DstData[DstIndex + 1] = FMath::Clamp(FMath::TruncToInt(StepColor.G / static_cast<float>(ColorCount)), 0, 255);
					DstData[DstIndex + 2] = FMath::Clamp(FMath::TruncToInt(StepColor.B / static_cast<float>(ColorCount)), 0, 255);
				}
				else
				{
					// The texel is fully transparent, just make it black
					DstData[DstIndex] = 0;
					DstData[DstIndex + 1] = 0;
					DstData[DstIndex + 2] = 0;
				}
			}
		}
	}

	// Applies a rounded edge effect to an image, radius is relative to half the size of the shortest side
	static void RoundImageCorners(TArray<uint8>& InOutImageData, const FIntPoint& Size, float Radius)
	{
		const int32 PixelRadius = FMath::FloorToInt(Size.GetMin() * Radius * 0.5f);
		if (PixelRadius == 0)
		{
			// No need to proceed if the bevel would be infinitely small
			return;
		}

		// Iterate through all corners
		for (int32 DirX = 1; DirX >= -1; DirX -= 2)
		{
			const int32 StartX = DirX > 0 ? 0 : Size.X - 1;
			for (int32 DirY = 1; DirY >= -1; DirY -= 2)
			{
				const int32 StartY = DirY > 0 ? 0 : Size.Y - 1;

				for (int32 X = 0; X < PixelRadius; ++X)
				{
					const int32 PixelX = StartX + X * DirX;
					// The number of pixels that have to be iterated vertically
					const int32 NumY = FMath::Min(FMath::RoundToInt(PixelRadius - FMath::Sqrt(FMath::Square(PixelRadius) - FMath::Square(X - PixelRadius))), PixelRadius);

					for (int32 Y = 0; Y < NumY; ++Y)
					{
						const int32 PixelY = StartY + Y * DirY;
						InOutImageData[GetPixelAlphaIndex(Size, FIntPoint(PixelX, PixelY))] = 0;
					}
				}
			}
		}
	}

	// Applies a drop shadow effect to the given image
	static void RenderDropShadow(TArray<uint8>& InOutImageData, const FIntPoint& Size, const FVector2D& ShadowOffset)
	{
		const auto PixelOffset = FIntPoint(FMath::FloorToInt(Size.GetMin() * ShadowOffset.X), FMath::FloorToInt(Size.GetMin() * ShadowOffset.Y));
		if (PixelOffset.X == 0 && PixelOffset.Y == 0)
		{
			// No need to proceed if the offset is zero
			return;
		}

		// Iterate through all pixels
		// NOTE: We always have to iterate opposite to the pixel offset direction, so we don't end up sampling the drop shadow itself
		const auto Dir = FIntPoint(PixelOffset.X >= 0 ? -1 : 1, PixelOffset.Y >= 0 ? -1 : 1);
		for (auto X = Dir.X > 0 ? 0 : Size.X - 1; X >= 0 && X < Size.X; X += Dir.X)
		{
			for (auto Y = Dir.Y > 0 ? 0 : Size.Y - 1; Y >= 0 && Y < Size.Y; Y += Dir.Y)
			{
				const auto Coord = FIntPoint(X, Y);
				const auto AlphaIndex = GetPixelAlphaIndex(Size, Coord);
				// No need to proceed if the 
				if (InOutImageData[AlphaIndex] < 255)
				{
					const auto OtherCoord = FIntPoint(X - PixelOffset.X, Y - PixelOffset.Y);
					if (IsValidPixelCoord(Size, OtherCoord))
					{
						// Check if the sample pixel has any opacity
						const auto OtherAlphaIndex = GetPixelAlphaIndex(Size, OtherCoord);
						if (InOutImageData[OtherAlphaIndex] > 0)
						{
							// Adjust the opacity
							InOutImageData[AlphaIndex] = FMath::Max(InOutImageData[AlphaIndex], InOutImageData[OtherAlphaIndex]);

							// Lerp the color to black
							const auto Alpha = InOutImageData[AlphaIndex] / 255.f;
							const auto RGBIndex = GetPixelRGBIndex(Size, Coord);
							InOutImageData[RGBIndex] = FMath::RoundToInt(InOutImageData[RGBIndex] * Alpha);
							InOutImageData[RGBIndex + 1] = FMath::RoundToInt(InOutImageData[RGBIndex + 1] * Alpha);
							InOutImageData[RGBIndex + 2] = FMath::RoundToInt(InOutImageData[RGBIndex + 2] * Alpha);
						}
					}
				}
			}
		}
	}

	// Overwrites a texture with raw image data
	static void CopyImageToTexture(UTexture2D* Texture, const uint8* RawData, const FIntPoint& Size)
	{
		check(Texture);

		// Create the platform data if needed
		if (!Texture->PlatformData)
		{
			Texture->PlatformData = new FTexturePlatformData();
		}
		Texture->LODGroup = TEXTUREGROUP_UI;
		Texture->CompressionSettings = TC_EditorIcon;
#if WITH_EDITOR
		Texture->MipGenSettings = TMGS_FromTextureGroup;
#endif
		Texture->PlatformData->SizeX = Size.X;
		Texture->PlatformData->SizeY = Size.Y;
		Texture->PlatformData->PixelFormat = PF_B8G8R8A8;

		// Create the first mip if needed
		if (Texture->PlatformData->Mips.Num() != 1)
		{
			Texture->PlatformData->Mips.Empty(1);
			Texture->PlatformData->Mips.Add(new FTexture2DMipMap());
		}
		FTexture2DMipMap& MipMap = Texture->PlatformData->Mips[0];
		MipMap.SizeX = Size.X;
		MipMap.SizeY = Size.Y;

		const uint32 NumPixels = Size.X * Size.Y;
		const uint32 NumBytesPerPixel = sizeof(FColor);
		const uint32 NumBytes = NumPixels * NumBytesPerPixel;

		// Reallocate the bulk data so the new texture data can be fed in
		FByteBulkData& BulkData = MipMap.BulkData;
		BulkData.Lock(LOCK_READ_WRITE);
		uint8* TextureData = static_cast<uint8*>(BulkData.Realloc(NumBytes));

		// Copy the image to the texture
		FMemory::Memcpy(TextureData, RawData, NumBytes);

#if WITH_EDITOR
		// Initialize the savable data
		Texture->Source.Init(Size.X, Size.Y, 1, 1, TSF_BGRA8, TextureData);
#endif
		// Unlock the bulk data again
		BulkData.Unlock();
		// Update the texture
		Texture->UpdateResource();
	}

	static void CopyRenderTargetToTexture(UTexture2D* Texture, UTextureRenderTarget2D* RenderTarget)
	{
		check(RenderTarget && Texture);

		FRenderTarget* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
		if (!RenderTargetResource)
		{
			ensure(false);
			return;
		}

		// Read the image data from the render target
		TArray<FColor> ImageData;
		RenderTargetResource->ReadPixels(ImageData);

		CopyImageToTexture(Texture, (uint8*)ImageData.GetData(), FIntPoint(RenderTarget->SizeX, RenderTarget->SizeY));
	}

	// Extracts the image data from a texture
	// WARNING: This will only work on textures marked as a VectorDisplacementMap, NoMipMaps and no sRGB
	static void GetTextureImageData(UTexture2D* Texture, TArray<uint8>& OutData, FIntPoint& OutSize)
	{
		check(Texture);

		if (Texture->PlatformData->Mips.Num() > 0)
		{
			FTexture2DMipMap& MipMap = Texture->PlatformData->Mips[0];
			OutSize.X = MipMap.SizeX;
			OutSize.Y = MipMap.SizeY;

			if (OutSize.X > 0 && OutSize.Y > 0)
			{
				const int32 NumPixels = OutSize.X * OutSize.Y;
				const uint32 NumBytesPerPixel = sizeof(FColor);
				const uint32 NumBytes = NumPixels * NumBytesPerPixel;

				// Reserve space in the output array
				OutData.SetNumUninitialized(NumBytes);

				// NOTE: This workaround is needed because otherwise BulkData.LockReadOnly() will return nullptr
				/*const auto OldCompressionSettings = Texture->CompressionSettings;
				const auto OldMipGenSettings = Texture->MipGenSettings;
				const auto OldSRGB = Texture->SRGB;

				Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
				Texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
				Texture->SRGB = false;
				Texture->UpdateResource();*/

				FByteBulkData& BulkData = MipMap.BulkData;
				const uint8* ImageData = (uint8*)BulkData.LockReadOnly();
				check(ImageData);

				if (ImageData)
				{
					// Copy the image over to the output array
					FMemory::Memcpy(OutData.GetData(), ImageData, NumBytes);
				}
				BulkData.Unlock();

				// Restore the original settings
				/*Texture->CompressionSettings = OldCompressionSettings;
				Texture->MipGenSettings = OldMipGenSettings;
				Texture->SRGB = OldSRGB;
				Texture->UpdateResource();*/
			}
		}
	}

	// NOTE: This won't work outside the editor, since BulkData.LockReadOnly() always returns null
#if WITH_EDITOR
	// Returns the color of the pixel at the given coordinates
	// WARNING: This will only work on textures marked as a VectorDisplacementMap, NoMipMaps and no sRGB
	static FColor SampleTexture(UTexture2D* Texture, const FIntPoint& Coord)
	{
		check(Texture && Coord.X >= 0 && Coord.Y >= 0);
		ensure(Texture->CompressionSettings == TextureCompressionSettings::TC_VectorDisplacementmap);
		ensure(Texture->MipGenSettings == TextureMipGenSettings::TMGS_NoMipmaps);

		FColor OutColor = FColor::Transparent;

		if (Texture->PlatformData->Mips.Num() > 0)
		{
			FTexture2DMipMap& MipMap = Texture->PlatformData->Mips[0];
			if (Coord.X < MipMap.SizeX && Coord.Y < MipMap.SizeY)
			{
				FByteBulkData& BulkData = MipMap.BulkData;
				const FColor* ImageData = (FColor*)BulkData.LockReadOnly();
				ensure(ImageData);

				if (ImageData)
				{
					const int32 PixelIndex = Coord.X + Coord.Y * MipMap.SizeX;
					OutColor = ImageData[PixelIndex];
				}
				BulkData.Unlock();
			}
		}

		return OutColor;
	}
#endif
};
