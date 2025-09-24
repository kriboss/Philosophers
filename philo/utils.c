/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:40:52 by kbossio           #+#    #+#             */
/*   Updated: 2025/09/24 12:12:50 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	get_ms(long long start)
{
	struct timeval	tv;
	long long		ms;

	gettimeofday(&tv, NULL);
	ms = ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - start;
	return (ms);
}

int	smart_sleep(long long duration_ms, t_philo *p)
{
	long long	start;
	long long	now;

	start = get_ms(0);
	now = get_ms(start);
	while (now < duration_ms)
	{
		usleep(50);
		now = get_ms(start);
		if (check_stop(p))
			return (1);
	}
	return (0);
}

int	check(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if ((argv[i][j] < '0' || argv[i][j] > '9') && argv[i][j] != '+')
				return (1);
			j++;
		}
		j = ft_atoi(argv[i]);
		if (j <= 0)
			return (1);
		i++;
	}
	return (0);
}

void	free_all(t_data *d, pthread_mutex_t *forks, pthread_t *th, pthread_t *m)
{
	int	i;

	if (th)
		free(th);
	if (m)
		free(m);
	if (forks)
	{
		i = 0;
		while (i < d->philo_count + 3)
		{
			pthread_mutex_destroy(&forks[i]);
			i++;
		}
		free(forks);
	}
	if (d->philos)
		free(d->philos);
	free(d);
}

long	ft_atoi(const char *str)
{
	unsigned int	i;
	int				num;
	int				segno;
	int				cont;

	num = 0;
	segno = 1;
	i = 0;
	cont = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		cont++;
		if (str[i] == '-')
			return (-1);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i++] - 48);
		if (cont > 1 || (num > IMAX && segno == 1))
			return (-1);
	}
	return (num * segno);
}
