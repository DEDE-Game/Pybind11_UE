from transformers import AutoModelForCausalLM, AutoTokenizer
import torch

# 加载DistilGPT-2模型和tokenizer
model_name = "distilgpt2"
tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModelForCausalLM.from_pretrained(model_name)

# 设置模型生成参数
max_length = 10000
num_return_sequences = 1
temperature = 0.7
top_k = 50
top_p = 0.95

# 初始提示语
chat_history = "The following is a conversation with an AI assistant.\n"

print("Chat with DistilGPT-2. Type 'exit' to end the conversation.")

while True:
    # 获取用户输入
    user_input = input("You: ")
    
    if user_input.lower() == 'exit':
        break

    # 将用户输入添加到聊天历史
    chat_history += f"You: {user_input}\nAI:"

    # 编码输入
    input_ids = tokenizer.encode(chat_history, return_tensors="pt")

    # 生成回复
    output = model.generate(
        input_ids, 
        max_length=max_length, 
        num_return_sequences=num_return_sequences, 
        temperature=temperature, 
        top_k=top_k, 
        top_p=top_p,
        pad_token_id=tokenizer.eos_token_id
    )

    # 解码输出文本
    output_text = tokenizer.decode(output[0], skip_special_tokens=True)

    # 提取AI的回复部分
    reply = output_text.split("AI:")[-1].strip()

    # 打印AI的回复
    print(f"AI: {reply}")

    # 将AI的回复添加到聊天历史
    chat_history += f" {reply}\n"
