#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

const int thread_count = 5;
int counter = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_var;

void* func(void* arg) 
{ 
    int ptid = pthread_self();
    printf("El thread %d inicia su tarea\n", ptid);
    int t = rand() % 5 + 1;
    sleep(t);

    printf("El thread %d bloquea el mutex\n", ptid);
    pthread_mutex_lock(&mutex);
    counter++;
    if (counter == thread_count)
    {
        counter = 0;
        pthread_cond_broadcast(&cond_var);
        printf("El thread %d fue el ultimo y da la senal a la varibale de condicion\n", ptid);
    }
    else
    {
        printf("El thread %d desbloquea el mutex y espera la senal de la variable de condicion\n", ptid);
        while (pthread_cond_wait(&cond_var, &mutex) != 0);
    }
    pthread_mutex_unlock(&mutex);

    
    printf("Todos los threads estan sincronizados! El thread %d seguira trabajando\n", ptid);
    pthread_exit(NULL); 
}
  

int main() 
{ 
    srand(time(NULL));

    pthread_cond_init(&cond_var, NULL);
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