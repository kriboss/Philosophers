/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:50:39 by kbossio           #+#    #+#             */
/*   Updated: 2025/09/23 22:35:18 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitoring(void *arg)
{
	t_data	*d;
	int		i;

	d = (t_data *)arg;
	usleep(100);
	while (1)
	{
		i = 0;
		while (i < d->philo_count)
		{
			pthread_mutex_lock(d->eat_lock);
			if (get_ms(d->philos[i].last_eat) >= d->time_to_die && check_stop(&d->philos[i]) == 1)
			{
				pthread_mutex_lock(d->print);
				printf("%lldms %d died\n", get_ms(d->philos[i].last_eat), i + 1);
				pthread_mutex_unlock(d->print);
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
			i++;
		}
	}
	pthread_exit(NULL);
}
