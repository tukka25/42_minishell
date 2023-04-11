/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_multiple_pipes_f.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdamoha <abdamoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 00:58:21 by abdamoha          #+#    #+#             */
/*   Updated: 2023/04/11 01:20:16 by abdamoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	before_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	if (c->pid == 0)
	{
		if (v->j == 0)
			first_cmd(c, p, v);
		else if (v->j == p->cmd_len - 1)
			third2_cmd(c, p, v);
		else
		{
			if (v->i % 2 == 1)
				fourth_cmd(c, p, v);
			else
				fifth_cmd(c, p, v);
			sixth_cmd(c, p, v);
		}
	}
}

void	init1(t_vars *v, t_pipe *c)
{
	v->i = 0;
	v->j = 0;
	v->h = 0;
	v->i = 0;
	v->d = 0;
	c->cmd_exec = NULL;
	c->cr = 1;
}

void	fifth_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	if (p[v->j].red_len > 0)
		check_exec_redirect(p, c, 1, v->j);
	if (input_check(p, c, v->j) == 0)
	{
		if (dup2(c->fd[1][0], STDIN_FILENO) == -1)
		{
			printf("error12\n");
			exit(0);
		}
	}
	if (output_check(p, c, v->j) == 0)
	{
		if (dup2(c->fd[0][1], STDOUT_FILENO) == -1)
		{
			printf("error14\n");
			exit(0);
		}
	}
}

void	exit_code_pipes(t_pipe *c)
{
	if (WIFEXITED(c->status))
		g_exit_code = WEXITSTATUS(c->status);
	else if (WIFSIGNALED(c->status))
		g_exit_code = WTERMSIG(c->status) + 128;
	else
		g_exit_code = 0;
}

void	child_exit(t_cmds *p, int j, t_pipe *c)
{
	g_exit_code = 127;
	write(2, p[j].cmd[0], ft_strlen(p[j].cmd[0]));
	write(2, ": command not found\n", 21);
	free_and_exit(c, p);
}
