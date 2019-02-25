/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command6_10.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akorchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 16:42:15 by akorchyn          #+#    #+#             */
/*   Updated: 2019/02/25 17:38:39 by akorchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int32_t			g_id;

void		and(t_carriage *carriage, t_corewar *corewar, t_vars *vars)
{
	int8_t		i;
	int64_t		values[2];

	i = -1;
	if (bad_register_id(vars, carriage))
		return ;
	while (++i < 2)
	{
		if (vars->parsed_codage[i] == T_REG)
			values[i] = carriage->reg[vars->vars[i] - 1];
		else if (vars->parsed_codage[i] == T_DIR)
			values[i] = vars->vars[i];
		else if (vars->parsed_codage[i] == T_IND)
			values[i] = bytes_to_dec(corewar->map,
					shift(carriage, vars->vars[i]), 4);
	}
	carriage->reg[vars->vars[2] - 1] = values[0] & values[1];
	carriage->carry = carriage->reg[vars->vars[2] - 1] == 0 ? 1 : 0;
	//ft_printf("P%5d | and %hd %hd r%hd\n", carriage->p_number, values[0], values[1], vars->vars[2]);
}

void		or(t_carriage *carriage, t_corewar *corewar, t_vars *vars)
{
	int8_t		i;
	int64_t		values[2];

	i = -1;
	if (bad_register_id(vars, carriage))
		return ;
	while (++i < 2)
	{
		if (vars->parsed_codage[i] == T_REG)
			values[i] = carriage->reg[vars->vars[i] - 1];
		else if (vars->parsed_codage[i] == T_DIR)
			values[i] = vars->vars[i];
		else if (vars->parsed_codage[i] == T_IND)
			values[i] = bytes_to_dec(corewar->map,
									 shift(carriage, vars->vars[i]), 4);
	}
	carriage->reg[vars->vars[2] - 1] = values[0] | values[1];
	carriage->carry = carriage->reg[vars->vars[2] - 1] == 0 ? 1 : 0;
	ft_printf("P%5d | or %hd %hd r%hd\n", carriage->p_number, values[0], values[1], vars->vars[2]);
}

void		xor(t_carriage *carriage, t_corewar *corewar, t_vars *vars)
{
	int8_t		i;
	int64_t		values[2];

	i = -1;
	if (bad_register_id(vars, carriage))
		return ;
	while (++i < 2)
	{
		if (vars->parsed_codage[i] == T_REG)
			values[i] = carriage->reg[vars->vars[i] - 1];
		else if (vars->parsed_codage[i] == T_DIR)
			values[i] = vars->vars[i];
		else if (vars->parsed_codage[i] == T_IND)
			values[i] = bytes_to_dec(corewar->map,
									 shift(carriage, vars->vars[i]), 4);
	}
	carriage->reg[vars->vars[2] - 1] = values[0] ^ values[1];
	carriage->carry = carriage->reg[vars->vars[2] - 1] == 0 ? 1 : 0;
	ft_printf("P%5d | xor %hd %hd r%hd\n", carriage->p_number, values[0], values[1], vars->vars[2]);
}

void		zjmp(t_carriage *carriage, t_corewar *corewar, t_vars *vars)
{
	UNUSED_VARIABLE(corewar);
	if (carriage->carry)
	{
		ft_printf("P%5d | zjmp %hd OK\n",carriage->p_number, vars->vars[0]);
		if (vars->vars[0] != 0)
			vars->skip = -1;
		carriage->counter = shift(carriage, vars->vars[0]);
	}
	else
		ft_printf("P%5d | zjmp %hd FAILED\n", carriage->p_number, vars->vars[0]);
}

void		ldi(t_carriage *carriage, t_corewar *corewar, t_vars *vars)
{
	int32_t		values[2];

	if (bad_register_id(vars, carriage))
		return ;
	if (vars->parsed_codage[0] == T_REG)
		values[0] = carriage->reg[vars->vars[0] - 1];
	else if (vars->parsed_codage[0] == T_IND)
		values[0] = bytes_to_dec(corewar->map, shift(carriage, vars->vars[0]),
														REG_SIZE);
	else
		values[0] = vars->vars[0];
	values[1] = (vars->parsed_codage[1] == T_REG) ?
				carriage->reg[vars->vars[1] - 1] : vars->vars[1];
	carriage->reg[vars->vars[2] - 1] = bytes_to_dec(corewar->map,
		shift(carriage, (int16_t )values[0] + (int16_t )values[1]), REG_SIZE);
	ft_printf("P%5d | ldi %hd %hd r%hd\n       | -> load from %hd + %hd = %hd (with pc and mod %hd)\n", carriage->p_number, values[0], values[1], vars->vars[2], values[0], values[1], values[0] + values[1], carriage->counter + values[0] + values[1]);
}