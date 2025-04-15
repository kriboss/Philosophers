/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:02:45 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/15 19:29:32 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*lone_philo(void *arg)
{
	t_philo	*philos;
	int		start;

	philos = (t_philo *)arg;
	start = get_ms(0);
	while (philos->data->time_to_die > get_ms(start))
	{
		printf("%dms %d is sleeping\n", get_ms(start), philos->id);
		if (smart_sleep(philos->data->time_to_sleep, philos->data->time_to_die))
			break ;
		printf("%dms %d is thinking\n", get_ms(start), philos->id);
	}
	printf("%dms %d died\n", get_ms(start), philos->id);
	pthread_exit(NULL);
}

void	eat(t_philo *p, int start)
{
	p->last_eat = get_ms(0);
	p->te++;
	printf("%dms %d has eaten %d times\n", get_ms(start), p->id, p->te);
}

void	behaviour(t_philo *p, int start)
{
	while (p->data->time_to_die > get_ms(p->last_eat)
		&& (p->data->ne == -1 || p->data->ne > p->te))
	{
		pthread_mutex_lock(p->left_fork);
		pthread_mutex_lock(p->right_fork);
		if (p->data->stop || p->data->time_to_die <= get_ms(p->last_eat))
			break ;
		printf("%dms %d has taken a fork\n", get_ms(start), p->id);
		printf("%dms %d has taken a fork\n", get_ms(start), p->id);
		if (p->data->time_to_die <= get_ms(p->last_eat) || p->data->stop)
			break ;
		printf("%dms %d is eating\n", get_ms(start), p->id);
		if (smart_sleep(p->data->time_to_eat, p->data->time_to_die) || p->data->stop)
			break ;
		eat(p, start);
		pthread_mutex_unlock(p->right_fork);
		pthread_mutex_unlock(p->left_fork);
		if (smart_sleep(p->data->time_to_eat, p->data->time_to_die) || p->data->stop
			|| (p->data->ne != -1 && p->te >= p->data->ne))
			break ;
		printf("%dms %d is sleeping\n", get_ms(start), p->id);
		if (smart_sleep(p->data->time_to_sleep, p->data->time_to_die) || p->data->stop)
			break ;
		printf("%dms %d is thinking\n", get_ms(start), p->id);
	}
}

void	*routine(void *arg)
{
	t_philo	*p;
	int		start;

	p = (t_philo *)arg;
	start = get_ms(0);
	p->last_eat = start;
	behaviour(p, start);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	if (p->data->time_to_die <= get_ms(p->last_eat))
	{
		if (p->data->stop == 0)
		{
			p->data->stop = 1;
			printf("%dms %d died\n", get_ms(start), p->id);
		}
		pthread_exit(NULL);
	}
	pthread_exit(NULL);
}
