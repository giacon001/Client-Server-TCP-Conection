# Sistema de Comunicação Cliente-Servidor TCP em C

Sistema completo de comunicação via sockets TCP implementado em linguagem C, demonstrando troca de mensagens entre processos distintos com protocolo simples de comandos.

## 📋 Descrição

Este projeto implementa um sistema cliente-servidor utilizando sockets TCP para demonstrar comunicação de rede em C. O servidor processa comandos predefinidos e responde dinamicamente às requisições dos clientes, enquanto mantém logs detalhados de todas as operações.

## ✨ Funcionalidades

### Servidor
- ✅ Escuta conexões TCP na porta 5000
- ✅ Processa comandos predefinidos (TIME, STATUS, ECHO, EXIT)
- ✅ Responde dinamicamente às requisições
- ✅ Gerenciamento eficiente de conexões (single-threaded)
- ✅ Sistema de logging completo com timestamps
- ✅ Tratamento robusto de erros
- ✅ Encerramento gracioso com sinais (SIGINT, SIGTERM)

### Cliente
- ✅ Conexão TCP com o servidor
- ✅ Interface interativa para envio de comandos
- ✅ Exibição formatada das respostas
- ✅ Suporte a argumentos de linha de comando
- ✅ Comando de ajuda integrado
- ✅ Tratamento de erros de conexão

## 🚀 Comandos Suportados

| Comando | Descrição | Exemplo |
|---------|-----------|---------|
| `TIME` | Retorna o horário atual do servidor | `TIME` |
| `STATUS` | Indica o estado operacional do servidor | `STATUS` |
| `ECHO <mensagem>` | Retorna a mensagem enviada | `ECHO Olá, servidor!` |
| `EXIT` | Encerra a conexão com o servidor | `EXIT` |
| `HELP` | Exibe ajuda (apenas no cliente) | `HELP` |

## 🛠️ Compilação

### Pré-requisitos
- Compilador GCC
- Sistema Linux/Unix
- Make (opcional, mas recomendado)

### Compilar com Make

```bash
# Compilar servidor e cliente
make

# Compilar apenas o servidor
make server

# Compilar apenas o cliente
make client
```

### Compilar manualmente

```bash
# Compilar servidor
gcc -Wall -Wextra -std=c11 server.c -o server

# Compilar cliente
gcc -Wall -Wextra -std=c11 client.c -o client
```

## 📖 Uso

### Executar o Servidor

```bash
# Usando Make
make run-server

# Ou diretamente
./server
```

O servidor iniciará e ficará aguardando conexões na porta 5000.

### Executar o Cliente

```bash
# Conectar ao localhost na porta padrão (5000)
./client

# Conectar a um host específico
./client 192.168.1.100

# Conectar a um host e porta específicos
./client 192.168.1.100 5000

# Ver ajuda
./client --help
```

## 💡 Exemplos de Uso

### Exemplo 1: Cliente Interativo

```
$ ./client
Conectando ao servidor 127.0.0.1:5000...
Conectado com sucesso!

=== Modo Interativo ===
Digite 'HELP' para ver comandos disponíveis
Digite 'EXIT' para sair

Bem-vindo ao servidor TCP!
Comandos disponíveis: TIME, STATUS, ECHO <msg>, EXIT

> TIME
Resposta: Hora atual: 14:35:21

> STATUS
Resposta: Servidor operacional e pronto para receber comandos

> ECHO Teste de mensagem
Resposta: Teste de mensagem

> EXIT
Resposta: Conexão encerrada pelo cliente
Encerrando conexão...

Conexão encerrada.
```

### Exemplo 2: Log do Servidor (server.log)

```
[2025-10-20 14:35:15] ========================================
[2025-10-20 14:35:15] Servidor iniciando...
[2025-10-20 14:35:15] Servidor iniciado na porta 5000
[2025-10-20 14:35:18] Cliente conectado (127.0.0.1:54321)
[2025-10-20 14:35:21] Comando recebido: TIME
[2025-10-20 14:35:25] Comando recebido: STATUS
[2025-10-20 14:35:30] Comando recebido: ECHO Teste de mensagem
[2025-10-20 14:35:35] Comando recebido: EXIT
[2025-10-20 14:35:35] Conexão encerrada pelo comando EXIT
[2025-10-20 14:35:35] Conexão encerrada
```

## 🏗️ Arquitetura

### Servidor (server.c)
```
┌─────────────────────────────────┐
│       Inicialização             │
│  - Criar socket TCP             │
│  - Bind na porta 5000           │
│  - Configurar modo listening    │
│  - Abrir arquivo de log         │
└──────────────┬──────────────────┘
               │
               ▼
┌─────────────────────────────────┐
│     Loop Principal              │
│  - Aceitar conexões             │
│  - Processar comandos           │
│  - Enviar respostas             │
│  - Registrar logs               │
└──────────────┬──────────────────┘
               │
               ▼
┌─────────────────────────────────┐
│     Encerramento                │
│  - Fechar conexões              │
│  - Fechar socket                │
│  - Fechar arquivo de log        │
└─────────────────────────────────┘
```

### Cliente (client.c)
```
┌─────────────────────────────────┐
│       Inicialização             │
│  - Processar argumentos         │
│  - Criar socket TCP             │
│  - Conectar ao servidor         │
└──────────────┬──────────────────┘
               │
               ▼
┌─────────────────────────────────┐
│     Modo Interativo             │
│  - Ler comando do usuário       │
│  - Enviar ao servidor           │
│  - Receber resposta             │
│  - Exibir resposta              │
└──────────────┬──────────────────┘
               │
               ▼
┌─────────────────────────────────┐
│     Encerramento                │
│  - Fechar socket                │
│  - Limpar recursos              │
└─────────────────────────────────┘
```

## 🔧 Detalhes Técnicos

### Protocolo de Comunicação
- **Camada de Transporte:** TCP (Transmission Control Protocol)
- **Porta Padrão:** 5000
- **Formato de Mensagem:** Texto ASCII com terminador de linha
- **Tamanho do Buffer:** 1024 bytes

### Tratamento de Erros
- Validação de comandos desconhecidos
- Verificação de falhas de socket
- Tratamento de desconexões inesperadas
- Logging de todos os erros
- Encerramento gracioso com sinais

### Sistema de Logging
- Arquivo: `server.log`
- Formato: `[YYYY-MM-DD HH:MM:SS] mensagem`
- Registra: conexões, comandos, erros e desconexões
- Flush automático para garantir persistência

## 🧹 Limpeza

```bash
# Remover arquivos compilados
make clean

# Remover arquivos compilados e logs
make cleanall
```

## 📂 Estrutura do Projeto

```
Client-Server-TCP-Conection/
├── server.c               # Código fonte do servidor
├── client.c               # Código fonte do cliente
├── Makefile               # Script de compilação
├── README.md              # Documentação principal
├── QUICKSTART.md          # Guia de início rápido
├── RELATORIO_TECNICO.md   # Análise técnica detalhada
├── GUIA_TESTES.md         # Guia de testes
├── ESTRUTURA_VISUAL.md    # Estrutura visual do projeto
├── .gitignore             # Arquivos ignorados pelo git
└── server.log             # Log do servidor (gerado em tempo de execução)
```

## 🔒 Segurança

Este é um projeto educacional. Para uso em produção, considere:
- Autenticação de clientes
- Criptografia de comunicação (TLS/SSL)
- Limitação de taxa de requisições
- Validação mais rigorosa de entrada
- Controle de acesso baseado em IP

## 🧪 Testes

### Teste Básico
```bash
# Terminal 1: Iniciar servidor
./server

# Terminal 2: Executar cliente
./client
```

### Teste Automatizado
```bash
make test
```

## 🐛 Resolução de Problemas

### Porta já em uso
```bash
# Verificar processos usando a porta 5000
lsof -i :5000

# Matar processo se necessário
kill -9 <PID>
```

### Permissões do arquivo de log
```bash
# Dar permissões de escrita
chmod 644 server.log
```

### Erro de compilação
```bash
# Verificar versão do GCC
gcc --version

# Limpar e recompilar
make clean && make
```

## 📚 Conceitos Demonstrados

- ✅ Programação de sockets em C
- ✅ Protocolo TCP/IP
- ✅ Cliente-servidor architecture
- ✅ Manipulação de strings em C
- ✅ Tratamento de sinais (signals)
- ✅ Sistema de arquivos (file I/O)
- ✅ Gerenciamento de memória
- ✅ Tratamento de erros
- ✅ Logging e debugging

## 🎯 Possíveis Melhorias

- [ ] Suporte multi-threaded para múltiplos clientes simultâneos
- [ ] Interface gráfica (GUI)
- [ ] Criptografia de comunicação
- [ ] Mais comandos (UPTIME, INFO, etc.)
- [ ] Configuração via arquivo
- [ ] Autenticação de usuários
- [ ] Transferência de arquivos
- [ ] Compressão de dados

## 📝 Licença

Este projeto é de código aberto e está disponível para fins educacionais.

## 👤 Autor

Desenvolvido como projeto educacional para demonstração de comunicação cliente-servidor TCP em C.

---

**Nota:** Este projeto foi desenvolvido para fins educacionais e demonstra os conceitos fundamentais de programação de rede em C usando sockets TCP.