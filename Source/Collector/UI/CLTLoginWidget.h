#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CLTLoginWidget.generated.h"

class UEditableTextBox;
class UButton;

/**
 * Login Widget for TCP/UDP Hybrid Connection.
 */
UCLASS()
class COLLECTOR_API UCLTLoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Input_IP;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Host;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Login;

	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnLoginClicked();
	
	UFUNCTION()
	void OnLoginResult(bool bSuccess);
};
