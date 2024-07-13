// PythonBridgeActorComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include <future>

#include "PythonBridgeActorComponent.generated.h"




UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PYTHONBRIDGE_API UPythonBridgeActorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPythonBridgeActorComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;



    TArray<int32> EncodeFString(const FString& InputText);



    FString DecodeTokens(const TArray<int32>& TokenIds);


    UFUNCTION(BlueprintCallable)
    int TestEncode(FString StringToEncode);


protected:
    TArray<std::future<void>> AsyncTasks;
};
