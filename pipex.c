/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 10:41:58 by fgata-va          #+#    #+#             */
/*   Updated: 2021/06/18 12:55:23 by fgata-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

char	**get_path(char **envp)
{
	while (ft_strncmp(*envp, "PATH", 4))
		envp++;
	return (ft_split(*envp + 5, ':'));
}

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

int	ft_executor(char **cmd, char **path, char **envp)
{
	int		pid;
	int		executed;
	int		i;
	int		status;

	executed = -1;
	status = 0;
	pid = fork();
	if (pid == 0)
	{
		i = 0;
		while (executed == -1 && path[i])
		{
			ft_strlcat(path[i], "/", 30);
			ft_strlcat(path[i], cmd[0], 30);
			executed = execve(path[i], cmd, envp);
			i++;
		}
		if (path[i] != NULL)
			perror(cmd[0]);
		else
		{
			ft_putstr_fd("command not found\n", 1);
			status = 127;
		}
		exit(status);
	}
	else
	{
		waitpid(pid, &status, WNOHANG);
		while (status == 0)
			waitpid(pid, &status, WNOHANG);
	}
	return (status / 256);
}

int	main(int argc, char **argv, char *envp[])
{
	int	i;
	char	**cmd1;
	char	**path;
	int		status;

	i = 0;
	status = 0;
	if (argc > 1)
	{
		path = get_path(envp);
		cmd1 = ft_split(argv[1], ' ');
		status = ft_executor(cmd1, path, envp);
		free_matrix(path);
		free(cmd1);
	}
	printf("%d\n", status);
	return (status);
}
