from fastapi import FastAPI
from pydantic import BaseModel
from datetime import datetime
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

app = FastAPI()

#Modelo de entrada de dados
class DadosSensor(BaseModel):
    bpm: int
    temperatura: float
    movimento: bool
    timestamp: datetime

#função para analise de risco
def analisar_risco(dados: DadosSensor):
    risco = "baixo"
    estado = "normal"

    if dados.bpm < 50 or dados.bpm > 120:
        risco = "alto"
        estado = "alerta cardiaco"

    elif dados.temperatura < 35 or dados.temperatura > 38:
        risco = "moderado"
        estado = "alteração térmica"

    elif dados.movimento is False:
        risco = "moderado"
        estado = "inatividade detectada"

    return risco, estado

def enviar_email(assunto: str, mensagem: str, destinatario: str):
    remetente = "seu_email@dominio.com"
    senha = "sua_senha"  # ideal usar variáveis de ambiente

    msg = MIMEMultipart()
    msg["From"] = remetente
    msg["To"] = destinatario
    msg["Subject"] = assunto

    msg.attach(MIMEText(mensagem, "plain"))

    try:
        with smtplib.SMTP("smtp.gmail.com", 587) as servidor:
            servidor.starttls()
            servidor.login(remetente, senha)
            servidor.send_message(msg)
        return "E-mail enviado com sucesso"
    except Exception as e:
        return f"Erro ao enviar e-mail: {e}"

#função para executar ação
def executar_acao(risco: str, estado: str):
    if risco == "alto":
        assunto = "⚠️ ALERTA CRÍTICO DETECTADO"
        mensagem = f"Foi detectado um risco ALTO: {estado}. Acione imediatamente a equipe responsavel."
        resultado = enviar_email(assunto, mensagem, [EMAIL_ADDRESS])
        return resultado
    
    elif risco == "moderado":
        return f"Ação preventiva: registrar alerta ({estado})"
    else:
        return f"Ação: continuar monitoramento"
