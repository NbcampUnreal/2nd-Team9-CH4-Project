#include "CustomBlueprintFunction.h"
#include "SocketSubsystem.h"

FString UCustomBlueprintFunction::GetLocalIPAddress()
{
	bool canBind = false;
	TSharedPtr<FInternetAddr> address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);

	return address.IsValid() ? address->ToString(false) : "Unknown";
}
