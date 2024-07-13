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

#include <pybind11/embed.h>  // 启动解释器
#include <pybind11/stl.h>    // 使用 C++ STL 容器

namespace py = pybind11;

// 全局的 Python 解释器管理类
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
    // 初始化 Python 解释器
    PythonInterpreterManager::Get().Initialize();
}

void UPythonBridgeActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    PythonInterpreterManager::Get().Finalize();  // 结束 Python 解释器
    Super::EndPlay(EndPlayReason);
}

TArray<int32> UPythonBridgeActorComponent::EncodeFString(const FString& InputText)
{
    // 确保 Python 解释器已初始化
    PythonInterpreterManager::Get().Initialize();
    
    // 导入 transformers 模块
    py::module_ transformers = py::module_::import("transformers");

    // 创建 DistilGPT2 分词器
    py::object GPT2Tokenizer = transformers.attr("GPT2Tokenizer");
    py::object tokenizer = GPT2Tokenizer.attr("from_pretrained")("distilgpt2");

    // 对输入文本进行分词
    py::list token_ids = tokenizer.attr("encode")(TCHAR_TO_UTF8(*InputText));

    // 将分词结果转换为 TArray<int32>
    TArray<int32> TokenizedText;
    for (auto token_id : token_ids)
    {
        TokenizedText.Add(token_id.cast<int32>());
    }

    return TokenizedText;
}


FString UPythonBridgeActorComponent::DecodeTokens(const TArray<int32>& TokenIds)
{
    // 启动Python解释器
    //py::scoped_interpreter guard{};
    
    if (!Py_IsInitialized()) 
    {
        py::scoped_interpreter guard{};
    }
    // 导入transformers模块
    py::module_ transformers = py::module_::import("transformers");

    // 创建DistilGPT2分词器
    py::object GPT2Tokenizer = transformers.attr("GPT2Tokenizer");
    py::object tokenizer = GPT2Tokenizer.attr("from_pretrained")("distilgpt2");

    // 创建一个Python列表来存储token ID
    py::list token_ids;
    for (int32 TokenId : TokenIds)
    {
        token_ids.append(TokenId);
    }

    // 对token ID进行解码
    std::string decoded_text = tokenizer.attr("decode")(token_ids).cast<std::string>();

    return FString(decoded_text.c_str());
}

int UPythonBridgeActorComponent::TestEncode(FString StringToEncode)
{
    TArray<int> ArrayInt = EncodeFString(StringToEncode);
    return ArrayInt.Num();
}
