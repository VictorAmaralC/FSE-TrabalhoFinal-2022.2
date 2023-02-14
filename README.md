# Código ESP32 - Trabalho 3 2022-2

Código da ESP32 integrado ao ThingsBoard para rodar o Trabalho 3 de 2022-2


**Disciplina**: FUNDAMENTOS DE SISTEMAS EMBARCADOS <br>


## Alunos

| Matrícula  | Aluno                          |
| ---------- | ------------------------------ |
| 17/0103200 | Gabriel Alves Hussein          |
| 17/0164411 | Victor Amaral Cerqueira        |

## Sobre

O projeto consiste na implementação de sensores e atuadores distribuídos baseados na ESP32. A ESP32 está integrada ao Thingsboard e utilizará widgets em um dashboard para acompanhamento das leituras de temperatura e umidades feitas, assim como controle do led RGB, ativação do Buzzer e leitura do sensor de chama KY-26. As leituras de temperatura e umidade são realizadas com a inscrição em tópicos via RPC e receberá dados em formato JSON com a temperatura e umidade percebidas pelo sensor, há no total 4 tópicos de envio de dados, são eles:

- Temperatura: `v1/devices/me/telemetry`
- Umidade: `v1/devices/me/telemetry`
- Alarme: `v1/devices/me/telemetry`
- Chama: `v1/devices/me/attributes`

Ao total da implementação temos os seguintes componentes implementados:

- Conexão da placa Wifi via MQTT 
- Sensor DHT11 de temperatura e umidade
- LED RGB 
- Buzzer 5V 
- Sensor de chama KY-26

## Instalação

**Linguagens**: C

- A build e configuração do projeto é feita utilizando uma extensão do VSCode que automatiza esse processo chamada de PlatformIO

- Ao configurar o ambiente primeiramente deve-se importar o projeto através da extensão

- Configurar o wi-fi de acordo com a rede em que está conectado deve-se abrir os arquivos Kconfig.projbuild e sdkconfig.esp32doit-devkit-v1

- Preencher os campos de acordo com a rede desejada.

- Após essa configuração utilizar a extensão para criar uma imagem do projeto e fazer o upload da mesma para a ESP32.

**obs. para propósitos de testagem alterar na main.c o valor de temperatura (overallTemperature) nas linhas 130 e 134**


## Uso

Ao executar o programa é necessário acessar o Dashboard "Gabriel-Victor" no Thingsboard é posível acompanhar as leituras de:

- Temperatura (Média e Tempo Real)
- Umidade (Média e Tempo Real)
- Sensor de Chama
- Ativação do alarme (Temperatura atingindo a margem e chama ativada)
- Teste do Buzzer alternando sua intensidade
