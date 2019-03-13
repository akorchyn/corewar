/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   name_comment.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlitvin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 15:23:25 by dmlitvin          #+#    #+#             */
/*   Updated: 2019/03/12 15:23:31 by dmlitvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

size_t		g_line;

int			line_not_clear(char const *start, char const *end)
{
	if (end - start > 1)
		while (start != end && *start != ALT_COMMENT_CHAR
			&& *start != COMMENT_CHAR)
			if (!ft_is_whitespace(start++))
				return (1);
	return (0);
}

static void	collect_name(char const *q1, char const *q2,
		const size_t content_size)
{
	if (g_header.prog_name[0])
		throw_error("double name declaration", g_line);
	else if (content_size > PROG_NAME_LENGTH)
		throw_error("too big bot name", g_line);
	else if (!content_size)
		throw_error("empty bot name", g_line);
	else
		ft_memcpy(g_header.prog_name, q1 + 1, q2 - q1 - 1);
}

static void	collect_comment(char const *q1, char const *q2,
		const size_t content_size)
{
	if (!content_size)
		throw_error("empty bot comment", g_line);
	else if (content_size > COMMENT_LENGTH)
		throw_error("too big bot comment", g_line);
	else if (g_header.comment[0])
		throw_error("double bot comment declaration", g_line);
	else
		ft_memcpy(g_header.comment, q1 + 1, q2 - q1 - 1);
}

static void	collect_directive(char *start, char *end)
{
	char	*quote1;
	char	*quote2;
	char	*str;

	str = get_clear_string(start, end);
	quote1 = ft_strchr(str, '\"');
	quote2 = (quote1) ? ft_strrchr(str, '\"') : NULL;
	if (!quote1 || quote1 == quote2)
		throw_error("bad name or comment", g_line);
	if (!ft_strncmp(str, NAME_CMD_STRING, ft_strlen(NAME_CMD_STRING))
			&& ft_is_whitespace(str + ft_strlen(NAME_CMD_STRING)))
		collect_name(quote1, quote2, quote2 - quote1 - 1);
	else if (!ft_strncmp(str, COMMENT_CMD_STRING, ft_strlen(COMMENT_CMD_STRING))
			&& ft_is_whitespace(str + ft_strlen(COMMENT_CMD_STRING)))
		collect_comment(quote1, quote2, quote2 - quote1 - 1);
	else
		throw_error("First two instructions must "
				"include bot name and comment!", 0);
	free(str);
}

char		*read_name_comment(char *start, uint8_t counter)
{
	char	*end;

	end = NULL;
	while (counter && start)
	{
		g_line++;
		end = ft_strchr(start, '\n');
		if (line_not_clear(start, end))
		{
			collect_directive(start, end);
			counter--;
		}
		start = (end) ? end + 1 : end;
	}
	if (!g_header.prog_name[0] || !g_header.comment[0])
		throw_error("First two instructions must "
					"include bot name and comment!", 0);
	return (end);
}