#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CollectorTypes.generated.h"

class UStaticMesh;
class UTexture2D;

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName Name = FName(TEXT("Empty"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Coin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UTexture2D> Icon;
};
