#pragma once

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