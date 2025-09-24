/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:50:39 by kbossio           #+#    #+#             */
/*   Updated: 2025/09/24 12:55:12 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	check_death(t_data *d, t_philo *p)
{
	pthread_mutex_lock(d->eat_lock);
	if (get_ms(p->last_eat) >= d->time_to_die)
	{
		print("%lldms %d died\n", get_ms(p->last_eat), p);
		pthread_mutex_lock(d->s);
		d->stop = 1;
		pthread_mutex_unlock(d->s);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(d->eat_lock);
	pthread_mutex_lock(d->s);
	if (d->full == d->philo_count && d->ne != -1)
	{
		pthread_mutex_unlock(d->s);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(d->s);
}

void	*monitoring(void *arg)
{
	t_data	*d;
	int		i;

	d = (t_data *)arg;
	usleep(100 * d->philo_count);
	while (1)
	{
		i = 0;
		while (i < d->philo_count)
		{
			check_death(d, &d->philos[i]);
			i++;
		}
	}
	pthread_exit(NULL);
}

t_data	*allocating(int n, pthread_t *threads)
{
	t_data		*d;

	d = malloc(sizeof(t_data));
	if (!d)
		return (printf("Error\n"), free(threads), NULL);
	d->monitor = malloc(sizeof(pthread_t));
	if (!d->monitor)
		return (printf("Error\n"), free(threads), free(d), NULL);
	d->philos = malloc(sizeof(t_philo) * n);
	if (!d->philos)
	{
		free(threads);
		return (printf("Error\n"), free(d->monitor), free(d), NULL);
	}
	return (d);
}
