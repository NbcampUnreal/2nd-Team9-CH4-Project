#include "AnimNotify_LandingEnd.h"
#include "GameCore/Fighter/Fighter.h"

void UAnimNotify_LandingEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AFighter* Fighter = Cast<AFighter>(MeshComp->GetOwner()))
		{
			Fighter->SetIdleTag();
		}
	}
}
