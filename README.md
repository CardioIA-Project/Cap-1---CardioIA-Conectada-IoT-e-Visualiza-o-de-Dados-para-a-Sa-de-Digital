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

Dentre os arquivos e pastas presentes na raiz do projeto, definem-se:

- <b>CA_Rest</b>: Contém o backend da aplicação desenvolvido em FastAPI. Inclui a lógica de análise de risco e o serviço de disparo de e-mails.
- <b>node-red</b>: Armazena os fluxos (JSON) utilizados no Node-RED para a integração entre o broker MQTT e a API REST.
- <b>notebooks</b>: Contém os notebooks Jupyter com análises estatísticas e estudos de séries temporais dos dados coletados.
- <b>sensor</b>: Código-fonte do firmware (C++/Arduino) para o ESP32, incluindo a simulação de sensores e lógica de cache offline.
- <b>requirements.txt</b>: Arquivo contendo todas as dependências Python necessárias para o projeto.
- <b>.env</b>: Arquivo para armazenamento de variáveis de ambiente sensíveis (credenciais de e-mail).
- <b>README.md</b>: Este arquivo, contendo a documentação geral do projeto.

## 🔧 Como executar o código

### 1. Backend (API)
1. Certifique-se de ter o **Python 3.10+** instalado.
2. Navegue até a pasta `CA_Rest`.
3. Crie um ambiente virtual: `python -m venv venv` e ative-o.
4. Instale as dependências: `pip install -r ../requirements.txt`.
5. Configure o arquivo `.env` na raiz do projeto com as chaves `EMAIL_USER` e `EMAIL_PASS`.
6. Inicie o servidor: `uvicorn api:app --reload`.

### 2. Sensor (IoT)
1. Utilize o simulador **Wokwi** (ou a extensão do VS Code).
2. Abra o arquivo `sensor/sketch.ino`.
3. Certifique-se de que as bibliotecas `PubSubClient`, `DHTesp` e `WiFi` estão configuradas.
4. Execute a simulação. O sensor enviará dados para o broker `broker.hivemq.com` no tópico `cardioia/paciente001/sinais`.

### 3. Node-RED
1. Importe o arquivo `node-red/flows (1).json` no seu Node-RED.
2. Certifique-se de que o nó MQTT está conectado ao mesmo broker do sensor.
3. Configure o nó HTTP Request para apontar para o endereço da API FastAPI (ex: `http://localhost:8000/dados`).

### 4. Notebooks
1. Abra o arquivo `notebooks/series_temporais.ipynb` em um ambiente Jupyter (ou no próprio VS Code).
2. Execute as células para visualizar as análises de dados.

## 🗃 Histórico de lançamentos

* 1.0.0 - 07/05/2026
    * Implementação da lógica de análise de risco na API.
    * Integração completa entre Sensor -> MQTT -> Node-RED -> FastAPI.
    * Adição de sistema de alertas via e-mail e persistência offline no sensor.
* 0.1.0 - 15/04/2026
    * Estrutura inicial do projeto e simulação básica de sensores.

## 📋 Licença

<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1"><p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><a property="dct:title" rel="cc:attributionURL" href="https://github.com/agodoi/template">MODELO GIT FIAP</a> por <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://fiap.com.br">Fiap</a> está licenciado sobre <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Attribution 4.0 International</a>.</p>

