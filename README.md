# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
<a href= "https://www.fiap.com.br/"><img src="assets/logo-fiap.png" alt="FIAP - Faculdade de Informática e Admnistração Paulista" border="0" width=40% height=40%></a>
</p>

<br>

# CardioIA Conectada: IoT e Visualização de Dados para a Saúde Digital

## 👨‍🎓 Integrantes: 
- <a href="https://github.com/Vitor985-hub">Vitor Eiji</a>
- <a href="https://github.com/BPilecarte">Beatriz Pilecarte</a>
- <a href="https://github.com/yggdrasilGit">Francismar Alves</a>
- <a href="https://github.com/matheusbento04">Matheus Soares</a>
- <a href="https://github.com/AntonioBarros19">Antonio Barros</a>


## 👩‍🏫 Professores:
### Tutor(a) 
- <a href="https://www.linkedin.com/in/caique-nonato/">Caique Nonato</a>
### Coordenador(a)
- <a href="https://www.linkedin.com/in/andregodoichiovato/">André Godoi Chiochiovatto</a>


## 📜 Descrição

O projeto **CardioIA Conectada** é uma solução de saúde digital que integra IoT e Inteligência Artificial para o monitoramento contínuo de pacientes. O sistema utiliza sensores (simulados via ESP32 no Wokwi) para capturar batimentos cardíacos (BPM), temperatura corporal e detecção de movimento. 

Os dados são transmitidos via protocolo **MQTT** e orquestrados pelo **Node-RED**, que encaminha as informações para um backend desenvolvido em **FastAPI**. A API analisa os sinais vitais em tempo real e, caso identifique padrões de risco (como arritmias ou febre), dispara alertas automáticos por e-mail para os responsáveis. O projeto também conta com uma camada de análise de dados através de **Notebooks Jupyter**, onde são realizadas análises de séries temporais para identificar tendências e anomalias históricas nos sinais vitais dos pacientes.


## 📁 Estrutura de pastas

Abaixo está a organização detalhada dos diretórios e arquivos vitais do projeto:

```text
📦 CardioIA-Conectada
 ┣ 📂 CA_Rest/              # Backend da aplicação (FastAPI)
 ┃ ┗ 📜 api.py              # Script principal com a lógica de análise de risco e envio de e-mails
 ┣ 📂 node-red/             # Configurações de orquestração IoT
 ┃ ┗ 📜 flows (1).json      # Fluxo exportado do Node-RED para integração MQTT -> API REST
 ┣ 📂 notebooks/            # Análise de dados e Inteligência Artificial
 ┃ ┗ 📜 series_temporais.ipynb # Estudo estatístico e modelos (Regressão Logística e LIF) para classificação de ECG
 ┣ 📂 sensor/               # Firmware IoT para simulação no Wokwi (ESP32)
 ┃ ┣ 📜 sketch.ino          # Código C++ do dispositivo (leitura de sensores, MQTT e cache offline)
 ┃ ┣ 📜 diagram.json        # Esquemático de conexões dos sensores virtuais (Wokwi)
 ┃ ┗ 📜 libraries.txt       # Dependências de bibliotecas C++ para o simulador
 ┣ 📜 .env.exemple          # Exemplo de arquivo para configuração das credenciais de e-mail
 ┣ 📜 requirements.txt      # Lista de dependências Python para rodar a API e os notebooks
 ┗ 📜 README.md             # Documentação principal e instruções de uso do projeto
```

## 🔧 Como executar o código

### 1. Backend (API)
1. Certifique-se de ter o **Python 3.10+** instalado.
2. Na raiz do projeto, crie um ambiente virtual: `python -m venv venv` e ative-o.
3. Instale as dependências: `pip install -r requirements.txt`.
4. Para configurar as variáveis de ambiente, consulte o arquivo `.env.exemple`.
5. Inicie o servidor com o comando: `uvicorn CA_Rest.api:app --host 0.0.0.0 --port 8000`

### 2. Sensor (IoT)
1. Utilize o simulador **Wokwi** (ou a extensão do VS Code).
2. Abra o arquivo `sensor/sketch.ino`.
3. Certifique-se de que as bibliotecas `PubSubClient`, `DHTesp` e `WiFi` estão configuradas.
4. Execute a simulação. O sensor enviará dados para o broker `broker.hivemq.com` no tópico `cardioia/paciente001/sinais`.

### 3. Node-RED
1. Certifique-se de ter o **Node-RED** instalado em sua máquina (via npm: `npm install -g --unsafe-perm node-red`).
2. Inicie o Node-RED com o comando `node-red` e acesse o painel (geralmente em `http://localhost:1880`).
3. Importe o arquivo `node-red/flows (1).json` no seu Node-RED.
4. Certifique-se de que o nó MQTT está conectado ao mesmo broker do sensor.
5. Configure o nó HTTP Request para apontar para o endereço da API FastAPI (ex: `http://SEU_IP:8000/dados`).

### 4. Notebooks
1. Abra o arquivo `notebooks/series_temporais.ipynb` em um ambiente Jupyter (ou no próprio VS Code).
2. Execute as células para visualizar as análises de dados.

## 📸 Imagens do Projeto

Abaixo estão as imagens ilustrando o funcionamento do Sensor, o fluxo no Node-RED e o Dashboard da aplicação:

<div align="center">
  <img src="assets/Captura%20de%20tela%202026-05-07%20201045.png" alt="Imagem 1" width="80%">
  <br><br>
  <img src="assets/WhatsApp%20Image%202026-05-06%20at%2020.44.34.jpeg" alt="Imagem 2" width="80%">
  <br><br>
  <img src="assets/WhatsApp%20Image%202026-05-06%20at%2020.45.12.jpeg" alt="Imagem 3" width="80%">
</div>

## 🗃 Histórico de lançamentos

* 1.0.0 - 07/05/2026
    * Implementação da lógica de análise de risco na API.
    * Integração completa entre Sensor -> MQTT -> Node-RED -> FastAPI.
    * Adição de sistema de alertas via e-mail e persistência offline no sensor.
* 0.1.0 - 15/04/2026
    * Estrutura inicial do projeto e simulação básica de sensores.

## 📋 Licença

<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1"><p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><a property="dct:title" rel="cc:attributionURL" href="https://github.com/agodoi/template">MODELO GIT FIAP</a> por <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://fiap.com.br">Fiap</a> está licenciado sobre <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Attribution 4.0 International</a>.</p>

