/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:02:45 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/29 19:50:52 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void
eat(t_philo *p, int start)
{
    p->last_eat = get_ms(0);
    p->te++;
    (void) start;
    sem_post(p->data->forks);
    sem_post(p->data->forks);
    sem_wait(p->data->l);
    p->l = 0;
    sem_post(p->data->l);
}

void
print(char *str, int start, t_philo *p)
{
    sem_wait(p->data->print);
    printf(str, start, p->id);
    sem_post(p->data->print);
}

int
lock(t_philo *p, int start)
{
    sem_wait(p->data->forks);
    sem_wait(p->data->forks);
    if (get_ms(p->last_eat) >= p->data->time_to_die)
        return (1);
    print("%dms %d has taken a fork\n", get_ms(start), p);
    print("%dms %d has taken a fork\n", get_ms(start), p);
    p->turn++;
    sem_wait(p->data->l);
    p->l = 1;
    sem_post(p->data->l);
    return (0);
}

void
behaviour(t_philo *p, int start)
{
    while (p->data->time_to_die > get_ms(p->last_eat))
    {
        if (lock(p, start) || get_ms(p->last_eat) >= p->data->time_to_die)
            break ;
        print("%dms %d is eating\n", get_ms(start), p);
        if (smart_sleep(p->data->time_to_eat, p->data->time_to_die))
            break ;
        eat(p, start);
        if (get_ms(p->last_eat) >= p->data->time_to_die || 
            (p->data->ne != -1 && p->data->ne <= p->te))
        {
            print("%dms %d ciao\n", get_ms(start), p);
            break ;
        }
        print("%dms %d is sleeping\n", get_ms(start), p);
        if (smart_sleep(p->data->time_to_sleep, p->data->time_to_die))
            break ;
        print("%dms %d is thinking\n", get_ms(start), p);
    }
}

void    *kill_all(void *arg)
{
    t_philo    *p;

    p = (t_philo *)arg;
    // Wait for stop signal
    sem_wait(p->data->s);
    // Exit child process
    exit(0);
}

void
routine(t_philo *p)
{
    int         start;
    pthread_t   watcher;

    // Create and detach watcher thread for coordinated exit
    pthread_create(&watcher, NULL, kill_all, p);
    pthread_detach(watcher);

    start = get_ms(0);
    p->last_eat = start;
    behaviour(p, start);
    if (p->l == 1)
    {
        sem_post(p->data->forks);
        sem_post(p->data->forks);
    }
    if (p->data->time_to_die <= get_ms(p->last_eat))
    {
        // Signal all watchers to exit
        for (int i = 0; i < p->data->philo_count; i++)
            sem_post(p->data->s);
        print("%dms %d DIED\n", get_ms(start), p);
    }
    // Normal exit
    exit(0);
}

void
start(t_philo *p, int n)
{
    int     i;

    i = 0;
    while (i < n)
    {
        p->data->pid[i] = fork();
        if (p->data->pid[i] == -1)
        {
            printf("Error Initializing Forks\n");
            break ;
        }
        p->id = i + 1;
        if (p->data->pid[i] == 0)
        {
            routine(p);
            //exit(0);
        }
        i++;
    }
    i = 0;
    while (i < n)
        waitpid(p->data->pid[i++], NULL, 0);
    free_all(p->data, p->data->pid);
}
