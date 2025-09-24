/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:47:20 by kbossio           #+#    #+#             */
/*   Updated: 2025/09/24 11:42:30 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	eat(t_philo *p, int start)
{
	pthread_mutex_lock(p->data->eat_lock);
	p->te++;
	pthread_mutex_unlock(p->data->eat_lock);
	(void) start;
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_lock(p->data->l);
	p->l = 0;
	pthread_mutex_unlock(p->data->l);
}

void	print(char *fmt, int timestamp, t_philo *p)
{
	pthread_mutex_lock(p->data->s);
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
	if (get_ms(p->last_eat) >= p->data->time_to_die
		|| (p->data->ne != -1 && p->te >= p->data->ne))
	{
		pthread_mutex_unlock(p->data->eat_lock);
		return (0);
	}
	pthread_mutex_unlock(p->data->eat_lock);
	return (1);
}
