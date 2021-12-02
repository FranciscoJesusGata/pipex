/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 10:41:58 by fgata-va          #+#    #+#             */
/*   Updated: 2021/12/02 18:41:49 by fgata-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

void	open_fds(char *infile, char *outfile, int is_heredoc,
		t_command *commands)
{
	int	readfile;
	int	writefile;

	if (BONUS && is_heredoc)
		readfile = heredoc(infile);
	else
		readfile = open(infile, O_RDONLY);
	if (commands->fds[0] >= 0)
		close(commands->fds[0]);
	commands->fds[0] = readfile;
	if (BONUS && is_heredoc)
		writefile = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		writefile = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (commands->next)
		commands = commands->next;
	if (commands->fds[1] >= 0)
		close(commands->fds[1]);
	commands->fds[1] = writefile;
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

void	initialize(t_pipex *pipex, int *argc, char ***argv, char **envp)
{
	pipex->path = get_path(envp);
	if (BONUS && !ft_strncmp("heredoc", (*argv)[1], ft_strlen((*argv)[1])))
	{
		pipex->heredoc = 1;
		(*argv)++;
		(*argc)--;
	}
	else
		pipex->heredoc = 0;
	if (!pipex->path)
	{
		finish_program(pipex);
		exit(1);
	}
	pipex->commands = ft_tokenizer(*argc, *argv);
}

void	pipe_connect(t_command *commands)
{
	int	fds[2];

	while (commands->next)
	{
		pipe(fds);
		commands->fds[1] = fds[1];
		commands->next->fds[0] = fds[0];
		commands = commands->next;
	}
}

int	main(int argc, char **argv, char *envp[])
{
	t_pipex		pipex;
	int			status;

	status = 0;
	if ((BONUS && argc >= 5) || argc == 5)
	{
		initialize(&pipex, &argc, &argv, envp);
		pipe_connect(pipex.commands);
		open_fds(argv[1], argv[argc - 1], pipex.heredoc, pipex.commands);
		status = executer(&pipex.commands, pipex.path, envp);
		finish_program(&pipex);
	}
	else
		status = program_error("pipex", 1, "Invalid number of arguments");
	return (status);
}
