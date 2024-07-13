from transformers import GPT2Tokenizer

def encode_text(input_text):
    # 创建GPT2分词器
    tokenizer = GPT2Tokenizer.from_pretrained("distilgpt2")

    # 对输入文本进行分词
    token_ids = tokenizer.encode(input_text)

    return token_ids

def decode_tokens(token_ids):
    # 创建GPT2分词器
    tokenizer = GPT2Tokenizer.from_pretrained("distilgpt2")

    # 对token ID进行解码
    decoded_text = tokenizer.decode(token_ids)

    return decoded_text

# 测试encode_text和decode_tokens函数
def test_functions():
    test_text = "Hello, world!"
    print(f"Test text: {test_text}")

    token_ids = encode_text(test_text)
    print(f"Encoded token IDs: {token_ids}")

    decoded_text = decode_tokens(token_ids)
    print(f"Decoded text: {decoded_text}")

# 运行测试函数
if __name__ == "__main__":
    test_functions()
