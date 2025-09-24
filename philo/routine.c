/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:02:45 by kbossio           #+#    #+#             */
/*   Updated: 2025/09/24 12:55:26 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	ft_min(long long a, long long b)
{
	if (a < b)
		return (a);
	return (b);
}

void	ft_usleep(long long duration_ms)
{
	long long	start;
	long long	now;

	start = get_ms(0);
	now = get_ms(start);
	while (now < duration_ms)
	{
		usleep(50);
		now = get_ms(start);
	}
}

void	behaviour(t_philo *p, int start)
{
	while (check_alive(p))
	{
		lock(p, start);
		if (check_stop(p) || p->data->time_to_die <= get_ms(p->last_eat))
			break ;
		if (p->data->time_to_die <= get_ms(p->last_eat) || check_stop(p))
			break ;
		print("%lldms %d is eating\n", get_ms(start), p);
		pthread_mutex_lock(p->data->eat_lock);
		p->last_eat = get_ms(0);
		pthread_mutex_unlock(p->data->eat_lock);
		if (smart_sleep(p->data->time_to_eat, p) || check_stop(p))
			break ;
		eat(p, start);
		if (p->data->time_to_die <= get_ms(p->last_eat) || check_stop(p)
			|| (p->data->ne != -1 && p->te >= p->data->ne))
			break ;
		print("%lldms %d is sleeping\n", get_ms(start), p);
		if (smart_sleep(p->data->time_to_sleep, p) || check_stop(p))
			break ;
		print("%lldms %d is thinking\n", get_ms(start), p);
		usleep(500);
	}
}

void	*routine(void *arg)
{
	t_philo		*p;
	long long	start;

	p = (t_philo *)arg;
	start = get_ms(0);
	pthread_mutex_lock(p->data->eat_lock);
	p->last_eat = start;
	pthread_mutex_unlock(p->data->eat_lock);
	if (p->id % 2 == 0)
		ft_usleep(ft_min(p->data->time_to_eat, p->data->time_to_sleep) * 0.9);
	behaviour(p, start);
	if (p->l == 1)
	{
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
	}
	if (p->data->stop == 1)
		pthread_exit(NULL);
	pthread_mutex_lock(p->data->s);
	p->data->full++;
	pthread_mutex_unlock(p->data->s);
	pthread_exit(NULL);
}
