/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 10:41:58 by fgata-va          #+#    #+#             */
/*   Updated: 2021/06/16 15:32:05 by fgata-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

int	main(int argc, char **argv, char *envp[])
{
	int	i;
	char	**cmd1;
	char	**path;
	int		pid;
	int		executed;

	i = 0;
	executed = -1;
	if (argc > 1)
	{
		printf("%d, %s\n", argc, *argv);
		while (ft_strncmp(envp[i], "PATH", 4))
			i++;
		path = ft_split(envp[i] + 5, ':');
		i = 0;
		while (path[i])
		{
			printf("%s ", path[i]);
			i++;
		}
		printf("\n");
		cmd1 = ft_split(argv[1], ' ');
		i = 1;
		while (cmd1[i])
		{
			printf("%s ", cmd1[i]);
			i++;
		}
		printf("\n");
		pid = fork();
		if (pid != 0)
		{
			i = 0;
			while (executed < 0 && path[i])
			{
				ft_strlcat(path[i], "/", 30);
				ft_strlcat(path[i], cmd1[0], 30);
				printf("%s %s\n", path[i], cmd1[0]);
				executed = execve(path[i], cmd1 + 1, envp);
				i++;
			}
			if (executed < 0)
				perror("execve");
		}
		return (0);
	}
	return (1);
}
