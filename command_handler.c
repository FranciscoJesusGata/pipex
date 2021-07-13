/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 17:25:24 by fgata-va          #+#    #+#             */
/*   Updated: 2021/07/09 10:45:01 by fgata-va         ###   ########.fr       */
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
	new->args = ft_split(cmd, ' ');
	new->binary = *new->args;
	new->path = NULL;
	new->input = 0;
	new->output = 1;
	new->next = NULL;
	return (new);
}

void	delete_cmd_lst(t_command *commands)
{
	t_command *nxt;

	while (commands)
	{
		nxt = commands->next;
		if (commands->input > 3)
			close(commands->input);
		if (commands->output > 3)
			close(commands->output);
		free_matrix(commands->args);
		if (commands->path)
			free(commands->path);
		free(commands);
		commands = nxt;
	}
}
