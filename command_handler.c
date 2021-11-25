/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 17:25:24 by fgata-va          #+#    #+#             */
/*   Updated: 2021/11/25 15:11:18 by fgata-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_command	*new_command(char *cmd)
{
	t_command	*new;

	if (!cmd)
		return (NULL);
	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
		return (NULL);
	new->argv = ft_split(cmd, ' ');
	new->argc = ft_strlen((void *)new->argv);
	new->path = NULL;
	new->fds[0] = -1;
	new->fds[1] = -1;
	new->next = NULL;
	return (new);
}

void	delete_cmd_lst(t_command *commands)
{
	t_command *nxt;

	while (commands)
	{
		nxt = commands->next;
		if (commands->fds[0] >= 0)
			close(commands->fds[0]);
		if (commands->fds[1] >= 0)
			close(commands->fds[1]);
		free_matrix(commands->argv);
		if (commands->path)
			free(commands->path);
		free(commands);
		commands = nxt;
	}
}

void	argv_printer(char **argv)
{
	write(1, "[", 1);
	while (*argv)
	{
		ft_putstr_fd(*argv, 1);
		if (*(argv + 1))
			write(1, ", ", 2);
		argv++;
	}
	write(1, "]\n", 2);
}

void	cmd_printer(t_command *commands)
{
	int	i;

	i = 0;
	while (commands)
	{
		write(1, "cmd", 3);
		ft_putnbr_fd(i, 1);
		write(1, ":\n" ,2);
		argv_printer(commands->argv);
		commands = commands->next;
		i++;
	}
}
