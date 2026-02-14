import streamlit as st
from groq import Groq
from streamlit_mic_recorder import speech_to_text
from gtts import gTTS
import os
from dotenv import load_dotenv

# 1. Setup & Security
load_dotenv()
api_key = os.getenv("GROQ_API_KEY")

if not api_key:
    st.error("🚨 API Key not found! Ensure your file is named exactly .env (no .txt)")
    st.stop()

client = Groq(api_key=api_key)

# 2. Page & Sidebar
st.set_page_config(page_title="MindEase: First-Aid Bot", page_icon="🌱")
with st.sidebar:
    st.title("🌱 MindEase")
    st.info("AI First-Aid for Social Welfare.")
    st.warning("🚨 **Emergency?** Call AASRA: 91-9820466726")

# 3. Chat History
if "messages" not in st.session_state:
    st.session_state.messages = [{"role": "system", "content": "You are a compassionate Mental Health Assistant."}]

for message in st.session_state.messages:
    if message["role"] != "system":
        with st.chat_message(message["role"]):
            st.markdown(message["content"])

# 4. Voice & Text Input
st.write("---")
voice_input = speech_to_text(language='en', start_prompt="🎤 Speak to MindEase", stop_prompt="🛑 Stop", just_once=True, key='STT')
user_input = voice_input if voice_input else st.chat_input("How are you feeling today?")

if user_input:
    st.session_state.messages.append({"role": "user", "content": user_input})
    with st.chat_message("user"):
        st.markdown(user_input)

    # 5. Response & Audio logic
    with st.chat_message("assistant"):
        response_placeholder = st.empty()
        full_response = ""
        
        try:
            completion = client.chat.completions.create(
                model="llama-3.3-70b-versatile",
                messages=st.session_state.messages,
                stream=True
            )

            for chunk in completion:
                content = chunk.choices[0].delta.content
                if content:
                    full_response += content
                    response_placeholder.markdown(full_response + "▌")
            
            response_placeholder.markdown(full_response)
            
            # Save audio and play
            if full_response:
                tts = gTTS(text=full_response, lang='en')
                tts.save("response.mp3")
                st.audio("response.mp3", format="audio/mp3", autoplay=True)
                st.session_state.messages.append({"role": "assistant", "content": full_response})

        except Exception as e:
            st.error(f"Error: {e}")