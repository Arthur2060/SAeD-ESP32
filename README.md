# SAeD-ESP32 | Sistema de Armazenamento e Despache

Um sistema completo de automação para almoxarifado baseado em um **carrinho AGV (Autonomous Ground Vehicle)** com **braço robótico acoplado**, capaz de movimentar-se em malhas quadriculadas, identificar e transportar itens por cor.

## 📋 Visão Geral

SAeD (Sistema de Armazenamento e Despache) é uma solução integrada que combina:

- **Robótica Móvel**: Carrinho AGV autônomo com navegação por malha quadriculada
- **Robótica de Manipulação**: Braço RoboARM da RoboCore para captura e posicionamento de itens
- **Visão Computacional**: Detecção e diferenciação de objetos por cor
- **Localização e Mapeamento**: Sistema de bússola digital e mapeamento de ambiente
- **Controle de Locomoção**: Gerenciamento avançado de motores com PID

## 🎯 Funcionalidades Principais

- ✅ Navegação autônoma em ambientes estruturados (malha quadriculada)
- ✅ Detecção de cores para identificação de itens
- ✅ Movimentação precisa do braço robótico
- ✅ Mapeamento e localização em tempo real
- ✅ Sistema de distribuição por áreas específicas
- ✅ Interface de controle e monitoramento
- ✅ Integração com sensores múltiplos (ultrassom, bússola, encoders)

## 🔧 Hardware

### Componentes Principais

| Componente                | Especificação                       |
| ------------------------- | ----------------------------------- |
| **Microcontrolador**      | ESP32 S3 (ESP32 Dev Module)         |
| **Braço Robótico**        | RoboARM (RoboCore)                  |
| **Controlador do Braço**  | Placa Vespa                         |
| **Sensores de Distância** | Ultrassônico                        |
| **Bússola Digital**       | Sensor QMC5883L                     |
| **Odometria**             | Encoders rotacionais (ESP32Encoder) |
| **Detecção de Cor**       | Sensor de cor                       |

### Bibliotecas Utilizadas

```
- Adafruit Unified Sensor (sensores diversos)
- ArduinoJson (processamento de dados JSON)
- ESP32Encoder (leitura de encoders)
- QuickPID (controle PID de motores)
- QMC5883LCompass (bússola digital)
```

## 📁 Estrutura do Projeto

```
SAeD-ESP32/
├── include/
│   ├── mpu6050.h           # Definições do sensor IMU
│   └── README
├── src/
│   ├── main.cpp            # Ponto de entrada
│   ├── controll/           # Controle de movimento e navegação
│   │   ├── Motores.cpp/h   # Controle de motores
│   │   ├── Demarcacao.cpp/h # Calibração e demarcação de áreas
│   │   ├── MapManager.cpp/h # Gerenciamento do mapa e posição
│   │   └── Radar.cpp/h     # Sistema de detecção com ultrassom
│   ├── model/              # Modelos de dispositivos e sensores
│   │   ├── Bussola.cpp/h   # Bússola digital (QMC5883L)
│   │   ├── Claw.cpp/h      # Controle da garra/braço robótico
│   │   ├── ColorDetect.cpp/h # Detecção de cores
│   │   └── Ultrasom.cpp/h  # Sensor ultrassônico
│   └── view/               # Interface e monitoramento
│       ├── Principal.cpp/h # Interface principal
│       └── ...
├── lib/
│   └── README
├── test/
│   └── README
├── platformio.ini          # Configuração do PlatformIO
└── README.md               # Este arquivo
```

## 🚀 Instalação e Configuração

### Pré-requisitos

- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE for VSCode](https://platformio.org/install/ide?install=vscode)

### Passos de Instalação

1. **Clone o repositório** (se aplicável)

   ```bash
   git clone <seu-repositorio>
   cd SAeD-ESP32
   ```

2. **Abra em VS Code com PlatformIO**
   - Abra a pasta do projeto no VS Code
   - O PlatformIO reconhecerá o projeto automaticamente

3. **Instale as dependências**

   ```bash
   platformio lib install
   ```

4. **Conecte o ESP32** via USB

## 📝 Compilação e Upload

### Compilar o código

```bash
platformio run
```

### Fazer upload para o ESP32

```bash
platformio run --target upload
```

### Monitorar saída serial

```bash
platformio device monitor
```

Ou use os botões do PlatformIO IDE no VS Code:

- ✔️ Build
- ➤ Upload
- 🔌 Serial Monitor

## 🎮 Controle e Operação

### Inicialização

1. Certifique-se de que o carrinho está em uma posição inicial conhecida
2. Execute a demarcação da malha (calibração)
3. O sistema identificará a posição inicial e estará pronto para operação

### Operação Básica

- **Movimento**: O carrinho navega autonomamente através da malha quadriculada
- **Detecção**: Identifica objetos por cor usando o sensor de cores
- **Coleta**: O braço RoboARM coleta itens usando a garra
- **Distribuição**: Leva os itens para suas áreas específicas conforme a cor detectada

## 🔌 Conexões e Pinagem

As configurações de pinos estão definidas nos arquivos de componentes:

- **Motores**: Configurados em `src/controll/Motores.h`
- **Sensores**: Configurados em `src/model/`
- **Garra/Braço**: Configurado em `src/model/Claw.h`

## 🐛 Troubleshooting

### Problema: ESP32 não é detectado

- Verifique a conexão USB
- Instale os drivers CH340 ou CP2102 conforme o modelo do ESP32
- Reinicie o VS Code e o PlatformIO

### Problema: Erro de compilação

- Limpe o build: `platformio run --target clean`
- Verifique se todas as bibliotecas foram instaladas
- Consulte os logs de erro do PlatformIO

### Problema: Movimento erraticamente

- Calibre a bússola digital (QMC5883L)
- Verifique a calibração dos motores em `Demarcacao.cpp`
- Ajuste os parâmetros PID em `Motores.cpp`

## 📊 Monitoramento e Debug

Use a interface Serial (ou Telemetria) para:

- Visualizar posição atual do carrinho
- Monitorar leituras de sensores
- Verificar status do braço robótico
- Executar testes de componentes individuais

## 🤝 Contribuindo

Para reportar bugs ou sugerir melhorias:

1. Crie uma issue descrevendo o problema
2. Faça um fork do repositório
3. Crie uma branch para sua feature (`git checkout -b feature/AmazingFeature`)
4. Commit suas mudanças (`git commit -m 'Add some AmazingFeature'`)
5. Push para a branch (`git push origin feature/AmazingFeature`)
6. Abra um Pull Request

## 📄 Licença

Este projeto é licenciado sob a MIT License - veja o arquivo LICENSE para detalhes.

## 👥 Autores

- Equipe de Desenvolvimento SAeD

## 📞 Suporte

Para dúvidas ou suporte técnico, abra uma issue no repositório ou entre em contato com a equipe de desenvolvimento.

---

**Status do Projeto**: ✅ Ativo e em desenvolvimento

**Última Atualização**: Junho de 2026
