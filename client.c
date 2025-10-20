/*
 * Cliente TCP Socket em C
 * Conecta-se a um servidor TCP e permite envio interativo de comandos:
 * - TIME: Solicita horário atual do servidor
 * - STATUS: Verifica estado do servidor
 * - ECHO <mensagem>: Envia mensagem para eco
 * - EXIT: Encerra conexão
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 5000
#define DEFAULT_HOST "127.0.0.1"

/**
 * Exibe menu de ajuda com comandos disponíveis
 */
void print_help() {
    printf("\n=== COMANDOS DISPONÍVEIS ===\n");
    printf("TIME          - Obtém horário atual do servidor\n");
    printf("STATUS        - Verifica estado do servidor\n");
    printf("ECHO <msg>    - Envia mensagem para eco\n");
    printf("EXIT          - Encerra conexão\n");
    printf("HELP          - Exibe esta mensagem\n");
    printf("===========================\n\n");
}

/**
 * Remove espaços em branco do início e fim da string
 */
void trim(char *str) {
    if (str == NULL || strlen(str) == 0) {
        return;
    }
    
    // Remove espaços do início
    char *start = str;
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r') {
        start++;
    }
    
    // Remove espaços do fim
    char *end = str + strlen(str) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    
    // Copia string trimada
    size_t len = end - start + 1;
    memmove(str, start, len);
    str[len] = '\0';
}

/**
 * Envia comando ao servidor e recebe resposta
 */
int send_command(int socket_fd, const char *command, char *response, size_t response_size) {
    // Envia comando
    ssize_t bytes_sent = send(socket_fd, command, strlen(command), 0);
    if (bytes_sent < 0) {
        perror("Erro ao enviar comando");
        return -1;
    }
    
    // Recebe resposta
    memset(response, 0, response_size);
    ssize_t bytes_received = recv(socket_fd, response, response_size - 1, 0);
    
    if (bytes_received < 0) {
        perror("Erro ao receber resposta");
        return -1;
    } else if (bytes_received == 0) {
        printf("Servidor encerrou a conexão\n");
        return 0;
    }
    
    response[bytes_received] = '\0';
    return bytes_received;
}

/**
 * Loop interativo para comunicação com servidor
 */
void interactive_mode(int socket_fd) {
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    char command[BUFFER_SIZE];
    
    printf("\n=== Modo Interativo ===\n");
    printf("Digite 'HELP' para ver comandos disponíveis\n");
    printf("Digite 'EXIT' para sair\n\n");
    
    // Recebe mensagem de boas-vindas do servidor
    memset(response, 0, BUFFER_SIZE);
    ssize_t bytes_received = recv(socket_fd, response, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        response[bytes_received] = '\0';
        printf("%s\n", response);
    }
    
    // Loop principal de interação
    while (1) {
        printf("> ");
        fflush(stdout);
        
        // Lê comando do usuário
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        
        // Copia e processa comando
        strncpy(command, buffer, BUFFER_SIZE - 1);
        command[BUFFER_SIZE - 1] = '\0';
        trim(command);
        
        // Verifica comando vazio
        if (strlen(command) == 0) {
            continue;
        }
        
        // Comando HELP local (não enviado ao servidor)
        if (strcasecmp(command, "HELP") == 0) {
            print_help();
            continue;
        }
        
        // Adiciona quebra de linha ao comando
        strcat(command, "\n");
        
        // Envia comando e recebe resposta
        int result = send_command(socket_fd, command, response, BUFFER_SIZE);
        
        if (result <= 0) {
            break;
        }
        
        // Exibe resposta
        printf("Resposta: %s\n", response);
        
        // Verifica se foi comando EXIT
        if (strncasecmp(buffer, "EXIT", 4) == 0) {
            printf("Encerrando conexão...\n");
            break;
        }
    }
}

/**
 * Conecta ao servidor
 */
int connect_to_server(const char *host, int port) {
    int socket_fd;
    struct sockaddr_in server_addr;
    
    // Cria socket TCP
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Erro ao criar socket");
        return -1;
    }
    
    // Configura endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    // Converte endereço IP
    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        perror("Endereço IP inválido");
        close(socket_fd);
        return -1;
    }
    
    // Conecta ao servidor
    printf("Conectando ao servidor %s:%d...\n", host, port);
    
    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao conectar ao servidor");
        close(socket_fd);
        return -1;
    }
    
    printf("Conectado com sucesso!\n");
    return socket_fd;
}

/**
 * Função principal do cliente
 */
int main(int argc, char *argv[]) {
    char host[256] = DEFAULT_HOST;
    int port = DEFAULT_PORT;
    
    // Processa argumentos da linha de comando
    if (argc >= 2) {
        strncpy(host, argv[1], sizeof(host) - 1);
        host[sizeof(host) - 1] = '\0';
    }
    
    if (argc >= 3) {
        port = atoi(argv[2]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Porta inválida: %s\n", argv[2]);
            fprintf(stderr, "Uso: %s [host] [porta]\n", argv[0]);
            return 1;
        }
    }
    
    // Exibe informações de uso se solicitado
    if (argc >= 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        printf("Uso: %s [host] [porta]\n", argv[0]);
        printf("  host: Endereço IP do servidor (padrão: %s)\n", DEFAULT_HOST);
        printf("  porta: Porta do servidor (padrão: %d)\n", DEFAULT_PORT);
        printf("\nExemplo: %s 192.168.1.100 5000\n", argv[0]);
        return 0;
    }
    
    // Conecta ao servidor
    int socket_fd = connect_to_server(host, port);
    if (socket_fd < 0) {
        return 1;
    }
    
    // Entra em modo interativo
    interactive_mode(socket_fd);
    
    // Fecha conexão
    close(socket_fd);
    printf("\nConexão encerrada.\n");
    
    return 0;
}
