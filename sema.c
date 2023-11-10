#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

const int thread_count = 5;
int counter = 0;
sem_t count_sem;
sem_t barrier_sem;


void* func(void* arg) 
{ 
    int ptid = pthread_self();
    printf("El thread %d inicia su tarea\n", ptid);
    int t = rand() % 5 + 1;
    sleep(t);
    printf("El thread %d termino su tarea y esperara al semaforo de contador\n", ptid);
    sem_wait(&count_sem);
    printf("El thread %d entro al semaforo de contador\n", ptid);

    if (counter == thread_count - 1)
    {
        counter = 0;
        sem_post(&count_sem);
        for (int j = 0; j < thread_count - 1; j++)
        {
            sem_post(&barrier_sem);
        }
    }
    else
    {
        counter++;
        printf("El thread %d activa el semaforo y esperara a que todos terminen sus tareas\n", ptid);
        sem_post(&count_sem);
        sem_wait(&barrier_sem);
    }
    printf("Todos los threads estan sincronizados! El thread %d seguira trabajando\n", ptid);
    pthread_exit(NULL); 
}
  

int main() 
{ 
    srand(time(NULL));

    sem_init(&count_sem, 0, 1);
    sem_init(&barrier_sem, 0, 0);

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