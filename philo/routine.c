/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:02:45 by kbossio           #+#    #+#             */
/*   Updated: 2025/09/23 22:32:53 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	eat(t_philo *p, int start)
{
	pthread_mutex_lock(p->data->eat_lock);
	p->te++;
	pthread_mutex_unlock(p->data->eat_lock);
	(void) start;
	printf("%lldms %d has eaten %d times\n", get_ms(start), p->id, p->te);
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_lock(p->data->l);
	p->l = 0;
	pthread_mutex_unlock(p->data->l);
}

void print(char *fmt, int timestamp, t_philo *p)
{
    pthread_mutex_lock(p->data->s);          /* check stop first */
    if (p->data->stop == 0)
    {
        pthread_mutex_lock(p->data->print);
        printf(fmt, timestamp, p->id);
        pthread_mutex_unlock(p->data->print);
    }
    pthread_mutex_unlock(p->data->s);
}


void	lock(t_philo *p, int start)
{
	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(p->left_fork);
		print("%lldms %d has taken a fork\n", get_ms(start), p);
		pthread_mutex_lock(p->right_fork);
		print("%lldms %d has taken a fork\n", get_ms(start), p);
	}
	else
	{
		pthread_mutex_lock(p->right_fork);
		print("%lldms %d has taken a fork\n", get_ms(start), p);
		pthread_mutex_lock(p->left_fork);
		print("%lldms %d has taken a fork\n", get_ms(start), p);
	}
	pthread_mutex_lock(p->data->l);
	p->l = 1;
	pthread_mutex_unlock(p->data->l);
}

int	check_stop(t_philo *p)
{
	pthread_mutex_lock(p->data->s);
	if (p->data->stop == 1)
	{
		pthread_mutex_unlock(p->data->s);
		return (1);
	}
	pthread_mutex_unlock(p->data->s);
	return (0);
}

int	check_alive(t_philo *p)
{
	pthread_mutex_lock(p->data->eat_lock);
	if (get_ms(p->last_eat) >= p->data->time_to_die || (p->data->ne != -1
		&& p->te >= p->data->ne))
	{
		pthread_mutex_unlock(p->data->eat_lock);
		return (0);
	}
	pthread_mutex_unlock(p->data->eat_lock);
	return (1);
}

void	behaviour(t_philo *p, int start)
{
	while (check_alive(p))
	{
		lock(p, start);
		if (check_stop(p) || p->data->time_to_die <= get_ms(p->last_eat))
			break ;
		//print("%dms %d has taken a fork\n", get_ms(start), p);
		//print("%dms %d has taken a fork\n", get_ms(start), p);
		if (p->data->time_to_die <= get_ms(p->last_eat) || check_stop(p))
			break ;
		print("%lldms %d is eating\n", get_ms(start), p);
		pthread_mutex_lock(p->data->eat_lock);
		p->last_eat = get_ms(0);
		pthread_mutex_unlock(p->data->eat_lock);
		if (smart_sleep(p->data->time_to_eat) || check_stop(p))
			break ;
		eat(p, start);
		if (p->data->time_to_die <= get_ms(p->last_eat) || check_stop(p)
			|| (p->data->ne != -1 && p->te >= p->data->ne))
			break ;
		print("%lldms %d is sleeping\n", get_ms(start), p);
		if (smart_sleep(p->data->time_to_sleep) || check_stop(p))
			break ;
		print("%lldms %d is thinking\n", get_ms(start), p);
		usleep(500);
	}
}

void	*routine(void *arg)
{
	t_philo	*p;
	long long		start;

	p = (t_philo *)arg;
	start = get_ms(0);
	pthread_mutex_lock(p->data->eat_lock);
	p->last_eat = start;
	pthread_mutex_unlock(p->data->eat_lock);
	if (p->id % 2 == 0)
		smart_sleep(p->data->time_to_eat * 0.9);
	behaviour(p, start);
	if (p->l == 1)
	{
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
	}
	if (p->data->time_to_die <= get_ms(p->last_eat))
	{
		if (check_stop(p) == 0)
		{
			pthread_mutex_lock(p->data->s);
			p->data->stop = 1;
			pthread_mutex_unlock(p->data->s);
			pthread_mutex_lock(p->data->print);
			printf("%lldms %d died\n", get_ms(start), p->id);
			pthread_mutex_unlock(p->data->print);
		}
	}
	pthread_mutex_lock(p->data->s);
	p->data->full++;
	pthread_mutex_unlock(p->data->s);
	pthread_exit(NULL);
}
