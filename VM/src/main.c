/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akorchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 23:43:59 by akorchyn          #+#    #+#             */
/*   Updated: 2019/02/26 16:36:43 by akorchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int8_t		check_ip(char *ip)
{
	int32_t		i;

	i = -1;
	if (!ip)
		return (0);
	while (ip[++i])
		if (!ft_isdigit(ip[i]) && ip[i] != '.')
			return (0);
	return (1);
}

int32_t		set_connection_to_visualization(char *ip, int8_t *i)
{
	int32_t					sock;
	struct sockaddr_in		server_address;
	char					*msg;
	int32_t					is_ip;

	is_ip = check_ip(ip);
	(is_ip) ? (*i)++ : 0;
	msg = "Connection is set!\n";
	sock = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(4242);
	server_address.sin_addr.s_addr = (is_ip) ? inet_addr(ip) : INADDR_ANY;
	if (connect(sock, (struct sockaddr *)&server_address,
			sizeof(server_address)) == -1)
	{
		ft_printf("Shit happened. You cannot connect to the server...");
		exit(EXIT_FAILURE);
	}
	send(sock, msg, sizeof(msg), 0);
	return (sock);
}

int32_t		main(int ac, char **av)
{
	t_corewar		corewar;
	t_dispatcher	dispatcher[16];
	t_header		*header[4];

	ft_bzero(&corewar, sizeof(corewar));
	parse_arguments(ac, av, &corewar);
	process_ids(corewar.carriages, corewar.players);
	!(corewar.players) ? print_usage(av) : 0;
	g_id = corewar.players;
	initializing(&corewar, header);
	initializing_dispatcher(dispatcher);
	if (!corewar.is_dump)
		cycle(&corewar, dispatcher, header);
	else
		dump_cycle(&corewar, dispatcher);
	return (0);
}
