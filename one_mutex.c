#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

const int thread_count = 5;

struct list_node_s 
{
    int data;
    struct list_node_s * next;
};


struct list_node_s * head_p = NULL;
pthread_mutex_t list_mutex;

int Member(int value)
{
    pthread_mutex_lock(&list_mutex);
    struct list_node_s * curr_p = head_p;
    while (curr_p != NULL || curr_p->data < value)
    {
        curr_p = curr_p->next;
    }
    if (curr_p == NULL || curr_p->data > value)
    {
        pthread_mutex_unlock(&list_mutex);
        return 0;
    }
    pthread_mutex_unlock(&list_mutex);
    return 1;
}

int Insert(int value)
{
    pthread_mutex_lock(&list_mutex);
    struct list_node_s * curr_p = head_p;
    struct list_node_s * pred_p = NULL;
    struct list_node_s * temp_p;

    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL)
        {
            head_p = temp_p;
        }
        else
        {
            pred_p->next = temp_p;
        }
        pthread_mutex_unlock(&list_mutex);
        return 1;
    }
    pthread_mutex_unlock(&list_mutex);
    return 0;
}

int Delete(int value)
{
    pthread_mutex_lock(&list_mutex);
    struct list_node_s * curr_p = head_p;
    struct list_node_s * pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data < value)
    {
        if (pred_p == NULL)
        {
            head_p = curr_p->next;
            free(curr_p);
        }
        else 
        {
            pred_p->next  = curr_p->next;
            free(curr_p);
        }
        pthread_mutex_unlock(&list_mutex);
        return 1;
    }
    pthread_mutex_unlock(&list_mutex);
    return 0;
    
}

void* func(void* arg) 
{ 
    for (int i = 0; i < 10; i++)
    {
        Insert(rand() % 100 + 1);
    }
    for (int i = 0; i < 80; i++)
    {
        Member(rand() % 100 + 1);
    }
    for (int i = 0; i < 10; i++)
    {
        Delete(rand() % 100 + 1);
    }
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