/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:02:45 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/22 18:16:43 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	eat(t_philo *p, int start)
{
	p->last_eat = get_ms(0);
	p->te++;
	(void) start;
	//printf("%dms %d has eaten %d times\n", get_ms(start), p->id, p->te);
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_lock(p->data->l);
	p->l = 0;
	pthread_mutex_unlock(p->data->l);
}

void	lock(t_philo *p)
{
	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(p->right_fork);
		pthread_mutex_lock(p->left_fork);
	}
	else
	{
		pthread_mutex_lock(p->left_fork);
		pthread_mutex_lock(p->right_fork);
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
void	print(char *str, int start, t_philo *p)
{
	pthread_mutex_lock(p->data->print);
	pthread_mutex_lock(p->data->s);
	if (p->data->stop == 0)
		printf(str, start, p->id);
	pthread_mutex_unlock(p->data->s);
	pthread_mutex_unlock(p->data->print);
}

void	behaviour(t_philo *p, int start)
{
	while (p->data->time_to_die > get_ms(p->last_eat)
		&& (p->data->ne == -1 || p->data->ne > p->te))
	{
		lock(p);
		if (check_stop(p) || p->data->time_to_die <= get_ms(p->last_eat))
			break ;
		print("%dms %d has taken a fork\n", get_ms(start), p);
		print("%dms %d has taken a fork\n", get_ms(start), p);
		if (p->data->time_to_die <= get_ms(p->last_eat) || check_stop(p))
			break ;
		print("%dms %d is eating\n", get_ms(start), p);
		if (smart_sleep(p->data->time_to_eat, p->data->time_to_die) || check_stop(p))
			break ;
		eat(p, start);
		if (smart_sleep(p->data->time_to_eat, p->data->time_to_die) || check_stop(p)
			|| (p->data->ne != -1 && p->te >= p->data->ne))
			break ;
		print("%dms %d is sleeping\n", get_ms(start), p);
		if (smart_sleep(p->data->time_to_sleep, p->data->time_to_die) || check_stop(p))
			break ;
		print("%dms %d is thinking\n", get_ms(start), p);
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
			printf("%dms %d died\n", get_ms(start), p->id);
			pthread_mutex_unlock(p->data->print);
		}
	}
	pthread_exit(NULL);
}
