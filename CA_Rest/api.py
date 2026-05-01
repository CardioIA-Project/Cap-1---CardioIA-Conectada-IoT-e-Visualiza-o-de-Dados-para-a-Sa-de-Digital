from fastapi import FastAPI
from pydantic import BaseModel
from datetime import datetime
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

app = FastAPI()

#Modelo de entrada de dados
class Dados_Sensor(BaseModel):
    bpm: int
    temperatura: float
    movimento: bool
    timestamp: datetime

#variavel global para controle de estado
ultimo_risco = "normal"

#função para analise de risco
def analisar_risco(dados: Dados_Sensor):
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

#função para enviar email
def enviar_email(assunto: str, mensagem: str, destinatario: str):
    remetente = "seu_email@gmail.com" #usar variaveis de ambiente
    senha = "sua_senha" #usar variaveis de ambiente

    msg = MIMEMultipart()
    msg['From'] = remetente
    msg['to'] = destinatario
    msg['subject'] = assunto
    msg.attach(MIMEText(mensagem, 'plain'))

    try:
        with smtplib.SMTP('smtp.gmail.com', 587) as servidor:
            servidor.starttls()
            servidor.login(remetente, senha)
            servidor.send_message(msg)
            return "email enviado com sucesso"
    except Exception as e:
        return f'Erro ao enviar email: {str(e)}'

#função para executar a lógica de transição
def executar_acao(risco: str, estado: str):
    global ultimo_risco
    resultado = "Nenhuma ação necessaria"

    if risco != ultimo_risco:
        if risco == "alto":
            assunto = "⚠️ ALERTA CRÍTICO DETECTADO"
            mensagem = f"Foi detectado um risco ALTO: {estado}. Acione imediatamente a equipe responsável."
            resultado = enviar_email(assunto, mensagem, "destinatario@dominio.com")
        elif risco == 'moderado':
            assunto = '⚠️ ALERTA MODERADO DETECTADO'
            mensagem = f'Foi detectado um risco MODERADO: {estado}. Resgistara alerta preditivo.'
            resultado = enviar_email(assunto, mensagem, "destinatario@dominio.com")
        else:
            resultado = 'ação: continuar monitoramento'
    #atualiza o ultimo risco
    ultimo_risco = risco
    return resultado 

#endpoint principal
@app.post('/dados')
async def receber_dados(dados: Dados_Sensor):
    risco, estado = analisar_risco(dados)
    acao = executar_acao(risco, estado)
    return{
        'mensagem': 'dados processados com sucesso',
        'dados': dados,
        'risco': risco,
        'estado': estado,
        'acao': acao
    }
