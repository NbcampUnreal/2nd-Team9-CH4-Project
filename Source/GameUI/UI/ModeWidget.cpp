// Fill out your copyright notice in the Description page of Project Settings.


#include "ModeWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"




void UModeWidget::NativeConstruct()  
{  
  Super::NativeConstruct();  

  if (RightButton)  
  {  
      RightButton->OnClicked.AddDynamic(this, &UModeWidget::ButtonClickRight);  
  }  
  if (LeftButton)  
  {  
      LeftButton->OnClicked.AddDynamic(this, &UModeWidget::ButtonClickLeft);  
  }  

  
}

void UModeWidget::ButtonClickRight()  
{  

  // if (UIDataRowList.Num() == 0) return;  
   //CurrentIndex = (CurrentIndex + 1) % UIDataRowList.Num();  
   UpdateText();  
}

void UModeWidget::ButtonClickLeft()
{
	//if(UIDataRowList.Num() == 0) return;
	//CurrentIndex = (CurrentIndex - 1 + UIDataRowList.Num()) % UIDataRowList.Num();
    UpdateText();
}

void UModeWidget::UpdateText()
{
	//if(UIDataRowList.IsValidIndex(CurrentIndex))
	//{
	//	FUIDataRow* Data = UIDataRowList[CurrentIndex];
	//	if (ModeNameText && Data)
	//	{
	//		ModeNameText->SetText(Data->ModeName);
	//	}
	//}
}
