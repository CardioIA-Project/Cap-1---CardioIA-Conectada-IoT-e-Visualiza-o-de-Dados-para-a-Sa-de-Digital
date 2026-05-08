# Relatório Comparativo: Regressão Logística vs. Modelo Neuromórfico (LIF)

## 1. Introdução

A análise de séries temporais de sinais vitais, como o Eletrocardiograma (ECG), representa um desafio para algoritmos de Inteligência Artificial devido à natureza contínua e sequencial dos dados. Neste relatório, avaliamos o desempenho de duas abordagens distintas para a classificação de batimentos cardíacos (Normal vs. Taquicardia): 
1. **Regressão Logística:** Um modelo clássico de Machine Learning.
2. **Modelo LIF (*Leaky Integrate-and-Fire*):** Um modelo baseado em *Spiking Neural Networks* (Redes Neuromórficas).

A tarefa consistiu em identificar padrões de arritmia (taquicardia) usando sinais sintéticos simulados no projeto *CardioIA Conectada*.

---

## 2. Abordagem Clássica: Regressão Logística

A Regressão Logística é um modelo estatístico robusto para classificação binária. No entanto, por ser um classificador pontual, ele não lida naturalmente com séries temporais brutas (onde cada amostra temporal é uma feature, o que causaria a maldição da dimensionalidade e perda da ordem temporal).

**Processamento:**
Foi necessário o uso de *Feature Engineering* (Extração de Características). Transformamos a janela temporal do ECG em variáveis estatísticas:
- **Média**, **Desvio Padrão**, **Valor Máximo** e **Variância**.
Em seguida, padronizamos as métricas usando o `StandardScaler` do `scikit-learn`.

**Vantagens:**
1. **Interpretabilidade:** Fácil de interpretar os coeficientes e a influência de cada estatística na decisão.
2. **Tempo de treinamento:** O treinamento é praticamente instantâneo devido à sua simplicidade algébrica.
3. **Maturidade:** Ferramental de suporte vasto no ecossistema Python (ex: scikit-learn).

**Limitações:**
1. **Perda da Dinâmica Temporal:** O modelo perde informações de transição de estado. O que importa é apenas o "resumo estatístico" da série, e não como o sinal variou ao longo do tempo.
2. **Complexidade de Pré-processamento:** Depende do ser humano identificar e extrair as melhores *features* antes do modelo realizar a inferência.

---

## 3. Abordagem Neuromórfica: Modelo LIF

O modelo LIF (*Leaky Integrate-and-Fire*) é uma unidade fundamental das *Spiking Neural Networks*. Diferente de modelos de Deep Learning convencionais, os SNNs simulam o funcionamento biológico do neurônio, disparando "picos" (*spikes*) quando o potencial de membrana atinge um limiar.

**Processamento:**
A série temporal do ECG foi passada sequencialmente para o neurônio. O potencial de membrana ($V$) sofria decaimento contínuo ("leak"), mas subia rapidamente nos picos (complexo QRS). A contagem final de disparos (*spikes*) serviu como um indicador da frequência cardíaca, permitindo uma separação linear simples através de um limiar (`threshold`).

**Vantagens:**
1. **Memória de Estado Física:** Lida nativamente com o tempo. A variável de membrana possui a "memória" física da entrada passada, sem necessidade de redes recorrentes densas ou extração de features estatísticas.
2. **Eficiência Energética (em Hardware):** Em chips neuromórficos (como o Loihi da Intel), essas redes consomem ordens de grandeza menos energia que modelos clássicos, ideais para o processamento "Edge/IoT" em sensores biológicos vestíveis.
3. **Ausência de Extração de Features Manuais:** O algoritmo atua diretamente no dado bruto, mitigando o processamento computacional prévio.

**Limitações:**
1. **Tuning de Hiperparâmetros Biológicos:** Variáveis como $\tau$ (constante de tempo de vazamento) e o *threshold* precisam ser precisamente ajustadas e requerem heurísticas baseadas em conhecimento físico/biológico do sinal.
2. **Treinamento Complexo:** O treinamento por Backpropagation em redes LIF profundas é desafiador devido à não-diferenciabilidade dos *spikes*.

---

## 4. Análise de Desempenho e Conclusões

Com base na experimentação registrada no Notebook Jupyter (`series_temporais.ipynb`), observamos:

* **Desempenho da Regressão Logística:**
  Através das *features* estatísticas extraídas, o modelo logístico apresentou altíssima precisão. Isso demonstra que para séries onde a perturbação da anomalia é clara (como aumento de picos refletindo na variância e média), a abordagem clássica é muito eficiente na inferência tradicional em backend.

* **Desempenho do Modelo LIF:**
  A separabilidade das classes usando apenas a contagem de disparos neuromórficos foi igualmente precisa. A diferença fundamental está na lógica de operação: o LIF simulou o que um hardware no **sensor edge (ESP32)** poderia fazer localmente antes de acionar a nuvem.

### Veredito Final

Ambas as abordagens se mostraram competentes, contudo o cenário de emprego ditará o "melhor" modelo. 
A **Regressão Logística** continua recomendável para análises em lote (batch processing) ou processos rodando na nuvem, devido ao baixo custo de desenvolvimento. O **Modelo LIF (SNN)**, por sua vez, demonstra grande viabilidade para um futuro de Edge AI na saúde, onde o modelo rodaria em *wearables* locais utilizando micropotência elétrica para prever episódios cardíacos em tempo real sem depender continuamente de comunicação com servidores potentes.
