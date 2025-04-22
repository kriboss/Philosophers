/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:40:52 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/22 18:02:55 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_ms(int start)
{
	struct timeval	tv;
	int				ms;

	gettimeofday(&tv, NULL);
	ms = ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - start;
	return (ms);
}

int	smart_sleep(long duration_ms, long ttd)
{
	long	start;
	long	now;

	start = get_ms(0);
	now = get_ms(start);
	while (now < duration_ms)
	{
		usleep(100);
		now = get_ms(start);
		if (now >= ttd)
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
		j = ft_atoi(argv[i]);
		if (j <= 0)
			return (1);
		i++;
	}
	return (0);
}

void	*free_all(t_data *data, pthread_mutex_t *forks)
{
	int	i;

	if (forks)
	{
		i = 0;
		while (i < data->philo_count + 3)
		{
			pthread_mutex_destroy(&forks[i]);
			i++;
		}
		free(forks);
	}
	if (data->philos)
		free(data->philos);
	free(data);
	return (NULL);
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
			segno = -segno;
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
