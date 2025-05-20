/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:34:18 by kbossio           #+#    #+#             */
/*   Updated: 2025/05/20 14:40:47 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	lone_philo(t_philo *p)
{
	int	start;

	start = get_ms(0);
	p->last_eat = start;
	sem_wait(p->data->forks);
	print("%dms %d has taken a fork\n", get_ms(start), p);
	while (get_ms(p->last_eat) < p->data->time_to_die)
		;
	print("%dms %d has died\n", get_ms(start), p);
	return (free_all(p->data, p->data->pid), 0);
}

void	free_sem(t_data *d)
{
	if (d->forks)
	{
		sem_close(d->forks);
		sem_unlink("/forks");
	}
	if (d->print)
	{
		sem_close(d->print);
		sem_unlink("/print");
	}
	if (d->s)
	{
		sem_close(d->s);
		sem_unlink("/s");
	}
}

void	init(t_data *d, int argc, char *argv[], int n)
{
	int	i;

	i = 0;
	d->philo_count = n;
	d->time_to_die = ft_atoi(argv[2]);
	d->time_to_eat = ft_atoi(argv[3]);
	d->time_to_sleep = ft_atoi(argv[4]);
	d->ne = -1;
	if (argc == 6)
		d->ne = ft_atoi(argv[5]);
	while (i < n)
	{
		d->philos[i].id = i + 1;
		d->philos[i].te = 0;
		d->philos[i].last_eat = 0;
		d->philos[i++].data = d;
	}
}

t_data	*create(int n, int argc, char *argv[])
{
	t_data	*d;

	d = malloc(sizeof(t_data));
	if (!d)
		return (printf("Error\n"), NULL);
	d->forks = sem_open("/forks", O_CREAT, 0644, n);
	if (d->forks == SEM_FAILED)
		return (printf("Error\n"), free_sem(d), free(d), NULL);
	d->print = sem_open("/print", O_CREAT, 0644, 1);
	if (d->print == SEM_FAILED)
		return (printf("Error\n"), free_sem(d), free(d), NULL);
	d->s = sem_open("/s", O_CREAT, 0644, 0);
	if (d->s == SEM_FAILED)
		return (printf("Error\n"), free_sem(d), free(d), NULL);
	d->philos = malloc(sizeof(t_philo) * n);
	if (!d->philos)
		return (printf("Error\n"), free_sem(d), free(d), NULL);
	init(d, argc, argv, n);
	return (d);
}

int	main(int argc, char **argv)
{
	int				n;
	int				i;
	t_data			*d;

	i = 0;
	if ((argc != 5 && argc != 6) || check(argv) || ft_atoi(argv[1]) > 200)
		return (printf("Error\n"), 1);
	(void)argc;
	n = ft_atoi(argv[1]);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/s");
	d = create(n, argc, argv);
	if (!d)
		return (1);
	d->pid = malloc(sizeof(pid_t) * n);
	if (!d->pid)
		return (printf("Error\n"), free_sem(d), free(d->philos), free(d), 1);
	while (i < n)
		d->pid[i++] = -1;
	start(d->philos, n);
	return (0);
}
