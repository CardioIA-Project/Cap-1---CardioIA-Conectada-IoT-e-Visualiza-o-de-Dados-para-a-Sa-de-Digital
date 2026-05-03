import os
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from dotenv import load_dotenv

load_dotenv()

remetente = os.getenv("EMAIL_USER")
senha = os.getenv("EMAIL_PASS")
destinatario = remetente  # envia para você mesmo

msg = MIMEMultipart()
msg["From"] = remetente
msg["To"] = destinatario
msg["Subject"] = "Teste de envio"
msg.attach(MIMEText("Este é um teste de envio via senha de app", "plain"))

try:
    with smtplib.SMTP("smtp.gmail.com", 587) as servidor:
        servidor.starttls()
        servidor.login(remetente, senha)
        servidor.send_message(msg)
    print("E-mail enviado com sucesso")
except Exception as e:
    print("Erro:", e)
