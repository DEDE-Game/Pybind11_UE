# -*- coding: utf-8 -*-


from transformers import GPT2Tokenizer, GPT2LMHeadModel
import torch

# ���غͼ���ģ�ͼ��ִ���
tokenizer = GPT2Tokenizer.from_pretrained('distilgpt2')
model = GPT2LMHeadModel.from_pretrained('distilgpt2')

# �����豸���������GPU������ʹ��cuda��
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model.to(device)

# �����ı��ĺ���
def generate_text(prompt, max_length=50):
    inputs = tokenizer(prompt, return_tensors="pt").to(device)
    outputs = model.generate(inputs.input_ids, max_length=max_length, num_return_sequences=1)
    return tokenizer.decode(outputs[0], skip_special_tokens=True)

# ��������
prompt = "�������һ���������غ�Σ�յĵط���"
generated_text = generate_text(prompt)
print(generated_text)
