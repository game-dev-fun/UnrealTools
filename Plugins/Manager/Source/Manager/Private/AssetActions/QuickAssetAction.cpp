// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"

void UQuickAssetAction::DuplicateAsset(int32 NoOfDuplicates, int32 StartingNumber)
{
	if (NoOfDuplicates <= 0)
	{
		Print(TEXT("Please enter a valid number"), FColor::Red);
		return;
	}
	TArray<FAssetData> SelectedAssets{ UEditorUtilityLibrary::GetSelectedAssetData() };
	for (const FAssetData& SelectedAsset : SelectedAssets)
	{
		FString Path{ SelectedAsset.PackagePath.ToString()};
		FString AssetName{ SelectedAsset.AssetName.ToString() };
		int32 counter{0};
		for (int i{ 0 }; i < NoOfDuplicates; ++i)
		{
			FString NewAssetName = FString::Printf(TEXT("%s_%d"), *AssetName, counter + StartingNumber) ;
			FString Destination{ FPaths::Combine(Path,NewAssetName) };
			bool AssetAlreadyExist{ UEditorAssetLibrary::DoesAssetExist(Destination) };
			if (AssetAlreadyExist)
			{
				Print(FString::Printf(TEXT("Naming Conflict: %s"), *NewAssetName), FColor::Red);
				if (counter > 0)
				{
					Print(FString::Printf(TEXT("%s succesfully duplicated %d times"), *AssetName, counter), FColor::Green);
				}
				return;
			}
			if (UEditorAssetLibrary::DuplicateAsset(SelectedAsset.GetSoftObjectPath().ToString(), Destination))
			{
				++counter;
			}
			UEditorAssetLibrary::SaveAsset(Destination, false);
		}
		Print(FString::Printf(TEXT("%s succesfully duplicated %d times"), *AssetName, counter), FColor::Green);
	}
}
