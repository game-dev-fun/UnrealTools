#pragma once
#include "Misc/MessageDialog.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

EAppReturnType::Type ShowDialogMsg(const EAppMsgType::Type& MsgType, const FString& Msg, bool bShowError = true)
{
	if (bShowError)
	{
		return FMessageDialog::Open(EAppMsgCategory::Error,MsgType, FText::FromString(Msg), FText::FromString(TEXT("Error")));
	}
	else
	{
		return FMessageDialog::Open(MsgType, FText::FromString(Msg));
	}
}

void ShowNotifyInfo(const FString& Msg)
{
	FNotificationInfo NotifyInfo(FText::FromString(Msg));
	NotifyInfo.bUseLargeFont = true;
	NotifyInfo.FadeOutDuration = 7.f;
	FSlateNotificationManager::Get().AddNotification(NotifyInfo);
}

void Print(const FString& Msg, const FColor& Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, Color, Msg);
	}
}

void PrintLog(const FString& Msg)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
}