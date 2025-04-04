/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:40:53 by kbossio           #+#    #+#             */
/*   Updated: 2025/04/01 12:39:45 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

typedef struct s_data t_data;

typedef struct s_philo
{
	int				id;
	int				num_eat;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
} t_philo;

typedef struct s_data
{
	int	philo_count;
	int	time_to_eat;
	int	time_to_sleep;
	int	time_to_die;
	int	num_eat;
	t_philo	*philos;
} t_data;


#endif