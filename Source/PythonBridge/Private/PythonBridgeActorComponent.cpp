// PythonBridgeActorComponent.cpp
#include "PythonBridgeActorComponent.h"
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

THIRD_PARTY_INCLUDES_START
#pragma push_macro("check")
#undef check
#pragma warning (push)
#pragma warning (disable : 4191)
#pragma warning (disable : 4686)
#include "pybind11/pybind11.h"
#pragma warning (pop)
#pragma pop_macro("check")
THIRD_PARTY_INCLUDES_END

#include <pybind11/embed.h>  // ����������
#include <pybind11/stl.h>    // ʹ�� C++ STL ����

namespace py = pybind11;

// ȫ�ֵ� Python ������������
class PythonInterpreterManager
{
public:
    static PythonInterpreterManager& Get()
    {
        static PythonInterpreterManager Instance;
        return Instance;
    }

    void Initialize()
    {
        if (!Py_IsInitialized())
        {
            py::initialize_interpreter();
        }
    }

    void Finalize()
    {
        if (Py_IsInitialized())
        {
            py::finalize_interpreter();
        }
    }

private:
    PythonInterpreterManager() = default;
    ~PythonInterpreterManager()
    {
        if (Py_IsInitialized())
        {
            py::finalize_interpreter();
        }
    }
};

UPythonBridgeActorComponent::UPythonBridgeActorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UPythonBridgeActorComponent::BeginPlay()
{
    Super::BeginPlay();
    // ��ʼ�� Python ������
    PythonInterpreterManager::Get().Initialize();
}

void UPythonBridgeActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    PythonInterpreterManager::Get().Finalize();  // ���� Python ������
    Super::EndPlay(EndPlayReason);
}

TArray<int32> UPythonBridgeActorComponent::EncodeFString(const FString& InputText)
{
    // ȷ�� Python �������ѳ�ʼ��
    PythonInterpreterManager::Get().Initialize();
    
    // ���� transformers ģ��
    py::module_ transformers = py::module_::import("transformers");

    // ���� DistilGPT2 �ִ���
    py::object GPT2Tokenizer = transformers.attr("GPT2Tokenizer");
    py::object tokenizer = GPT2Tokenizer.attr("from_pretrained")("distilgpt2");

    // �������ı����зִ�
    py::list token_ids = tokenizer.attr("encode")(TCHAR_TO_UTF8(*InputText));

    // ���ִʽ��ת��Ϊ TArray<int32>
    TArray<int32> TokenizedText;
    for (auto token_id : token_ids)
    {
        TokenizedText.Add(token_id.cast<int32>());
    }

    return TokenizedText;
}


FString UPythonBridgeActorComponent::DecodeTokens(const TArray<int32>& TokenIds)
{
    // ����Python������
    //py::scoped_interpreter guard{};
    
    if (!Py_IsInitialized()) 
    {
        py::scoped_interpreter guard{};
    }
    // ����transformersģ��
    py::module_ transformers = py::module_::import("transformers");

    // ����DistilGPT2�ִ���
    py::object GPT2Tokenizer = transformers.attr("GPT2Tokenizer");
    py::object tokenizer = GPT2Tokenizer.attr("from_pretrained")("distilgpt2");

    // ����һ��Python�б����洢token ID
    py::list token_ids;
    for (int32 TokenId : TokenIds)
    {
        token_ids.append(TokenId);
    }

    // ��token ID���н���
    std::string decoded_text = tokenizer.attr("decode")(token_ids).cast<std::string>();

    return FString(decoded_text.c_str());
}

int UPythonBridgeActorComponent::TestEncode(FString StringToEncode)
{
    TArray<int> ArrayInt = EncodeFString(StringToEncode);
    return ArrayInt.Num();
}
