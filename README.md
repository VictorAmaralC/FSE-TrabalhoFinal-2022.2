# Código ESP32 - Trabalho 3 2022-2

Código da ESP32 integrado ao ThingsBoard para rodar o Trabalho 3 de 2022-2


**Disciplina**: FUNDAMENTOS DE SISTEMAS EMBARCADOS <br>


## Alunos

| Matrícula  | Aluno                          |
| ---------- | ------------------------------ |
| 17/0103200 | Gabriel Alves Hussein          |
| 17/0164411 | Victor Amaral Cerqueira        |

## Sobre

O projeto consiste na implementação de sensores e atuadores distribuídos baseados na ESP32. A ESP32 está integrada ao Thingsboard e utilizará widgets em um dashboard para acompanhamento das leituras de temperatura e umidades feitas, assim como controle do led RGB e ativação do Buzzer. As leituras de temperatura e umidade são realizadas com a inscrição em tópicos via RPC e receberá dados em formato JSON com a temperatura e umidade percebidas pelo sensor, há no total dois tópicos de envio de dados, são eles:

- Temperatura: `v1/devices/me/telemetry`
- Umidade: `v1/devices/me/attributes`

Ao total da implementação temos os seguintes componentes implementados:

- Conexão da placa Wifi via MQTT 
- Leitura de temperature e umidade via sensor DHT11
- LED RGB de controle via Thingsboard
- Buzzer 5V para emitir sinais sonoros

## Instalação

**Linguagens**: C<br>

Para a build e configuração do projeto utilizamos uma extensão do VSCode que automatiza esse processo chamada de PlatformIO

Para configurar o ambiente primeiramente deve-se importar o projeto através da extensão

Para configurar o wi-fi de acordo com a rede em que está conectado deve-se abrir o arquivo Kconfig.projbuild e preencher os campos de acordo com a rede desejada.

Após essa configuração utilizar a extensão para criar uma imagem do projeto e fazer o upload da mesma para a ESP32.


## Uso

Ao executar o programa é necessário acessar o Dashboard "Gabriel-Victor" no Thingsboard e utilizar o widget para acompanhar as leituras de temperatura/umidade e controle do LED RGB.
