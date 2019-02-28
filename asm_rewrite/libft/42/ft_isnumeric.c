/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnumeric.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlitvin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/06 23:12:40 by dmlitvin          #+#    #+#             */
/*   Updated: 2019/02/06 23:14:21 by dmlitvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_isnumeric(char *str)
{
	while (*str && *str != ' ')
		if (!(ft_isdigit(*str++)))
			return (0);
	return (1);
}
