# Guia de Testes - Sistema Cliente-Servidor TCP

Este documento fornece um guia passo a passo para testar todas as funcionalidades do sistema.

---

## 🚀 Preparação

### 1. Compilar o Projeto
```bash
make
```

### 2. Verificar Compilação
```bash
ls -lh server client
```
Você deve ver dois executáveis: `server` e `client`.

---

## 🧪 Testes Básicos

### Teste 1: Iniciar Servidor

**Terminal 1:**
```bash
./server
```

**Resultado Esperado:**
```
Servidor iniciado na porta 5000
Aguardando conexões...
```

---

### Teste 2: Conectar Cliente

**Terminal 2:**
```bash
./client
```

**Resultado Esperado:**
```
Conectando ao servidor 127.0.0.1:5000...
Conectado com sucesso!

=== Modo Interativo ===
Digite 'HELP' para ver comandos disponíveis
Digite 'EXIT' para sair

Bem-vindo ao servidor TCP!
Comandos disponíveis: TIME, STATUS, ECHO <msg>, EXIT

> _
```

---

## 📝 Testes de Comandos

### Teste 3: Comando HELP

**Digite no cliente:**
```
HELP
```

**Resultado Esperado:**
```
=== COMANDOS DISPONÍVEIS ===
TIME          - Obtém horário atual do servidor
STATUS        - Verifica estado do servidor
ECHO <msg>    - Envia mensagem para eco
EXIT          - Encerra conexão
HELP          - Exibe esta mensagem
===========================
```

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 4: Comando TIME

**Digite no cliente:**
```
TIME
```

**Resultado Esperado:**
```
Resposta: Hora atual: HH:MM:SS
```
(onde HH:MM:SS é o horário atual do servidor)

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 5: Comando STATUS

**Digite no cliente:**
```
STATUS
```

**Resultado Esperado:**
```
Resposta: Servidor operacional e pronto para receber comandos
```

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 6: Comando ECHO (Simples)

**Digite no cliente:**
```
ECHO teste
```

**Resultado Esperado:**
```
Resposta: teste
```

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 7: Comando ECHO (Com Espaços)

**Digite no cliente:**
```
ECHO Esta é uma mensagem com vários espaços
```

**Resultado Esperado:**
```
Resposta: Esta é uma mensagem com vários espaços
```

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 8: Comando ECHO (Caracteres Especiais)

**Digite no cliente:**
```
ECHO !@#$%^&*()_+-={}[]|:;"'<>,.?/
```

**Resultado Esperado:**
```
Resposta: !@#$%^&*()_+-={}[]|:;"'<>,.?/
```

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 9: Comando Desconhecido

**Digite no cliente:**
```
INVALID_COMMAND
```

**Resultado Esperado:**
```
Resposta: Comando desconhecido: INVALID_COMMAND
Comandos válidos: TIME, STATUS, ECHO <msg>, EXIT
```

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 10: Comando Vazio

**Digite no cliente:**
```
(pressione apenas ENTER)
```

**Resultado Esperado:**
Nada acontece, o prompt retorna.

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 11: Comando EXIT

**Digite no cliente:**
```
EXIT
```

**Resultado Esperado (Cliente):**
```
Resposta: Conexão encerrada pelo cliente
Encerrando conexão...

Conexão encerrada.
```

**Resultado Esperado (Servidor):**
```
Conexão encerrada pelo comando EXIT

Aguardando nova conexão...
```

**Status:** ⬜ Passou  ⬜ Falhou

---

## 📊 Testes de Logging

### Teste 12: Verificar Arquivo de Log

**Comando:**
```bash
cat server.log
```

**Resultado Esperado:**
O log deve conter:
- Data/hora de inicialização
- Registro de conexão do cliente com IP e porta
- Todos os comandos recebidos
- Mensagem de encerramento de conexão

**Exemplo:**
```
[2025-10-20 14:30:00] ========================================
[2025-10-20 14:30:00] Servidor iniciando...
[2025-10-20 14:30:00] Servidor iniciado na porta 5000
[2025-10-20 14:30:15] Cliente conectado (127.0.0.1:54321)
[2025-10-20 14:30:18] Comando recebido: TIME
[2025-10-20 14:30:22] Comando recebido: STATUS
[2025-10-20 14:30:28] Comando recebido: ECHO teste
[2025-10-20 14:30:35] Comando desconhecido recebido: INVALID_COMMAND
[2025-10-20 14:30:40] Comando recebido: EXIT
[2025-10-20 14:30:40] Conexão encerrada pelo comando EXIT
[2025-10-20 14:30:40] Conexão encerrada
```

**Status:** ⬜ Passou  ⬜ Falhou

---

## 🔄 Testes de Conexão

### Teste 13: Múltiplas Conexões Sequenciais

**Procedimento:**
1. Conecte cliente 1, execute alguns comandos, EXIT
2. Conecte cliente 2, execute alguns comandos, EXIT
3. Conecte cliente 3, execute alguns comandos, EXIT

**Resultado Esperado:**
- Servidor aceita todas as conexões sequencialmente
- Cada conexão é registrada no log
- Servidor continua aguardando após cada desconexão

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 14: Desconexão Abrupta do Cliente

**Procedimento:**
1. Conecte um cliente
2. Pressione CTRL+C no terminal do cliente (sem usar EXIT)

**Resultado Esperado (Servidor):**
```
Cliente desconectou
Conexão encerrada

Aguardando nova conexão...
```

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 15: Encerramento Gracioso do Servidor (CTRL+C)

**Procedimento:**
1. Com servidor rodando (com ou sem cliente conectado)
2. Pressione CTRL+C no terminal do servidor

**Resultado Esperado:**
```
^C
Recebido sinal 2. Encerrando servidor...
```

O servidor deve encerrar e o log deve ser fechado corretamente.

**Status:** ⬜ Passou  ⬜ Falhou

---

## 🌐 Testes de Rede

### Teste 16: Conexão em Host Específico

**Comando:**
```bash
./client 127.0.0.1
```

**Resultado Esperado:**
Cliente conecta normalmente (mesmo resultado do teste 2).

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 17: Conexão em Host e Porta Específicos

**Comando:**
```bash
./client 127.0.0.1 5000
```

**Resultado Esperado:**
Cliente conecta normalmente.

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 18: Conexão com Servidor Não Disponível

**Procedimento:**
1. Certifique-se de que o servidor NÃO está rodando
2. Tente conectar o cliente

**Comando:**
```bash
./client
```

**Resultado Esperado:**
```
Conectando ao servidor 127.0.0.1:5000...
Erro ao conectar ao servidor: Connection refused
```

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 19: Porta Inválida

**Comando:**
```bash
./client 127.0.0.1 99999
```

**Resultado Esperado:**
```
Porta inválida: 99999
Uso: ./client [host] [porta]
```

**Status:** ⬜ Passou  ⬜ Falhou

---

## 🎯 Testes de Performance

### Teste 20: Sequência Rápida de Comandos

**Procedimento:**
Execute rapidamente (copie e cole):
```
TIME
STATUS
ECHO teste1
TIME
STATUS
ECHO teste2
TIME
STATUS
ECHO teste3
```

**Resultado Esperado:**
Todos os comandos devem ser processados corretamente sem erros.

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 21: Comando ECHO com Mensagem Longa

**Digite no cliente:**
```
ECHO Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.
```

**Resultado Esperado:**
A mensagem completa deve ser ecoada de volta.

**Status:** ⬜ Passou  ⬜ Falhou

---

## 🛠️ Testes de Compilação

### Teste 22: Make Clean

**Comando:**
```bash
make clean
ls server client 2>/dev/null
```

**Resultado Esperado:**
```
Removendo arquivos compilados...
rm -f server client
Limpeza concluída!
ls: cannot access 'server': No such file or directory
ls: cannot access 'client': No such file or directory
```

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 23: Make Cleanall

**Comando:**
```bash
make cleanall
ls server.log 2>/dev/null
```

**Resultado Esperado:**
```
Removendo arquivos compilados...
...
Removendo logs...
rm -f server.log
Limpeza completa concluída!
ls: cannot access 'server.log': No such file or directory
```

**Status:** ⬜ Passou  ⬜ Falhou

---

### Teste 24: Recompilação

**Comando:**
```bash
make clean
make
```

**Resultado Esperado:**
Compilação bem-sucedida sem erros ou warnings.

**Status:** ⬜ Passou  ⬜ Falhou

---

## ✅ Resumo de Testes

| Categoria | Testes | Passou | Falhou |
|-----------|--------|--------|--------|
| Básicos | 2 | __ | __ |
| Comandos | 9 | __ | __ |
| Logging | 1 | __ | __ |
| Conexão | 3 | __ | __ |
| Rede | 4 | __ | __ |
| Performance | 2 | __ | __ |
| Compilação | 3 | __ | __ |
| **TOTAL** | **24** | **__** | **__** |

---

## 📝 Notas

- Marque ✅ em "Passou" se o teste foi bem-sucedido
- Marque ❌ em "Falhou" se o teste não passou
- Anote quaisquer observações ou problemas encontrados

---

## 🐛 Resolução de Problemas

### Problema: "Address already in use"
**Solução:**
```bash
lsof -i :5000
kill -9 <PID>
```

### Problema: "Permission denied" ao executar
**Solução:**
```bash
chmod +x server client
```

### Problema: Log não é criado
**Solução:**
Verifique permissões do diretório:
```bash
ls -ld .
chmod 755 .
```

---

**Data dos Testes:** ______________  
**Testador:** ______________  
**Resultado Final:** ⬜ Todos passaram  ⬜ Alguns falharam  ⬜ Muitos falharam
