#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MessageBus.generated.h"

DECLARE_DELEGATE_TwoParams(FMessageDelegate, const FString&, UObject*);

UCLASS()
class MESSAGEBUS_API UMessageBus : public UObject
{
	GENERATED_BODY()
public:
	static UMessageBus* GetInstance();

public:
	void BroadcastMessage(const FString& MessageType, UObject* Payload) const;
	void Subscribe(const FString& MessageType, const FMessageDelegate& Delegate);
	
	TMap<FString, TArray<FMessageDelegate>> Subscribers;
};
