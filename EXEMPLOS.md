# Exemplos de Uso - Sistema Cliente-Servidor TCP

Este documento contém exemplos práticos de uso do sistema cliente-servidor TCP.

---

## 📖 Índice

1. [Uso Básico](#uso-básico)
2. [Exemplos Interativos](#exemplos-interativos)
3. [Uso Avançado](#uso-avançado)
4. [Scripts de Automação](#scripts-de-automação)
5. [Integração com Outras Ferramentas](#integração-com-outras-ferramentas)

---

## 🎯 Uso Básico

### Exemplo 1: Primeira Execução

**Passo 1 - Compilar:**
```bash
cd /caminho/para/Client-Server-TCP-Conection
make
```

**Passo 2 - Abrir dois terminais:**

**Terminal 1 (Servidor):**
```bash
./server
```
Saída:
```
[2025-10-20 15:00:00] ========================================
[2025-10-20 15:00:00] Servidor iniciando...
[2025-10-20 15:00:00] Servidor iniciado na porta 5000
Servidor iniciado na porta 5000
Aguardando conexões...
```

**Terminal 2 (Cliente):**
```bash
./client
```
Saída:
```
Conectando ao servidor 127.0.0.1:5000...
Conectado com sucesso!

=== Modo Interativo ===
Digite 'HELP' para ver comandos disponíveis
Digite 'EXIT' para sair

Bem-vindo ao servidor TCP!
Comandos disponíveis: TIME, STATUS, ECHO <msg>, EXIT

> 
```

---

## 💬 Exemplos Interativos

### Exemplo 2: Consultar Horário do Servidor

```bash
> TIME
Resposta: Hora atual: 15:05:23
```

### Exemplo 3: Verificar Status do Servidor

```bash
> STATUS
Resposta: Servidor operacional e pronto para receber comandos
```

### Exemplo 4: Ecoar Mensagens Simples

```bash
> ECHO Olá
Resposta: Olá

> ECHO Teste
Resposta: Teste
```

### Exemplo 5: Ecoar Mensagens com Espaços

```bash
> ECHO Esta é uma mensagem completa
Resposta: Esta é uma mensagem completa
```

### Exemplo 6: Ecoar Números e Caracteres Especiais

```bash
> ECHO 123456789
Resposta: 123456789

> ECHO !@#$%^&*()
Resposta: !@#$%^&*()
```

### Exemplo 7: Comando de Ajuda

```bash
> HELP

=== COMANDOS DISPONÍVEIS ===
TIME          - Obtém horário atual do servidor
STATUS        - Verifica estado do servidor
ECHO <msg>    - Envia mensagem para eco
EXIT          - Encerra conexão
HELP          - Exibe esta mensagem
===========================
```

### Exemplo 8: Testar Comando Inválido

```bash
> INVALID
Resposta: Comando desconhecido: INVALID
Comandos válidos: TIME, STATUS, ECHO <msg>, EXIT
```

### Exemplo 9: Encerrar Conexão

```bash
> EXIT
Resposta: Conexão encerrada pelo cliente
Encerrando conexão...

Conexão encerrada.
```

---

## 🚀 Uso Avançado

### Exemplo 10: Conectar a Servidor Remoto

```bash
# Conectar a servidor em outro host
./client 192.168.1.100

# Conectar a servidor em porta diferente
./client 192.168.1.100 5001
```

### Exemplo 11: Executar Comandos via Pipe

```bash
# Executar um único comando
echo "TIME" | ./client

# Executar múltiplos comandos
(echo "TIME"; sleep 1; echo "STATUS"; sleep 1; echo "ECHO teste"; sleep 1; echo "EXIT") | ./client
```

### Exemplo 12: Salvar Saída em Arquivo

```bash
# Salvar todas as interações
./client > output.txt 2>&1

# Ou interativamente e salvando ao mesmo tempo
./client | tee client_session.log
```

### Exemplo 13: Usar Netcat para Teste Rápido

```bash
# Enviar comando único
echo "TIME" | nc localhost 5000

# Sessão interativa com netcat
nc localhost 5000
```

---

## 🤖 Scripts de Automação

### Exemplo 14: Script de Teste Automatizado

**Arquivo: test_commands.sh**
```bash
#!/bin/bash

echo "=== Teste Automatizado do Servidor ==="
echo ""

# Array de comandos para testar
commands=(
    "TIME"
    "STATUS"
    "ECHO Teste automatizado"
    "ECHO 123456"
    "ECHO !@#$%"
)

# Executa cada comando
for cmd in "${commands[@]}"; do
    echo "Executando: $cmd"
    echo "$cmd" | nc -w 1 localhost 5000
    echo ""
    sleep 1
done

echo "=== Teste Concluído ==="
```

**Uso:**
```bash
chmod +x test_commands.sh
./test_commands.sh
```

### Exemplo 15: Script de Monitoramento

**Arquivo: monitor.sh**
```bash
#!/bin/bash

echo "=== Monitoramento do Servidor ==="
echo ""

while true; do
    clear
    echo "=========================================="
    echo "  STATUS DO SERVIDOR - $(date)"
    echo "=========================================="
    echo ""
    
    # Verifica se servidor está rodando
    if pgrep -x "server" > /dev/null; then
        echo "✅ Servidor: RODANDO"
        echo "   PID: $(pgrep -x server)"
    else
        echo "❌ Servidor: NÃO ESTÁ RODANDO"
    fi
    
    echo ""
    echo "=========================================="
    echo "  ÚLTIMAS LINHAS DO LOG"
    echo "=========================================="
    
    if [ -f "server.log" ]; then
        tail -10 server.log
    else
        echo "⚠️  Log não encontrado"
    fi
    
    echo ""
    echo "Pressione CTRL+C para sair..."
    sleep 5
done
```

**Uso:**
```bash
chmod +x monitor.sh
./monitor.sh
```

### Exemplo 16: Script de Benchmark

**Arquivo: benchmark.sh**
```bash
#!/bin/bash

NUM_REQUESTS=100
echo "=== Benchmark: $NUM_REQUESTS requisições ==="
echo ""

start_time=$(date +%s.%N)

for i in $(seq 1 $NUM_REQUESTS); do
    echo "TIME" | nc -w 1 localhost 5000 > /dev/null 2>&1
done

end_time=$(date +%s.%N)
elapsed=$(echo "$end_time - $start_time" | bc)
rps=$(echo "$NUM_REQUESTS / $elapsed" | bc -l)

echo "Requisições: $NUM_REQUESTS"
echo "Tempo total: $elapsed segundos"
echo "Requisições/segundo: $(printf '%.2f' $rps)"
```

**Uso:**
```bash
chmod +x benchmark.sh
./benchmark.sh
```

---

## 🔗 Integração com Outras Ferramentas

### Exemplo 17: Integração com cURL (via HTTP Wrapper)

Se você criar um wrapper HTTP para o servidor TCP, pode usar com curl:

```bash
# Exemplo conceitual (requer implementação adicional)
curl http://localhost:8080/tcp?cmd=TIME
curl http://localhost:8080/tcp?cmd=STATUS
```

### Exemplo 18: Integração com Python

**Arquivo: client.py**
```python
#!/usr/bin/env python3

import socket

def send_command(host, port, command):
    """Envia comando ao servidor TCP e retorna resposta"""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        
        # Recebe mensagem de boas-vindas
        welcome = s.recv(1024).decode('utf-8')
        print(f"Servidor: {welcome}")
        
        # Envia comando
        s.sendall(f"{command}\n".encode('utf-8'))
        
        # Recebe resposta
        response = s.recv(1024).decode('utf-8')
        return response.strip()

if __name__ == "__main__":
    host = "127.0.0.1"
    port = 5000
    
    # Testa comandos
    print("TIME:", send_command(host, port, "TIME"))
    print("STATUS:", send_command(host, port, "STATUS"))
    print("ECHO:", send_command(host, port, "ECHO Python Client"))
```

**Uso:**
```bash
chmod +x client.py
python3 client.py
```

### Exemplo 19: Integração com Node.js

**Arquivo: client.js**
```javascript
#!/usr/bin/env node

const net = require('net');

function sendCommand(host, port, command) {
    return new Promise((resolve, reject) => {
        const client = new net.Socket();
        
        client.connect(port, host, () => {
            console.log(`Conectado a ${host}:${port}`);
        });
        
        client.on('data', (data) => {
            const response = data.toString();
            
            if (response.includes('Bem-vindo')) {
                // Recebeu mensagem de boas-vindas, envia comando
                client.write(`${command}\n`);
            } else {
                // Recebeu resposta do comando
                console.log('Resposta:', response.trim());
                client.destroy();
                resolve(response);
            }
        });
        
        client.on('error', (err) => {
            reject(err);
        });
        
        client.on('close', () => {
            console.log('Conexão encerrada');
        });
    });
}

// Uso
(async () => {
    try {
        await sendCommand('127.0.0.1', 5000, 'TIME');
        await sendCommand('127.0.0.1', 5000, 'STATUS');
        await sendCommand('127.0.0.1', 5000, 'ECHO Node.js Client');
    } catch (err) {
        console.error('Erro:', err);
    }
})();
```

**Uso:**
```bash
chmod +x client.js
node client.js
```

### Exemplo 20: Logging Customizado

**Análise de logs com comandos Unix:**

```bash
# Contar número de conexões
grep "Cliente conectado" server.log | wc -l

# Listar todos os comandos TIME recebidos
grep "Comando recebido: TIME" server.log

# Contar comandos por tipo
echo "TIME: $(grep -c 'Comando recebido: TIME' server.log)"
echo "STATUS: $(grep -c 'Comando recebido: STATUS' server.log)"
echo "ECHO: $(grep -c 'Comando recebido: ECHO' server.log)"
echo "EXIT: $(grep -c 'Comando recebido: EXIT' server.log)"

# Ver horários de pico de conexões
grep "Cliente conectado" server.log | cut -d' ' -f2 | cut -d: -f1-2 | sort | uniq -c

# Últimas 20 operações
tail -20 server.log

# Log em tempo real (similar a tail -f)
tail -f server.log
```

---

## 🎮 Casos de Uso Práticos

### Exemplo 21: Sistema de Monitoramento Simples

Use o servidor para monitorar um sistema:

```bash
# Terminal 1: Servidor rodando

# Terminal 2: Script de monitoramento
while true; do
    echo "TIME" | nc -w 1 localhost 5000
    sleep 60  # A cada minuto
done
```

### Exemplo 22: Teste de Conectividade

```bash
#!/bin/bash
# Script para verificar se servidor está respondendo

if echo "STATUS" | nc -w 2 localhost 5000 | grep -q "operacional"; then
    echo "✅ Servidor está respondendo corretamente"
    exit 0
else
    echo "❌ Servidor não está respondendo"
    exit 1
fi
```

### Exemplo 23: Sistema de Notificações

```bash
#!/bin/bash
# Envia notificação ao servidor

notify_server() {
    local message="$1"
    echo "ECHO [NOTIFICAÇÃO] $message" | nc -w 1 localhost 5000
}

# Uso
notify_server "Backup concluído"
notify_server "Deploy realizado"
notify_server "Sistema reiniciado"
```

---

## 🔧 Dicas e Truques

### Dica 1: Usar Alias para Comandos Frequentes

Adicione ao seu `~/.bashrc`:

```bash
alias tcp-server='cd ~/Client-Server-TCP-Conection && ./server'
alias tcp-client='cd ~/Client-Server-TCP-Conection && ./client'
alias tcp-log='tail -f ~/Client-Server-TCP-Conection/server.log'
```

### Dica 2: Executar Servidor como Serviço

**Arquivo: tcp-server.service** (systemd)
```ini
[Unit]
Description=TCP Socket Server
After=network.target

[Service]
Type=simple
User=seu-usuario
WorkingDirectory=/caminho/para/Client-Server-TCP-Conection
ExecStart=/caminho/para/Client-Server-TCP-Conection/server
Restart=always

[Install]
WantedBy=multi-user.target
```

### Dica 3: Usar tmux para Gerenciar Servidor

```bash
# Criar sessão tmux para servidor
tmux new-session -d -s tcp-server './server'

# Conectar à sessão
tmux attach -t tcp-server

# Desconectar (servidor continua rodando): CTRL+B, D

# Matar sessão
tmux kill-session -t tcp-server
```

---

## 📊 Exemplos de Análise de Dados

### Exemplo 24: Estatísticas de Uso

```bash
#!/bin/bash
# Gera estatísticas de uso do servidor

echo "=== ESTATÍSTICAS DO SERVIDOR ==="
echo ""
echo "Total de conexões: $(grep -c 'Cliente conectado' server.log)"
echo "Total de desconexões: $(grep -c 'Conexão encerrada' server.log)"
echo ""
echo "Comandos executados:"
echo "  TIME:   $(grep -c 'Comando recebido: TIME' server.log)"
echo "  STATUS: $(grep -c 'Comando recebido: STATUS' server.log)"
echo "  ECHO:   $(grep -c 'Comando recebido: ECHO' server.log)"
echo "  EXIT:   $(grep -c 'Comando recebido: EXIT' server.log)"
echo ""
echo "Comandos desconhecidos: $(grep -c 'Comando desconhecido' server.log)"
echo ""
echo "Primeira conexão: $(grep -m 1 'Cliente conectado' server.log | cut -d'[' -f2 | cut -d']' -f1)"
echo "Última conexão: $(grep 'Cliente conectado' server.log | tail -1 | cut -d'[' -f2 | cut -d']' -f1)"
```

---

## 🎓 Exercícios Práticos

### Exercício 1: Sessão Completa
Pratique uma sessão completa:
1. Inicie o servidor
2. Conecte com cliente
3. Execute todos os comandos (TIME, STATUS, ECHO, HELP)
4. Encerre com EXIT
5. Verifique o log

### Exercício 2: Múltiplas Conexões
Pratique múltiplas conexões:
1. Inicie o servidor
2. Conecte cliente 1, execute comandos, saia
3. Conecte cliente 2, execute comandos, saia
4. Conecte cliente 3, execute comandos, saia
5. Analise o log para ver todas as conexões

### Exercício 3: Automação
Crie seu próprio script que:
1. Conecte ao servidor
2. Execute uma sequência de comandos
3. Salve os resultados
4. Gere um relatório

---

## 📞 Suporte

Para mais informações, consulte:
- `README.md` - Documentação geral
- `RELATORIO_TECNICO.md` - Detalhes técnicos
- `GUIA_TESTES.md` - Guia de testes

---

**Última atualização:** 20 de outubro de 2025
