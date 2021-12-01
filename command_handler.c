/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 17:25:24 by fgata-va          #+#    #+#             */
/*   Updated: 2021/12/01 18:54:58 by fgata-va         ###   ########.fr       */
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
	new->fds[0] = -1;
	new->fds[1] = -1;
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

void	delete_cmd_lst(t_command *commands)
{
	t_command	*nxt;

	while (commands)
	{
		nxt = commands->next;
		close(commands->fds[0]);
		close(commands->fds[1]);
		free_matrix(commands->argv);
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
		write(1, ":\n", 2);
		argv_printer(commands->argv);
		ft_putstr_fd("fds: ", 1);
		ft_putnbr_fd(commands->fds[0], 1);
		write(1, " ", 1);
		ft_putnbr_fd(commands->fds[1], 1);
		write(1, "\n", 1);
		commands = commands->next;
		i++;
	}
}
