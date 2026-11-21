# 🤖 Kit Robô Explorer Deluxe — Controle IoT via Aplicativo (RoboCore Vespa)

Projeto de robótica e Internet das Coisas (IoT) desenvolvido para controlar remotamente via Bluetooth (smartphone) um robô móvel equipado com **sistema de locomoção** e **braço/garra articulada de 4 servos**, utilizando a placa **RoboCore Vespa**.

---

## 📋 Visão Geral

O sistema permite pilotar o robô e manipular o braço/garra em tempo real por meio de comandos seriais enviados pelo aplicativo no celular. O controle dos servomotores funciona de forma intuitiva, semelhante a um controle de videogame: **enquanto o botão está sendo pressionado, o servo se movimenta; ao soltar, ele para e mantém a posição atual**.

### Compatibilidade Inteligente de Aplicativos
O firmware identifica automaticamente dois padrões diferentes de envio de comandos dos aplicativos Bluetooth:
* **Apps com repetição contínua:** enquanto o botão é segurado, a letra é enviada repetidamente; quando o envio para, o servo interrompe o movimento automaticamente.
* **Apps com evento ao soltar (*Release*):** ao soltar o botão, o app envia `'0'` (para parar o servo) ou `'S'` (para parar tudo), interrompendo a ação instantaneamente.

---

## 🎮 Protocolo de Comandos utilizando o App BLE Controller (Bluetooth Serial)

### 1. Movimentação do Robô
| Comando | Ação |
| :---: | :--- |
| `↑` | Mover para **Frente** (*Forward*) |
| `↓` | Mover para **Trás** (*Backward*) |
| `←` | Virar para a **Esquerda** (*Left*) |
| `→` | Virar para a **Direita** (*Right*) |
| `☐` | **Parar** os motores (e também para os servos) |

### 2. Braço / Garra (Segure o botão para mover)
| Comando | Ação |
| :---: | :--- |
| `1` / `2` | **Servo 1** — sentido `+` / `-` |
| `3` / `4` | **Servo 2** — sentido `+` / `-` |
| `5` / `6` | **Servo 3** — sentido `+` / `-` |
| `7` / `8` | **Servo 4** — sentido `+` / `-` |
| `0` | **Para** o servo (configure no app para enviar `'0'` ao **soltar** o botão) |
| `P` | Retorna o braço/garra para a **posição inicial** |

---

## ⚙️ Hardware e Alimentação

* **Placa Controladora:** RoboCore Vespa (ESP32 com Bluetooth integrado)
* **Locomoção:** Motores DC do chassi Explorer
* **Manipulador:** Braço robótico / garra com 4 servomotores
* **Alimentação:** Bateria conectada à placa Vespa com a chave geral ligada

> ⚠️ **IMPORTANTE:** Na **RoboCore Vespa**, os servomotores **só recebem energia da BATERIA** (com a chave ligada). Conectando **apenas o cabo USB**, os servos **não** se mexem.

---

## 🚀 Como Configurar e Usar

1. **Alimentação:** Conecte a bateria carregada na RoboCore Vespa e **ligue a chave geral** da placa.
2. **Gravação do Código:** Abra o código-fonte na **Arduino IDE**, selecione a placa correspondente à RoboCore Vespa e faça o upload.
3. **Conexão Bluetooth:** Ative o Bluetooth do smartphone, pareie com o dispositivo da Vespa e conecte pelo aplicativo de controle serial/gamepad.
4. **Mapeamento no App:**
   * Configure os botões direcionais com `F`, `B`, `L`, `R` e `S`.
   * Configure os botões dos 4 servos com os pares `1/2`, `3/4`, `5/6` e `7/8`.
   * *(Recomendado)* Configure o evento de **soltar o botão** (*On Release*) dos servos para enviar o caractere `0`.
   * Configure um botão auxiliar com `P` para resetar a posição do braço/garra.
5. **Operação:** Pressione e segure os comandos para movimentar o chassi e posicionar a garra com precisão.

---

## 📁 Estrutura do Projeto

```text
├── codigo.ino/               # Código-fonte do firmware para RoboCore Vespa (.ino)
└── README.md          # Guia principal do projeto
