#include <pthread.h>
#include <signal.h>

void *thread_func(void *arg)
{
    (void)arg;
    return NULL;
}

int main(void)
{
    pthread_t thread;
    if (pthread_create(&thread, NULL, thread_func, NULL))
        raise(SIGSEGV);
    return 0;
}