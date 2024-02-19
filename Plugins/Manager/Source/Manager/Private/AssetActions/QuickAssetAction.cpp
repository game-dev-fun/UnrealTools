// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"

void UQuickAssetAction::DuplicateAsset(int32 NoOfDuplicates, int32 StartingNumber)
{
	if (NoOfDuplicates <= 0)
	{
		ShowDialogMsg(EAppMsgType::Ok, TEXT("Enter a Valid Duplicate Number"));
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
				ShowDialogMsg(EAppMsgType::Ok,FString::Printf(TEXT("Naming Conflict: %s"), *NewAssetName));
				if (counter > 0)
				{
					ShowNotifyInfo(FString::Printf(TEXT("%s succesfully duplicated %d times"), *AssetName, counter));
				}
				return;
			}
			if (UEditorAssetLibrary::DuplicateAsset(SelectedAsset.GetSoftObjectPath().ToString(), Destination))
			{
				++counter;
			}
			UEditorAssetLibrary::SaveAsset(Destination, false);
		}
		if (counter > 0)
		{
			ShowNotifyInfo(FString::Printf(TEXT("%s succesfully duplicated %d times"), *AssetName, counter));
		}
	}
}
