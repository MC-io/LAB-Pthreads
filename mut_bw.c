#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int counter = 0;
const int thread_count = 5;
pthread_mutex_t barrier_mutex;


void* func(void* arg) 
{ 
    int ptid = pthread_self();
    printf("El thread %d inicia su tarea\n", ptid);
    int t = rand() % 5 + 1;
    sleep(t);
    printf("El thread %d termino su tarea y esperara si hay threads que faltan\n", ptid);
    printf("Mutex bloqueado por thread nro %d\n", ptid);
    pthread_mutex_lock(&barrier_mutex);
    counter++;
    pthread_mutex_unlock(&barrier_mutex);
    printf("Mutex liberado por thread nro %d\n", ptid);
    while (counter < thread_count);

    printf("Todos los threads estan sincronizados a este punto! El thread %d seguira trabajando\n", ptid);
    pthread_exit(NULL); 
}
  

int main() 
{ 
    srand(time(NULL));
    pthread_t ptid[thread_count];
  
    for (int i = 0; i < thread_count; i++)
    {
        pthread_create(&ptid[i], NULL, &func, NULL); 
    }
    
    // Waiting for the created thread to terminate
    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(ptid[i], NULL); 
    }
  
    pthread_exit(NULL);  
    return 0; 
} 