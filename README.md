# 🤖 Kit Robô Explorer Deluxe — Controle IoT via Aplicativo (RoboCore Vespa)

Projeto de robótica e Internet das Coisas (IoT) para controlar pelo celular, via Bluetooth, um robô móvel com **sistema de locomoção** e **braço/garra de 4 servos**, usando a placa **RoboCore Vespa**.

| | |
| :--- | :--- |
| **Instituição** | UniSenac Pelotas |
| **Disciplina** | Internet das Coisas — 6º semestre |
| **Integrantes** | Renata Bueno, Janine Veigas Farias, Miguel Vencato e Kauã Santos Das Neves |

---

## 📋 Visão Geral

Pelo aplicativo, o robô anda para frente e para trás, faz curvas e usa o braço para pegar e levantar objetos. Cada botão do app envia **um caractere** pelo Bluetooth, e a Vespa executa a ação correspondente.

O braço funciona **por cliques**: cada clique move o servo cerca de **30°**, de forma suave, e ele **para e mantém a posição**. Para mover mais, basta clicar de novo.

### Por que por cliques?
O firmware também está preparado para o modo "segurar para mover", como num controle de videogame. Ele reconhece dois tipos de app:
* **Apps com repetição contínua:** o app repete a letra enquanto o botão está pressionado, e o servo para quando o envio para.
* **Apps com evento ao soltar (*On Release*):** o app envia `0` ou `S` quando o botão é solto, e o servo para na hora.

No teste com o **BLE Controller**, esse modo **não funcionou**: o app envia o comando só uma vez por toque e não avisa quando o botão é solto. Por isso, com esse app, o braço é controlado por cliques. Com um app que tenha uma dessas duas opções, o modo segurar funciona sem mudar o código.

---

## 🎮 Comandos do App

Cada botão do app deve ser configurado para enviar o caractere da coluna **Envia**.

### 1. Movimentação do Robô
| Botão | Envia | Ação |
| :---: | :---: | :--- |
| `↑` | `F` | Mover para **frente** |
| `↓` | `B` | Mover para **trás** |
| `←` | `L` | Virar para a **esquerda** |
| `→` | `R` | Virar para a **direita** |
| `☐` | `S` | **Parar** os motores e os servos |

### 2. Braço / Garra (cerca de 30° por clique)
| Envia | Ação |
| :---: | :--- |
| `1` / `2` | **Servo 1** — sentido `+` / `-` |
| `3` / `4` | **Servo 2** — sentido `+` / `-` |
| `5` / `6` | **Servo 3** — sentido `+` / `-` |
| `7` / `8` | **Servo 4** — sentido `+` / `-` |
| `P` | Volta o braço/garra para a **posição inicial** (90°) |
| `0` | **Para** o servo (só para apps com evento ao soltar) |

---

## ⚙️ Hardware e Software

| Item | Função |
| :--- | :--- |
| **RoboCore Vespa** (ESP32 com Bluetooth) | Controla motores e servos e recebe os comandos |
| **Motores DC** do chassi Explorer | Locomoção |
| **4 servomotores** (S1 a S4) | Braço e garra |
| **Bateria** + chave geral da Vespa | Alimenta motores e servos |
| **Arduino IDE** + pacote de placas ESP32 | Programação |
| Biblioteca **RoboCore - Vespa** | Controle dos motores (`VespaMotors`) e servos (`VespaServo`) |
| Biblioteca **BluetoothSerial** (já vem com o ESP32) | Comunicação Bluetooth com o celular |

> ⚠️ **IMPORTANTE:** Na RoboCore Vespa, os servomotores **só recebem energia da bateria** (com a chave ligada). Conectando **apenas o cabo USB**, os servos **não** se mexem.

---

## 🚀 Como Configurar e Usar

1. **Biblioteca:** na Arduino IDE, instale a biblioteca **RoboCore - Vespa** pelo Gerenciador de Bibliotecas.
2. **Gravação do código:** abra `codigo/codigo.ino`, selecione a placa da Vespa e a porta e faça o upload.
3. **Alimentação:** conecte a bateria carregada e **ligue a chave geral**. Quando o LED acender, o robô está pronto.
4. **Conexão Bluetooth:** pareie o celular com o dispositivo **`VESPA_BT`** e conecte pelo aplicativo.
5. **Mapeamento no app:**
   * Botões direcionais: `F`, `B`, `L`, `R` e `S`.
   * Botões dos servos: pares `1/2`, `3/4`, `5/6` e `7/8`.
   * Um botão com `P` para resetar a posição do braço/garra.
   * *(Opcional)* Se o app tiver a opção, configure o evento de **soltar** dos botões dos servos para enviar `0` e use o modo segurar.
6. **Operação:** use as setas para mover o robô e clique nos botões do braço até posicionar a garra.

### 🔍 Teste rápido
Abra o **Monitor Serial** em **115200**. Cada botão do app deve aparecer como `Recebido: <caractere>`, seguido dos ângulos atuais (`S1: 90 | S2: 90 | ...`).

---

## 🔧 Ajustes no Código

Todas as configurações ficam no início do código:

| Constante | Valor | O que faz |
| :--- | :---: | :--- |
| `VELOCIDADE` | `80` | Velocidade dos motores (0 a 100) |
| `MS_POR_GRAU` | `8` | Velocidade dos servos: menor = mais rápido |
| `TEMPO_SEM_REPETICAO` | `250` | Quanto o servo anda por clique (em ms); aumente para mover mais por clique |
| `ANG_MIN` / `ANG_MAX` | `0` / `180` | Limites de cada servo, para a garra não forçar a estrutura |
| `ANG_INICIAL` | `90` | Posição ao ligar e ao receber `P` |

---

## 🛠️ Problemas Comuns

| Problema | Solução |
| :--- | :--- |
| Robô anda, mas os servos não mexem | Ligue a bateria e a chave da Vespa; só a USB não alimenta os servos |
| Segurar o botão não move o servo continuamente | O app não avisa quando o botão é solto; use cliques ou aumente `TEMPO_SEM_REPETICAO` |
| Placa reinicia quando a garra pega peso | Use a bateria carregada e mantenha o objeto perto da base do braço |
| Garra sem força para levantar o objeto | Troque por servo com engrenagem de metal (MG90S ou MG996R) |
| App não encontra o robô | O código usa Bluetooth clássico; procure no app a opção clássica/HC-05 e pareie com `VESPA_BT` |

---

## 📁 Estrutura do Projeto

```text
├── codigo/
│   └── codigo.ino     # Firmware da RoboCore Vespa
└── README.md          # Guia principal do projeto
```
