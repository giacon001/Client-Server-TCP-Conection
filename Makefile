# Makefile para compilação do sistema cliente-servidor TCP

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic
LDFLAGS = 

# Alvos
SERVER = server
CLIENT = client

# Arquivos fonte
SERVER_SRC = server.c
CLIENT_SRC = client.c

# Alvos padrão
all: $(SERVER) $(CLIENT)

# Compilação do servidor
$(SERVER): $(SERVER_SRC)
	@echo "Compilando servidor..."
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER) $(LDFLAGS)
	@echo "Servidor compilado com sucesso!"

# Compilação do cliente
$(CLIENT): $(CLIENT_SRC)
	@echo "Compilando cliente..."
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT) $(LDFLAGS)
	@echo "Cliente compilado com sucesso!"

# Limpeza de arquivos compilados
clean:
	@echo "Removendo arquivos compilados..."
	rm -f $(SERVER) $(CLIENT)
	@echo "Limpeza concluída!"

# Limpeza completa (inclui logs)
cleanall: clean
	@echo "Removendo logs..."
	rm -f server.log
	@echo "Limpeza completa concluída!"

# Executar servidor
run-server: $(SERVER)
	@echo "Iniciando servidor..."
	./$(SERVER)

# Executar cliente
run-client: $(CLIENT)
	@echo "Iniciando cliente..."
	./$(CLIENT)

# Teste: executa servidor em background e cliente
test: all
	@echo "Iniciando teste..."
	@./$(SERVER) & SERVER_PID=$$!; \
	sleep 1; \
	echo "TIME" | ./$(CLIENT); \
	kill $$SERVER_PID 2>/dev/null || true
	@echo "Teste concluído!"

# Ajuda
help:
	@echo "Alvos disponíveis:"
	@echo "  all         - Compila servidor e cliente (padrão)"
	@echo "  server      - Compila apenas o servidor"
	@echo "  client      - Compila apenas o cliente"
	@echo "  clean       - Remove arquivos compilados"
	@echo "  cleanall    - Remove arquivos compilados e logs"
	@echo "  run-server  - Compila e executa o servidor"
	@echo "  run-client  - Compila e executa o cliente"
	@echo "  test        - Executa teste básico"
	@echo "  help        - Exibe esta mensagem"

.PHONY: all clean cleanall run-server run-client test help
