// Server side implementation of UDP client-server model

#include "common.h"
#include <pthread.h>
#include <semaphore.h>

/// the socket queue
struct sock_queue_t {
  int* sockfd;
  struct sock_queue_t* next;
};

/// the shared data between all threads
typedef struct {
  int thread_count;
  // int position;
  struct sock_queue_t* sock_queue;
  // This mutex is used to guard the sock_queue
  pthread_mutex_t can_acess_sock_queue; 
} shared_data_t;

/// the private data of each thread
typedef struct  {
  int thread_number;
  shared_data_t* shared_data;
} private_data_t;

int receive_autentication_data(int* sockfd, struct sockaddr_in* cliaddr);
int receive_file_from_client(int* sockfd, struct sockaddr_in* cliaddr);
int check_autentication();
void decode_message(char *buffer, char *output_filename);
void write_in_file(char *output_filename, char *text);
int start_up_server(int* sockfd, struct sockaddr_in* servaddr);
void delete_server(int* sockfd, struct sockaddr_in* servaddr);
/// Consumer threads
int create_threads(shared_data_t* shared_data);
void* main_thread(void* data);
void* sock_thread(void* data);
void insert_to_sock_queue(struct sock_queue_t* head, int* sockfd);

// VARIABLE GLOBAL. MR. JEISSON GET DOWN!
//char* server_string;

// Driver code
int main(int argc, char* argv[])
{
    int error = EXIT_SUCCESS;
    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
    if (shared_data) {
        shared_data->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
        shared_data->sock_queue = NULL;
        if (argc == 2) {
            if (sscanf(argv[1], "%d", &shared_data->thread_count) != 1 || errno) {
                fprintf(stderr, "error: invalid thread count\n");
                error = 1;
            }
        }
        if (error == EXIT_SUCCESS) {
            printf("Threads: %ld\n\n",shared_data->thread_count);
            error = pthread_mutex_init(&shared_data->can_acess_sock_queue, /*attr*/ NULL);
            if (shared_data->can_acess_sock_queue) {
                // CODE --------------------------------------------------
                error = create_threads(shared_data);
                // CODE END ----------------------------------------------
                pthread_mutex_destroy(&shared_data->can_acess_sock_queue);
            } else {
                fprintf(stderr, "error: could not allocate mutex\n");
                error = 3;
            }
            free(shared_data);
        } else {
          fprintf(stderr, "error: could not allocate shared memory\n");
          error = 2;
        }
    }
    return error;
}

int create_threads(shared_data_t* shared_data) {
    assert(shared_data);
    int error = EXIT_SUCCESS;
    // main thread
    pthread_t* main = (pthread_t*)calloc(1, sizeof(pthread_t));
    private_data_t* private_data_main = (private_data_t*)calloc(1, sizeof(private_data_t));
    private_data_main->thread_number = -1; // -1 for main thread
    private_data_main->shared_data = shared_data;
    int current_thread = 0;
    if (pthread_create(&main, /*attr*/ NULL, main_thread, &private_data_main[current_thread]) != EXIT_SUCCESS) {
        fprintf(stderr, "error: could not create main thread\n");
        return error = 20;
    }
    // secondary threads
    pthread_t* threads = (pthread_t*)calloc(shared_data->thread_count, sizeof(pthread_t));
    private_data_t* private_data = (private_data_t*)calloc(shared_data->thread_count, sizeof(private_data_t));
    if (threads && private_data) {
        for (size_t current_thread = 0; current_thread < shared_data->thread_count; ++current_thread) {
            private_data[current_thread]->thread_number = current_thread;
            private_data[current_thread]->shared_data = shared_data;
            if (pthread_create(&threads[current_thread], /*attr*/ NULL, sock_thread, &private_data[current_thread]) != EXIT_SUCCESS) {
                fprintf(stderr,
                "error: could not create thread %zu\n", current_thread);
                return error = 21;
            }
        }
        for (size_t i = 0; i < shared_data->thread_count; ++i) {
            pthread_join(threads[i], /*value_ptr*/ NULL);
        }
        free(threads);
        free(private_data);
    }
    pthread_join(main, /*value_ptr*/ NULL);
    free(main);
    free(private_data_main);
    return error;
}

void* main_thread(void* data) {
    const private_data_t* private_data = (private_data_t*)data;
    shared_data_t* shared_data = private_data->shared_data;
    int* sockfd = NULL;
    struct sockaddr_in* servaddr = calloc(1, sizeof(struct sockaddr_in));
    start_up_server(sockfd, servaddr);
    printf("Server start\n\n");
    while (true) {
        // cualquier sock que entre
        sockfd = calloc(1, sizeof(int));
        // Todo: asignarle el sock que acaba de acaba de acabar de entrar
        *sockfd = 69; // nice
        pthread_mutex_lock(&shared_data->can_acess_sock_queue);        
        insert_to_sock_queue(shared_data->sock_queue, sockfd);
        pthread_mutex_unlock(&shared_data->can_acess_sock_queue);
    }
    delete_server(sockfd, servaddr);
    printf("\nServer end\n");
    return 0;
}

void insert_to_sock_queue(sock_queue_t* head, int* sockfd) {
    if (head == NULL) {
        head = calloc(1, sizeof(struct sockaddr_in));
        head->sockfd = sockfd;
        head->next = NULL;
    } else {
        sock_queue_t* tmp = head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = calloc(1, sizeof(struct sockaddr_in));
        tmp->sockfd = sockfd;
        tmp->next = NULL;
    }
}

void* sock_thread(void* data) {
    const private_data_t* private_data = (private_data_t*)data;
    shared_data_t* shared_data = private_data->shared_data;
    int* sockfd = NULL;
    struct sock_queue_t* tmp = NULL;
    struct sockaddr_in* cliaddr = calloc(1, sizeof(struct sockaddr_in));
    memset(cliaddr, 0, sizeof(*cliaddr));
    while (true) {    // Todo: STOP CONDITION ctrl+c
        // Here is the consume
        pthread_mutex_lock(&shared_data->can_acess_sock_queue);
        if (shared_data->sock_queue != NULL) {
            sockfd = shared_data->sock_queue->sockfd;
            tmp = shared_data->sock_queue;
            shared_data->sock_queue = shared_data->sock_queue->next;
            free(tmp);
        }
        pthread_mutex_unlock(&shared_data->can_acess_sock_queue);
        if (sockfd != NULL) {
            // Here is the dynamic magic
            if(receive_autentication_data(sockfd, cliaddr)) {
                perror("Autentication failed\n");
            }
            if(receive_file_from_client(sockfd, cliaddr)) {
                perror("Failed to receive the file from the client\n");
            }
            free(NULL);
            sockfd = NULL;
        }
    }
    return NULL;
}

int receive_file_from_client(int* sockfd, struct sockaddr_in* cliaddr) {
    socklen_t len = sizeof(*cliaddr); //len is value/resuslt
    printf("\nAutentication process ended\n\n"); // NO TOCAR ESTA LINEA DE CODIGO BAJO NINGUNA CIRCUNSTANCIA
    char output_filename[20];
    printf("Write the filename: ");
    scanf("%s", output_filename);
    printf("\nWaiting for the setup message\n");
    char package_message[PACKAGE_LENGTH];
    int n = recvfrom(*sockfd, (char *)package_message, PACKAGE_LENGTH, MSG_WAITALL, (struct sockaddr *)cliaddr, &len);
    package_message[n] = '\0';
    printf("Message received: \"%s\"\n", package_message);
    int package_number = package_number_to_integer(package_message);
    char confirmation[8] = "Received";
    sendto(*sockfd, (const char *)confirmation, strlen(confirmation), MSG_CONFIRM, (const struct sockaddr *)cliaddr, len);
    char send_next[4] = "0000"; //  0000, 0001, 0002
    send_next[4] = '\0';
    int next_message = 0;       //  0,  1,  2, 3
    int last_received = 0;      //  0,  0,  1, 2
    for (int i = 0; i < package_number; ++i)
    { 
        printf("next message %d\t last_received %d\t send_next %s\n", next_message, last_received, send_next);
        int n3 = recvfrom(*sockfd, (char *)package_message, PACKAGE_LENGTH, MSG_WAITALL, (struct sockaddr *)cliaddr, &len);
        package_message[n3] = '\0';
        last_received = package_number_to_integer(package_message);
        if (last_received == next_message) {
            decode_message(package_message, output_filename);
            ++next_message;
            package_number_to_string(send_next, next_message);
        } else {
            --i;
        }
        printf("next message %d\t last_received %d\t send_next %s\n\n", next_message, last_received, send_next);
        sendto(*sockfd, (const char *)send_next, PACKAGE_NUMBER_LENGTH, MSG_CONFIRM, (const struct sockaddr *)cliaddr, len);
    }
    return 0;
}

int receive_autentication_data(int* sockfd, struct sockaddr_in* cliaddr) {
    socklen_t len = sizeof(*cliaddr); //len is value/results
    int auten_code = 69;
    char buffer[MAXLINE];
    printf("\nWaiting for the username and password\n");
    int n = recvfrom(*sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)cliaddr, &len);
    buffer[n] = '\0';
    auten_code = check_autentication(buffer);
    if (auten_code != 0)
    {
        char *auten_error = "auten_error";
        sendto(*sockfd, (const char *)auten_error, strlen(auten_error), MSG_CONFIRM, (const struct sockaddr *)cliaddr, len);
        return 11;
    }
    char *auten_sucess = "auten_sucess";
    sendto(*sockfd, (const char *)auten_sucess, strlen(auten_sucess), MSG_CONFIRM, (const struct sockaddr *)cliaddr, len);
    return 0;
}

void delete_server(int* sockfd, struct sockaddr_in* servaddr) {
    free(sockfd);
    free(servaddr);
}

int start_up_server(int* sockfd, struct sockaddr_in* servaddr) {
    // Creating socket file descriptor
    if ((*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        return 4;
    }

    memset(servaddr, 0, sizeof(*servaddr));

    // Filling server information
    servaddr->sin_family = AF_INET; // IPv4
    servaddr->sin_addr.s_addr = INADDR_ANY;
    servaddr->sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(*sockfd, (const struct sockaddr *)servaddr, sizeof(*servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int check_autentication(char *buffer)
{
    int separador[2];
    FILE *data_base;
    int j = 0;
    for (int i = 0; i < MAXLINE; ++i)
    {
        if (buffer[i] == '/')
        {
            separador[j] = i;
            ++j;
        }
    }
    char name[20];
    char password[20];
    for (int i = 0; i < separador[0]; ++i)
    {
        name[i] = buffer[i];
    }
    name[separador[0]] = '\0';
    int auxCounter = 0;
    for (int i = separador[0] + 1; i < separador[1]; ++i)
    {
        password[auxCounter] = buffer[i];
        ++auxCounter;
    }
    password[auxCounter] = '\0';
    printf("Name: %s\n", name);
    printf("Password: %s\n", password);

    // Consulta si el archivo se abrio correctamente
    if ((data_base = fopen("data_base.txt", "r")) == NULL)
    {
        printf("ERROR: File couldnt be open");
        exit(1);
    }

    // Mientras que hayan datos en el archivo
    char file_username[40];
    char file_password[40];
    while (!feof(data_base))
    {
        fgets(file_username, 40, data_base);
        fgets(file_password, 40, data_base);
        // Si el usuario y la contrasena son iguales
        if (strstr(file_username, name) != NULL && strstr(file_password, password) != NULL)
        {
            printf("Autentication completed, access granted\n");
            fclose(data_base);
            return 0;
        }
    }
    printf("Autentication failed, access denied\n");
    fclose(data_base);
    return 1;
}

void decode_message(char *buffer, char *output_filename)
{
    char buffer_message[PACKAGE_MESSAGE_LENGTH];
    for (int i = 0; i <= PACKAGE_MESSAGE_LENGTH; ++i)
    {
        buffer_message[i] = buffer[i + PACKAGE_NUMBER_LENGTH];
    }
    buffer_message[PACKAGE_MESSAGE_LENGTH] = '\0';
    write_in_file(output_filename, buffer_message);
}

void write_in_file(char *output_filename, char *text)
{
    FILE *f;
    f = fopen(output_filename, "a");
    fprintf(f, "%s", text);
    fclose(f);
}
