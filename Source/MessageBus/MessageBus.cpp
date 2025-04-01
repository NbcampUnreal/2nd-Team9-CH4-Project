#include "MessageBus.h"

UMessageBus* UMessageBus::GetInstance()
{
	static UMessageBus* Instance = NewObject<UMessageBus>();
	
	return Instance;
}

void UMessageBus::BroadcastMessage(const FString& MessageType, UObject* Payload) const
{
	if (const TArray<FMessageDelegate>* DelegateArray = Subscribers.Find(MessageType))
	{
		for (const FMessageDelegate& Delegate : *DelegateArray)
		{
			Delegate.ExecuteIfBound(MessageType, Payload);
		}
	}
}

void UMessageBus::Subscribe(const FString& MessageType, const FMessageDelegate& Delegate)
{
	//FindOrAdd는 제공한 Key값으로 중복된걸 찾으면 Value를 리턴하고 없으면 새로 생성해준다고 한다
	Subscribers.FindOrAdd(MessageType).Add(Delegate);
}

// 필요에 따라 try-catch나 조건 검사를 추가할 수 있음.