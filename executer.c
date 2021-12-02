/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 15:38:07 by fgata-va          #+#    #+#             */
/*   Updated: 2021/12/02 18:23:43 by fgata-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

char	*get_binary_path(char **path, char *binary)
{
	int		i;
	char	*exec_path;
	int		found;

	i = 1;
	found = -1;
	while (found == -1 && path[i])
	{
		exec_path = path_concat(path[i], binary);
		if (!exec_path)
			return (NULL);
		found = access(exec_path, X_OK);
		if (found < 0)
		{
			free(exec_path);
			exec_path = NULL;
			if (errno != ENOENT)
				return (exec_path);
		}
		i++;
	}
	return (exec_path);
}

void	redirect(int fds[2])
{
	if ((dup2(fds[0], STDIN_FILENO)) < 0)
		exit(program_error("pipex", 1, NULL));
	close(fds[0]);
	if ((dup2(fds[1], STDOUT_FILENO)) < 0)
		exit(program_error("pipex", 1, NULL));
	close(fds[1]);
}

char	*find_binary(char *name, char **path)
{
	char	*path_to_bin;

	path_to_bin = NULL;
	if (ft_strchr(name, '/'))
	{
		if ((access(name, X_OK)) < 0)
			exit(program_error(name, 127, NULL));
		return (name);
	}
	path_to_bin = get_binary_path(path, name);
	if (!path_to_bin)
		exit(program_error(name, 127, "command not found"));
	return (path_to_bin);
}

void	launch_processes(t_command *commands, char **path, char **envp)
{
	int	pid;

	while (commands)
	{
		pid = fork();
		if (!pid)
		{
			redirect(commands->fds);
			execve(find_binary(commands->argv[0], path), commands->argv, envp);
		}
		close(commands->fds[0]);
		close(commands->fds[1]);
		commands->pid = pid;
		commands = commands->next;
	}
}

int	executer(t_command **commands, char **path, char **envp)
{
	t_command	*iterator;
	int			exit_status;

	exit_status = 0;
	launch_processes(*commands, path, envp);
	iterator = *commands;
	while (iterator)
	{
		waitpid(iterator->pid, &exit_status, 0);
		iterator = iterator->next;
	}
	return (WEXITSTATUS(exit_status));
}
