/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 10:41:58 by fgata-va          #+#    #+#             */
/*   Updated: 2021/07/13 13:27:14 by fgata-va         ###   ########.fr       */
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
		found = access(exec_path, 1);
		if (found < 0)
		{
			free(exec_path);
			exec_path = NULL;
		}
		i++;
	}
	return (exec_path);
}

void	finish_program(t_pipex *pipex)
{
	delete_cmd_lst(pipex->commands);
	free_matrix(pipex->path);
}

void	redirect(char *filename, int fd, int io, t_pipex *pipex)
{
	if (fd < 0 || (dup2(fd, io) < 0))
	{
		finish_program(pipex);
		exit(program_error(filename, 1, strerror(errno)));
	}
}

t_command	*ft_tokenizer(int argc, char **argv)
{
	int	i;
	t_command	*commands;
	t_command	*current;

	i = 2;
	commands = new_command(argv[i]);
	current = commands;
	i++;
	while (i < argc - 1)
	{
		current->next = new_command(argv[i]);
		current = current->next;
		if (!current)
		{
			delete_cmd_lst(commands);
			exit(program_error("pipex", 1, "Allocation error"));
		}
		i++;
	}
	return (commands);
}

void	ft_parser(t_pipex *pipex)
{
	int	in_fd;
	int	out_fd;

	in_fd = open(pipex->infile, O_RDONLY);
	//Search binary
	out_fd = open(pipex->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0622);
}

void	initialize(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->commands = ft_tokenizer(argc, argv);
	pipex->path = get_path(envp);
	pipex->infile = argv[1];
	pipex->outfile = argv[argc - 1];
	if (!pipex->path)
	{
		finish_program(pipex);
		exit(1);
	}
}

int	main(int argc, char **argv, char *envp[])
{
	int			i;
	t_pipex		pipex;
	int			status;

	i = 0;
	status = 0;
	if (argc == 5)
	{
		initialize(&pipex, argc, argv, envp);
		//redirect(argv[1], open(argv[1], O_RDONLY), 0);
		finish_program(&pipex);
	}
	else
		status = program_error("pipex", 1, "Invalid number of arguments");
	system("leaks pipex");
	return (status);
}
