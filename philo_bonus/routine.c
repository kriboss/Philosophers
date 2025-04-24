/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:02:45 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/24 18:57:03 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	eat(t_philo *p, int start)
{
	p->last_eat = get_ms(0);
	p->te++;
	(void) start;
	//printf("%dms %d has eaten %d times\n", get_ms(start), p->id, p->te);
	sem_post(p->data->forks);
	sem_post(p->data->forks);
	sem_wait(p->data->l);
	p->l = 0;
	sem_post(p->data->l);
}

void	lock(t_philo *p)
{
	sem_wait(p->data->forks);
	sem_wait(p->data->forks);
	sem_wait(p->data->l);
	p->l = 1;
	sem_post(p->data->l);
}

int	check_stop(t_philo *p)
{
	sem_wait(p->data->s);
	if (p->data->stop == 1)
	{
		sem_post(p->data->s);
		return (1);
	}
	sem_post(p->data->s);
	return (0);
}

void	print(char *str, int start, t_philo *p)
{
	sem_wait(p->data->print);
	sem_wait(p->data->s);
	if (p->data->stop == 0)
		printf(str, start, p->id);
	sem_post(p->data->print);
	sem_post(p->data->s);
}

void	behaviour(t_philo *p, int start)
{
	while (p->data->time_to_die > get_ms(p->last_eat)
		&& (p->data->ne == -1 || p->data->ne > p->te))
	{
		write(1, "CIAOOO\n", 7);
		lock(p);
		break ;
		if (check_stop(p) || get_ms(p->last_eat) >= p->data->time_to_die)
			break ;
		print("%dms %d has taken a fork\n", get_ms(start), p);
		print("%dms %d has taken a fork\n", get_ms(start), p);
		if (get_ms(p->last_eat) >= p->data->time_to_die || check_stop(p))
			break ;
		print("%dms %d is eating\n", get_ms(start), p);
		if (smart_sleep(p->data->time_to_eat, p->data->time_to_die)
			|| check_stop(p))
			break ;
		eat(p, start);
		if (get_ms(p->last_eat) >= p->data->time_to_die || check_stop(p))
			break ;
		print("%dms %d is sleeping\n", get_ms(start), p);
		if (smart_sleep(p->data->time_to_sleep, p->data->time_to_die)
			|| check_stop(p))
			break ;
		print("%dms %d is thinking\n", get_ms(start), p);
	}
	write(1, "DIOOOO\n", 7);
}

void	routine(t_philo *p)
{
	int		start;

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
		if (check_stop(p) == 0)
		{
			sem_wait(p->data->s);
			p->data->stop = 1;
			sem_post(p->data->s);
			sem_wait(p->data->print);
			print("%dms %d died\n", get_ms(start), p);
			sem_post(p->data->print);
		}
	}
}

void	start(t_philo *p, int n, pid_t *pid)
{
	int		i;

	i = 0;
	while (i < n)
	{
		pid[i] = fork();
		if (pid[i] == -1)
		{
			printf("Error Initializing Forks\n");
			break ;
		}
		p->id = i + 1;
		if (pid[i] == 0)
		{
			routine(p);
			break ;
		}
		i++;
	}
	i = 0;
	while (i < n)
		waitpid(pid[i++], NULL, 0);
	free_all(p->data, pid);
}
