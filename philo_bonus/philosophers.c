/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:34:18 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/15 19:33:42 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	int				n;
	int				i;
	pthread_t		*threads;
	t_data			*d;
	pid_t			pid;

	//if ((argc != 5 && argc != 6) || check(argv) || ft_atoi(argv[1]) > 200)
	//	return (printf("Error\n"), 1);
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
	while (i < n)
	{
		pid = fork();
		i++;
	}
	return (0);
}
