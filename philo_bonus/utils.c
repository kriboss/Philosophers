/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:40:52 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/10 16:04:45 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
		while (i < data->philo_count)
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
