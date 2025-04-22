/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:02:45 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/22 18:49:30 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	behaviour(t_philo *p, int start, int id)
{
	while (1)
	{
		printf("%dms %d is eating\n", get_ms(start), id);
		if (smart_sleep(p->data->time_to_eat, p->data->time_to_die))
			break ;
		printf("%dms %d is sleeping\n", get_ms(start), id);
		if (smart_sleep(p->data->time_to_sleep, p->data->time_to_die))
			break ;
		printf("%dms %d is thinking\n", get_ms(start), id);
		if (p->data->stop == 1)
			break ;
	}
}

void	routine(t_philo *p, int n)
{
	int		i;
	int		start;
	pid_t	pid;

	i = 0;
	start = get_ms(0);
	p->last_eat = start;
	while (i < n)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("Fork failed");
			exit(EXIT_FAILURE);
		}
		p->id = i + 1;
		behaviour(p, start, i + 1);
		i++;
	}
}
