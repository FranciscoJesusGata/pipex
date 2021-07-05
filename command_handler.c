/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 17:25:24 by fgata-va          #+#    #+#             */
/*   Updated: 2021/06/25 17:52:47 by fgata-va         ###   ########.fr       */
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
	new->input = -1;
	new->output = -1;
	new->next = NULL;
	return (new);
}

