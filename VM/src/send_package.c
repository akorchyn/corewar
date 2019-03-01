/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_package.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpshenyc <kpshenyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 15:47:33 by kpshenyc          #+#    #+#             */
/*   Updated: 2019/03/01 18:06:40 by kpshenyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"

/*
** Allocates memory for buffer, that consists:
** First	carriages count								( int32_t each )
** Next		map + player those affected on each cell	( int8_t each )
** Next		CYCLE_TO_DIE								( int32_t )
** Next		carriages position							( int16_t each )
*/

uint8_t				*alloc_package(int32_t *size)
{
	*size = sizeof(uint32_t) +
			sizeof(uint8_t) * (MEM_SIZE * 2) +
			sizeof(uint32_t) +
			sizeof(uint16_t) * g_car_count;
	return ((uint8_t *)malloc(sizeof(uint8_t) * (*size)));
}

void				fill_package(t_corewar *corewar, uint8_t *package)
{
	int32_t			i;
	int32_t			step;
	t_carriage		*tmp;

	step = 0;
	ft_memcpy(package, &g_car_count, (step += sizeof(uint32_t)));
	i = -1;
	while (++i < MEM_SIZE)
	{
		package[step++] = corewar->map[i];
		package[step++] = corewar->player_affected[i];
	}
	ft_memcpy(package, &(corewar->cycles_to_die), sizeof(uint32_t));
	tmp = corewar->carriages;
	while (tmp)
	{
		ft_memcpy(package + step, &(tmp->counter), sizeof(uint16_t));
		step += sizeof(uint16_t);
		tmp = tmp->next;
	}
}

void				send_package(t_corewar *corewar)
{
	uint8_t			*package;
	int32_t			size;

	package = alloc_package(&size);
	fill_package(corewar, package);
	send(corewar->sock, package, size, 0);
	free(package);
}

/*
** Allocates and fill init package, that consists:
** First	amount of players							( int8_t )
** Next		info about 4 players, some may be empty		4 * (id + name), id - int8_t, name - 128 int8_t
** Next		CYCLE_DELTA									( int32_t )
** Next		NBR_LIVE									( int32_t )
** Next		MAX_CHECKS									( int32_t )
*/

void	send_init_package(t_corewar *corewar)
{
	uint8_t			*init_package;
	int32_t			size;
	int8_t			i;
	int32_t			package_iter;

	package_iter = 0;
	size = sizeof(uint8_t) +
	4 * (sizeof(uint8_t) + PROG_NAME_LENGTH * sizeof(uint8_t)) +
	3 * (sizeof(uint32_t));
	init_package = (uint8_t *)malloc(sizeof(uint8_t) * size);
	init_package[package_iter++] = corewar->players;
	i = -1;
	while (++i < 4)
	{
		if (i < corewar->players)
		{
			init_package[package_iter] = i + 1;
			ft_memcpy(init_package + package_iter + 1, g_header[i]->prog_name, PROG_NAME_LENGTH);
		}
		package_iter += PROG_NAME_LENGTH + 1;
	}
	ft_memcpy(init_package + package_iter, &((uint32_t){CYCLE_DELTA}), 4);
	ft_memcpy(init_package + (package_iter += 4), &((uint32_t){NBR_LIVE}), 4);
	ft_memcpy(init_package + (package_iter += 4), &((uint32_t){MAX_CHECKS}), 4);
	send(corewar->sock, (void *)init_package, size, 0);
	free(init_package);
}
