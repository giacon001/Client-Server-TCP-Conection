/*
 * Servidor TCP Socket em C
 * Implementa um servidor que escuta conexões TCP e processa comandos:
 * - TIME: Retorna horário atual do servidor
 * - STATUS: Indica estado do servidor
 * - ECHO <mensagem>: Retorna mensagem enviada
 * - EXIT: Encerra conexão
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>

#define PORT 5000
#define BUFFER_SIZE 1024
#define LOG_FILE "server.log"

// Variável global para o socket do servidor (para encerramento gracioso)
int server_socket = -1;
FILE *log_file = NULL;

/**
 * Escreve mensagem no arquivo de log com timestamp
 */
void write_log(const char *message) {
    if (log_file == NULL) {
        return;
    }
    
    time_t now;
    struct tm *time_info;
    char time_buffer[26];
    
    time(&now);
    time_info = localtime(&now);
    strftime(time_buffer, 26, "%Y-%m-%d %H:%M:%S", time_info);
    
    fprintf(log_file, "[%s] %s\n", time_buffer, message);
    fflush(log_file); // Garante que o log é escrito imediatamente
}

/**
 * Obtém horário atual formatado
 */
void get_current_time(char *buffer, size_t size) {
    time_t now;
    struct tm *time_info;
    
    time(&now);
    time_info = localtime(&now);
    strftime(buffer, size, "%H:%M:%S", time_info);
}

/**
 * Processa comando TIME
 */
void handle_time_command(char *response, size_t size) {
    char time_str[64];
    get_current_time(time_str, sizeof(time_str));
    snprintf(response, size, "Hora atual: %s", time_str);
    write_log("Comando recebido: TIME");
}

/**
 * Processa comando STATUS
 */
void handle_status_command(char *response, size_t size) {
    snprintf(response, size, "Servidor operacional e pronto para receber comandos");
    write_log("Comando recebido: STATUS");
}

/**
 * Processa comando ECHO
 */
void handle_echo_command(const char *message, char *response, size_t size) {
    snprintf(response, size, "%s", message);
    char log_msg[BUFFER_SIZE];
    snprintf(log_msg, sizeof(log_msg), "Comando recebido: ECHO %s", message);
    write_log(log_msg);
}

/**
 * Processa comando EXIT
 */
void handle_exit_command(char *response, size_t size) {
    snprintf(response, size, "Conexão encerrada pelo cliente");
    write_log("Comando recebido: EXIT");
}

/**
 * Processa comando desconhecido
 */
void handle_unknown_command(const char *command, char *response, size_t size) {
    snprintf(response, size, "Comando desconhecido: %s\nComandos válidos: TIME, STATUS, ECHO <msg>, EXIT", command);
    char log_msg[BUFFER_SIZE];
    snprintf(log_msg, sizeof(log_msg), "Comando desconhecido recebido: %s", command);
    write_log(log_msg);
}

/**
 * Processa comandos recebidos do cliente
 */
int process_command(const char *command, char *response, size_t size) {
    // Remove caracteres de nova linha
    char cmd_copy[BUFFER_SIZE];
    strncpy(cmd_copy, command, BUFFER_SIZE - 1);
    cmd_copy[BUFFER_SIZE - 1] = '\0';
    
    // Remove \n e \r
    cmd_copy[strcspn(cmd_copy, "\r\n")] = 0;
    
    if (strlen(cmd_copy) == 0) {
        snprintf(response, size, "Comando vazio");
        return 1;
    }
    
    // Processa comando TIME
    if (strcmp(cmd_copy, "TIME") == 0) {
        handle_time_command(response, size);
        return 1;
    }
    
    // Processa comando STATUS
    if (strcmp(cmd_copy, "STATUS") == 0) {
        handle_status_command(response, size);
        return 1;
    }
    
    // Processa comando EXIT
    if (strcmp(cmd_copy, "EXIT") == 0) {
        handle_exit_command(response, size);
        return 0; // Retorna 0 para indicar encerramento da conexão
    }
    
    // Processa comando ECHO
    if (strncmp(cmd_copy, "ECHO ", 5) == 0) {
        handle_echo_command(cmd_copy + 5, response, size);
        return 1;
    }
    
    // Comando desconhecido
    handle_unknown_command(cmd_copy, response, size);
    return 1;
}

/**
 * Manipula conexão com cliente individual
 */
void handle_client(int client_socket, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    char log_msg[BUFFER_SIZE];
    
    // Registra conexão do cliente
    char *client_ip = inet_ntoa(client_addr.sin_addr);
    snprintf(log_msg, sizeof(log_msg), "Cliente conectado (%s:%d)", 
             client_ip, ntohs(client_addr.sin_port));
    write_log(log_msg);
    printf("%s\n", log_msg);
    
    // Envia mensagem de boas-vindas
    const char *welcome = "Bem-vindo ao servidor TCP!\nComandos disponíveis: TIME, STATUS, ECHO <msg>, EXIT\n";
    send(client_socket, welcome, strlen(welcome), 0);
    
    // Loop de processamento de comandos
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        
        // Recebe comando do cliente
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                write_log("Cliente desconectou");
                printf("Cliente desconectou\n");
            } else {
                snprintf(log_msg, sizeof(log_msg), "Erro ao receber dados: %s", strerror(errno));
                write_log(log_msg);
                perror("Erro ao receber dados");
            }
            break;
        }
        
        buffer[bytes_received] = '\0';
        
        // Processa comando
        memset(response, 0, BUFFER_SIZE);
        int continue_connection = process_command(buffer, response, BUFFER_SIZE);
        
        // Envia resposta
        strcat(response, "\n");
        ssize_t bytes_sent = send(client_socket, response, strlen(response), 0);
        
        if (bytes_sent < 0) {
            snprintf(log_msg, sizeof(log_msg), "Erro ao enviar resposta: %s", strerror(errno));
            write_log(log_msg);
            perror("Erro ao enviar resposta");
            break;
        }
        
        // Se comando foi EXIT, encerra conexão
        if (!continue_connection) {
            write_log("Conexão encerrada pelo comando EXIT");
            printf("Conexão encerrada pelo comando EXIT\n");
            break;
        }
    }
    
    close(client_socket);
    write_log("Conexão encerrada");
}

/**
 * Handler para sinais (SIGINT, SIGTERM)
 */
void signal_handler(int signum) {
    printf("\nRecebido sinal %d. Encerrando servidor...\n", signum);
    write_log("Servidor encerrando por sinal");
    
    if (server_socket != -1) {
        close(server_socket);
    }
    
    if (log_file != NULL) {
        fclose(log_file);
    }
    
    exit(0);
}

/**
 * Função principal do servidor
 */
int main() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char log_msg[BUFFER_SIZE];
    
    // Configura handlers de sinal para encerramento gracioso
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Abre arquivo de log
    log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Erro ao abrir arquivo de log");
        return 1;
    }
    
    write_log("========================================");
    write_log("Servidor iniciando...");
    
    // Cria socket TCP
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        write_log("Erro ao criar socket");
        perror("Erro ao criar socket");
        fclose(log_file);
        return 1;
    }
    
    // Configura opção SO_REUSEADDR para permitir reutilização da porta
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        write_log("Erro ao configurar SO_REUSEADDR");
        perror("Erro ao configurar SO_REUSEADDR");
        close(server_socket);
        fclose(log_file);
        return 1;
    }
    
    // Configura endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Aceita conexões de qualquer interface
    server_addr.sin_port = htons(PORT);
    
    // Faz bind do socket à porta
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        snprintf(log_msg, sizeof(log_msg), "Erro ao fazer bind na porta %d: %s", PORT, strerror(errno));
        write_log(log_msg);
        perror("Erro ao fazer bind");
        close(server_socket);
        fclose(log_file);
        return 1;
    }
    
    // Coloca socket em modo de escuta
    if (listen(server_socket, 5) < 0) {
        write_log("Erro ao escutar conexões");
        perror("Erro ao escutar conexões");
        close(server_socket);
        fclose(log_file);
        return 1;
    }
    
    snprintf(log_msg, sizeof(log_msg), "Servidor iniciado na porta %d", PORT);
    write_log(log_msg);
    printf("%s\n", log_msg);
    printf("Aguardando conexões...\n");
    
    // Loop principal - aceita e processa conexões
    while (1) {
        client_addr_len = sizeof(client_addr);
        
        // Aceita conexão do cliente
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        
        if (client_socket < 0) {
            snprintf(log_msg, sizeof(log_msg), "Erro ao aceitar conexão: %s", strerror(errno));
            write_log(log_msg);
            perror("Erro ao aceitar conexão");
            continue;
        }
        
        // Processa cliente (implementação single-threaded)
        handle_client(client_socket, client_addr);
        
        printf("\nAguardando nova conexão...\n");
    }
    
    // Limpeza (nunca alcançado no loop infinito, mas mantido para completude)
    close(server_socket);
    write_log("Servidor encerrado");
    fclose(log_file);
    
    return 0;
}
