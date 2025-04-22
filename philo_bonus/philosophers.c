/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:34:18 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/22 18:44:54 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init(t_data *d, int argc, char *argv[], int n)
{
	d->philo_count = n;
	d->time_to_die = ft_atoi(argv[2]);
	d->time_to_eat = ft_atoi(argv[3]);
	d->time_to_sleep = ft_atoi(argv[4]);
	d->ne = -1;
	if (argc == 6)
		d->ne = ft_atoi(argv[5]);
}

int	main(int argc, char **argv)
{
	int				n;
	int				i;
	pthread_t		*threads;
	t_data			*d;

	if ((argc != 5 && argc != 6) || check(argv) || ft_atoi(argv[1]) > 200)
		return (printf("Error\n"), 1);
	(void)argc;
	n = ft_atoi(argv[1]);
	i = 0;
	threads = malloc(sizeof(pthread_t) * n);
	if (!threads)
		return (printf("Error\n"), 1);
	d = malloc(sizeof(t_data));
	if (!d)
		return (printf("Error\n"), free(threads), 1);
	d->philos = malloc(sizeof(t_philo) * n);
	if (!d->philos)
		return (printf("Error\n"), free(threads), free(d), 1);
	routine(d->philos, n);
	return (0);
}
