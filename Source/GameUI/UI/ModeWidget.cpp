// Fill out your copyright notice in the Description page of Project Settings.


#include "ModeWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"





void UModeWidget::NativeConstruct()
{  
  Super::NativeConstruct();  

  if (ModeRightButton)  
  {  
      ModeRightButton->OnClicked.AddDynamic(this, &UModeWidget::ShiftModeRight);  
  }  
  if (ModeLeftButton)  
  {  
      ModeLeftButton->OnClicked.AddDynamic(this, &UModeWidget::ShiftModeLeft);  
  }  

  if (TimeRightButton)
  {
      TimeRightButton->OnClicked.AddDynamic(this, &UModeWidget::ShiftTimeRight);
  }
  if (TimeLeftButton)
  {
      TimeLeftButton->OnClicked.AddDynamic(this, &UModeWidget::ShiftTimeLeft);
  }




  InitializeGameModes();
  UpdateDisplayedMode();
  
}



void UModeWidget::InitializeGameModes()
{
    TArray<FString> ModeTagNames = {
        "ModeSelect.Mode.Single"
        "ModeSelect.Mode.Team"
        "ModeSelect.Mode.Survival"

    };

    ModeTags.Empty();
    for (const FString& TagName : ModeTagNames)
    {
        FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(*TagName), false);
        if (Tag.IsValid())
            ModeTags.Add(Tag);
    }

    TArray<FString> TimeTagNames = {
       "ModeSelect.Time.180"
       "ModeSelect.Time.300"
       "ModeSelect.Time.600"

    };

    TimeTags.Empty();
    for (const FString& TagName : TimeTagNames)
    {
        FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(*TagName), false);
        if (Tag.IsValid())
            TimeTags.Add(Tag);
    }

    
}

void UModeWidget::UpdateDisplayedMode()
{
    //Mode

    if (ModeNameText && ModeTags.IsValidIndex(CurrentIndex))
    {
        const FGameplayTag& ModeTag = ModeTags[CurrentIndex];
        FString FullTag = ModeTag.ToString();

        FString Left, Right;
        if (ModeTag.ToString().Split(TEXT("."), &Left, &Right))
            ModeNameText->SetText(FText::FromString(Right));
        else
            ModeNameText->SetText(FText::FromString(ModeTag.ToString()));
    }
    
    //Time
    
    if (ModeNameText && ModeTags.IsValidIndex(CurrentIndex))
    {
        const FGameplayTag& TimeTag = ModeTags[CurrentIndex];
        FString Left, Right;
        if (TimeTag.ToString().Split(TEXT("."), &Left, &Right))
            ModeNameText->SetText(FText::FromString(Right));
        else
            ModeNameText->SetText(FText::FromString(TimeTag.ToString()));
    }
    
}



void UModeWidget::ShiftModeRight()
{
    if (ModeTags.Num() == 0) return;
    CurrentIndex = (CurrentIndex + 1 ) % ModeTags.Num();
    UpdateDisplayedMode();
}

void UModeWidget::ShiftModeLeft()
{
    if (ModeTags.Num() == 0) return;
    CurrentIndex = (CurrentIndex - 1 + ModeTags.Num()) % ModeTags.Num();
    UpdateDisplayedMode();
}



void UModeWidget::ShiftTimeRight()
{
    if(TimeTags.Num() == 0) return;
    TimeIndex = (TimeIndex + 1) % TimeTags.Num();
    UpdateDisplayedMode();

}

void UModeWidget::ShiftTimeLeft()
{
    if (TimeTags.Num() == 0) return;
    CurrentIndex = (CurrentIndex - 1 + TimeTags.Num()) % TimeTags.Num();
    UpdateDisplayedMode();
}

FGameplayTag UModeWidget::TimesSelectTag() const
{
 return TimeTags.IsValidIndex(TimeIndex) ? TimeTags[TimeIndex]:FGameplayTag();
}

FGameplayTag UModeWidget::ModesSelectTag() const
{
    return FGameplayTag();
}

