#!/bin/bash

# Script de Demonstração do Sistema Cliente-Servidor TCP
# Este script demonstra o funcionamento do sistema com exemplos práticos

echo "=========================================="
echo "  DEMONSTRAÇÃO: Sistema Cliente-Servidor"
echo "=========================================="
echo ""

# Verifica se os executáveis existem
if [ ! -f "./server" ] || [ ! -f "./client" ]; then
    echo "❌ Executáveis não encontrados!"
    echo "Execute 'make' para compilar o projeto primeiro."
    exit 1
fi

echo "✅ Executáveis encontrados!"
echo ""

# Inicia o servidor em background
echo "🚀 Iniciando servidor na porta 5000..."
./server &
SERVER_PID=$!
echo "   PID do servidor: $SERVER_PID"
echo ""

# Aguarda servidor inicializar
sleep 2

# Verifica se servidor está rodando
if ! ps -p $SERVER_PID > /dev/null; then
    echo "❌ Erro: Servidor não iniciou corretamente!"
    exit 1
fi

echo "✅ Servidor iniciado com sucesso!"
echo ""
echo "=========================================="
echo "  TESTE 1: Comando TIME"
echo "=========================================="
echo ""
echo "Enviando comando: TIME"
echo "TIME" | nc localhost 5000 2>/dev/null | tail -1
sleep 1

echo ""
echo "=========================================="
echo "  TESTE 2: Comando STATUS"
echo "=========================================="
echo ""
echo "Enviando comando: STATUS"
echo "STATUS" | nc localhost 5000 2>/dev/null | tail -1
sleep 1

echo ""
echo "=========================================="
echo "  TESTE 3: Comando ECHO"
echo "=========================================="
echo ""
echo "Enviando comando: ECHO Olá, Servidor!"
echo "ECHO Olá, Servidor!" | nc localhost 5000 2>/dev/null | tail -1
sleep 1

echo ""
echo "=========================================="
echo "  TESTE 4: Comando Desconhecido"
echo "=========================================="
echo ""
echo "Enviando comando: INVALID"
echo "INVALID" | nc localhost 5000 2>/dev/null | tail -1
sleep 1

echo ""
echo "=========================================="
echo "  VISUALIZANDO LOG DO SERVIDOR"
echo "=========================================="
echo ""

if [ -f "server.log" ]; then
    echo "📋 Últimas 15 linhas do server.log:"
    echo ""
    tail -15 server.log
else
    echo "⚠️  Arquivo server.log não encontrado"
fi

echo ""
echo "=========================================="
echo "  ENCERRANDO DEMONSTRAÇÃO"
echo "=========================================="
echo ""

# Encerra servidor
echo "🛑 Encerrando servidor..."
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null

echo "✅ Servidor encerrado!"
echo ""
echo "=========================================="
echo "  DEMONSTRAÇÃO CONCLUÍDA"
echo "=========================================="
echo ""
echo "📝 Para uso interativo, execute:"
echo "   Terminal 1: ./server"
echo "   Terminal 2: ./client"
echo ""
echo "📚 Para mais informações, veja:"
echo "   - README.md"
echo "   - RELATORIO_TECNICO.md"
echo ""
