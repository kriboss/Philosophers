/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:02:45 by kbossio           #+#    #+#             */
/*   Updated: 2025/05/14 12:39:17 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void    eat(t_philo *p, int start)
{
    p->last_eat = get_ms(0);
    p->te++;
    (void) start;
    sem_post(p->data->forks);
    sem_post(p->data->forks);
	sem_post(p->data->l);
    p->l = 0;
}

void    print(char *str, int start, t_philo *p)
{
    sem_wait(p->data->print);
	printf(str, start, p->id);
	sem_post(p->data->print);
}

int lock(t_philo *p, int start)
{
    if (get_ms(p->last_eat) >= p->data->time_to_die)
		return (1);
	if (p->turn % 2 != p->id % 2)
		sem_wait(p->data->l);
	p->turn++;
	sem_wait(p->data->forks);
	sem_wait(p->data->forks);
	p->l = 1;
	print("%dms %d has taken a fork\n", get_ms(start), p);
	print("%dms %d has taken a fork\n", get_ms(start), p);
    return (0);
}

int    behaviour(t_philo *p, int start)
{
    while (p->data->time_to_die > get_ms(p->last_eat))
    {
        if (lock(p, start) || get_ms(p->last_eat) >= p->data->time_to_die)
			return (1);
        print("%dms %d is eating\n", get_ms(start), p);
        if (smart_sleep(p->data->time_to_eat, p->data->time_to_die))
			return (1);
        eat(p, start);
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

void    *kill_all(void *arg)
{
    t_philo    *p;

    p = (t_philo *)arg;
    // Wait for stop signal
    sem_wait(p->data->s);
    // Exit child process
    exit(0);
}

void    routine(t_philo *p)
{
    int         start;
    int         i;
    pthread_t   watcher;

    i = 0;
    // Create and detach watcher thread for coordinated exit
    pthread_create(&watcher, NULL, kill_all, p);
    pthread_detach(watcher);
    
    start = get_ms(0);
    p->last_eat = start;
    if (behaviour(p, start))
	{
		if (p->l == 1)
		{
			sem_post(p->data->forks);
			sem_post(p->data->forks);
		}
		while (i < p->data->philo_count - 1)
        {
			sem_post(p->data->s);
            i++;
        }
		print("%dms %d DIED\n", get_ms(start), p);
	}
    exit(0);
}

void    start(t_philo *p, int n)
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
