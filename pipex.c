/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 10:41:58 by fgata-va          #+#    #+#             */
/*   Updated: 2021/12/01 18:55:32 by fgata-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

int	open_fds(char *infile, char *outfile, t_command *commands)
{
	int	readfile;
	int	writefile;

	readfile = open(infile, O_RDONLY);
	if (readfile < 0)
		return (program_error(infile, 1, NULL));
	if (commands->fds[0] >= 0)
		close(commands->fds[0]);
	commands->fds[0] = readfile;
	writefile = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (writefile < 0)
		return (program_error(outfile, 1, NULL));
	while (commands->next)
		commands = commands->next;
	if (commands->fds[1] >= 0)
		close(commands->fds[1]);
	commands->fds[1] = writefile;
	return (0);
}

t_command	*ft_tokenizer(int argc, char **argv)
{
	int			i;
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

void	initialize(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->commands = ft_tokenizer(argc, argv);
	pipex->path = get_path(envp);
	if (!pipex->path)
	{
		finish_program(pipex);
		exit(1);
	}
}

void	pipe_connect(t_command *commands)
{
	int	fds[2];

	while (commands->next)
	{
		pipe(fds);
		commands->fds[1] = fds[1];
		commands->next->fds[0] = fds[0];
		commands->next->prev = commands->fds;
		commands = commands->next;
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
		pipe_connect(pipex.commands);
		status = open_fds(argv[1], argv[argc - 1], pipex.commands);
		if (!status)
			status = executer(&pipex.commands, pipex.path, envp);
		else
			return (status);
		finish_program(&pipex);
	}
	else
		status = program_error("pipex", 1, "Invalid number of arguments");
	system("leaks pipex");
	system("lsof -c pipex");
	return (status);
}
