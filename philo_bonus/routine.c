/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:02:45 by kbossio           #+#    #+#             */
/*   Updated: 2025/05/20 18:23:55 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void    print(char *str, int start, t_philo *p)
{
	sem_wait(p->data->print);
	printf(str, start, p->id);
	sem_post(p->data->print);
}

void    eat(t_philo *p)
{
    p->last_eat = get_ms(0);
    p->te++;
	sem_wait(p->data->print);
	printf("%d has eaten %d times\n", p->id, p->te);
	sem_post(p->data->print);
    sem_post(p->data->forks);
    sem_post(p->data->forks);
}

void forks(t_philo *p, int start)
{
	sem_wait(p->data->forks);
	print("%dms %d has taken a fork\n", get_ms(start), p);
	sem_wait(p->data->forks);
	print("%dms %d has taken a fork\n", get_ms(start), p);
}

int    behaviour(t_philo *p, int start)
{
    while (p->data->time_to_die > get_ms(p->last_eat))
    {
        forks(p, start);
		if (get_ms(p->last_eat) >= p->data->time_to_die)
			return (1);
        print("%dms %d is eating\n", get_ms(start), p);
        if (smart_sleep(p->data->time_to_eat, p->data->time_to_die))
			return (1);
        eat(p);
        if (get_ms(p->last_eat) >= p->data->time_to_die)
            return (1);
		else if (p->data->ne != -1 && p->te == p->data->ne)
			return (0);
        print("%dms %d is sleeping\n", get_ms(start), p);
        if (smart_sleep(p->data->time_to_sleep, p->data->time_to_die))
            return (1);
        print("%dms %d is thinking\n", get_ms(start), p);
    }
	return (1);
}

void    *watch_die(void *arg)
{
    t_philo    *p;
	
    p = (t_philo *)arg;
    sem_wait(p->data->s);
    pthread_exit(NULL);
    exit(0);
}

void    routine(t_philo *p)
{
    int         i;
    pthread_t   watcher;

    i = 0;
    pthread_create(&watcher, NULL, watch_die, p);
    pthread_detach(watcher);
    p->data->start = get_ms(0);
    p->last_eat = p->data->start;
	print("%dms %d is SPAWNED\n", get_ms(0), p);
    if (behaviour(p, p->data->start))
	{
        print("%dms %d has died\n", get_ms(p->data->start), p);
		sem_wait(p->data->print);
		while (i < p->data->philo_count - 1)
		{
			sem_post(p->data->s);
			i++;
		}
	}
    free_all(p->data, p->data->pid);
    exit(0);
}

int    start(t_philo *p, int n)
{
    int     i;

    i = 0;
    while (i < n)
    {
        p->data->pid[i] = fork();
        if (p->data->pid[i] == -1)
            return (printf("Error Initializing Forks\n"), free_all(p->data, p->data->pid), 1);
        if (p->data->pid[i] == 0)
        {
			p->id = i + 1;
			if (n != 1)
            	routine(p);
			else
				lone_philo(p);
            exit(0);
        }
        i++;
    }
    i = 0;
	while (i < n)
        waitpid(p->data->pid[i++], NULL, 0);
    return (free_all(p->data, p->data->pid), 0);
}
