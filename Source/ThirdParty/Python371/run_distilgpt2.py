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
def generate_text(prompt, max_length=50):
    inputs = tokenizer(prompt, return_tensors="pt").to(device)
    outputs = model.generate(inputs.input_ids, max_length=max_length, num_return_sequences=1)
    return tokenizer.decode(outputs[0], skip_special_tokens=True)

# 测试生成
prompt = "风箱城是一个充满神秘和危险的地方。"
generated_text = generate_text(prompt)
print(generated_text)
