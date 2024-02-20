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

void UQuickAssetAction::AddPrefix()
{
	TArray<UObject*>  SelectedAssets{ UEditorUtilityLibrary::GetSelectedAssets()};
	uint32 counter{ 0 };
	for (UObject* SelectedAsset : SelectedAssets)
	{
		FString* Prefix{ PrefixMap.Find(SelectedAsset->GetClass()) };
		if (!Prefix)
		{
			
			Print(FString::Printf(TEXT("%s assets doesn't have a prefix"), *SelectedAsset->GetClass()->GetName()), FColor::Red);
			continue;
		}
		else
		{
			if (!SelectedAsset->GetName().StartsWith(*Prefix))
			{
				FString NewName;
				if (SelectedAsset->GetClass() == UMaterialInstanceConstant::StaticClass())
				{
					FString OldName{ SelectedAsset->GetName() };
					if (OldName.StartsWith(TEXT("M_")))
					{
						OldName.RemoveAt(0, 2);
					}
					if (OldName.EndsWith(TEXT("_Inst")))
					{
						OldName.RemoveAt(OldName.Len() - 5, 5);
					}
					NewName = *Prefix + OldName;
				}
				else
				{
					NewName = *Prefix + SelectedAsset->GetName() ;
				}
				UEditorUtilityLibrary::RenameAsset(SelectedAsset, NewName);
				++counter;
			}
		}
	}
	if (counter > 0)
	{
		ShowNotifyInfo(FString::Printf(TEXT("Successfully Added Prefixes to %d assets"), counter));
	}
}
