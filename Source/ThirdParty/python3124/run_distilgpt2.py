# -*- coding: utf-8 -*-

from transformers import GPT2Tokenizer, GPT2LMHeadModel
import torch

# 下载和加载模型及分词器
tokenizer = GPT2Tokenizer.from_pretrained('distilgpt2')
model = GPT2LMHeadModel.from_pretrained('distilgpt2')

# 设置设备（如果你有GPU，可以使用cuda）
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model.to(device)

# 生成文本的函数
def generate_text(prompt, max_length=50, temperature=0.7, top_k=50, top_p=0.9):
    inputs = tokenizer(prompt, return_tensors="pt").to(device)
    
    # 创建 attention_mask
    attention_mask = torch.ones(inputs.input_ids.shape, device=device)
    
    # 设置 pad_token_id 为 eos_token_id
    pad_token_id = tokenizer.eos_token_id

    outputs = model.generate(
        inputs.input_ids,
        attention_mask=attention_mask,
        max_length=max_length,
        pad_token_id=pad_token_id,
        num_return_sequences=1,
        temperature=temperature,   # 调整 temperature 参数
        top_k=top_k,               # 调整 top_k 参数
        top_p=top_p,               # 调整 top_p 参数
        do_sample=True             # 设置为 True 来启用采样
    )
    return tokenizer.decode(outputs[0], skip_special_tokens=True)

# 测试生成
prompt = "你好，"
generated_text = generate_text(prompt)
print(generated_text)
