/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_multiple_pipe.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdamoha <abdamoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 16:00:16 by abdamoha          #+#    #+#             */
/*   Updated: 2023/04/07 05:35:58 by abdamoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	first_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	if (p[v->j].red_len > 0)
		check_exec_redirect(p, c, 1, v->j);
	else
		dup2(c->fd[0][1], STDOUT_FILENO);
	if (input_check(p, c, v->j))
		dup2(c->fd[0][1], STDOUT_FILENO);
	if (builtins_pipes(p, c, c->fd[0][1], v->j) == 0)
		free_and_exit(c, p);
	close(c->fd[0][1]);
	close(c->fd[0][0]);
	c->cmd_exec = check_command_existence(p[v->j].cmd[0], c->m_path);
	if (!c->cmd_exec)
	{
		g_exit_code = 127;
		free_and_exit(c, p);
	}
	if (execve(c->cmd_exec, p[v->j].cmd, NULL) < 0)
	{
		write(2, p[v->j].cmd[0], ft_strlen(p[v->j].cmd[0]));
		write(2, ": command not found\n", 21);
		free(c->cmd_exec);
		// closing_fds(c);
		g_exit_code = 126;
		free_and_exit(c, p);
	}
}
static void	second_cmd(t_pipe *c, t_vars *v, t_cmds *p)
{
	if (v->i % 2 == 0 && v->j == 1 && input_check(p, c, v->j) == 0)
	{
		if (dup2(c->fd[0][0], STDIN_FILENO) == -1)
		{
			printf("error3\n");
			exit(0);
		}
		close(c->fd[0][0]);
		close(c->fd[0][1]);
	}
	else if (v->i % 2 == 1 && input_check(p, c, v->j) == 0)
	{
		if (dup2(c->fd[0][0], STDIN_FILENO) == -1)
		{
			printf("error3\n");
			exit(0);
		}
		close(c->fd[0][1]);
		close(c->fd[0][0]);
	}
	else
	{
		if (input_check(p, c, v->j) == 0)
			dup2(c->fd[1][0], STDIN_FILENO);
		close(c->fd[1][1]);
		close(c->fd[1][0]);
	}
}
static void	third2_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	if (p[v->j].red_len > 0)
	{
		// printf("v->j = %d\n", v->j);
		check_exec_redirect(p, c, 1, v->j);
		// printf("g = %d\n", g_exit_code);
	}
	// else
		second_cmd(c, v, p);
	if (builtins_pipes(p, c, 1, v->j) == 0)
		free_and_exit(c, p);
	c->cmd_exec = check_command_existence(p[v->j].cmd[0], c->m_path);
	if (!c->cmd_exec)
	{
		g_exit_code = 127;
		free_and_exit(c, p);
	}
	else
	{
		close(c->fdin);
		close(c->fdout);
	}
	if (execve(c->cmd_exec, p[v->j].cmd, NULL) < 0)
	{
		write(2, p[v->j].cmd[0], ft_strlen(p[v->j].cmd[0]));
		write(2, ": command not found\n", 21);
		free(c->cmd_exec);
		g_exit_code = 126;
		free_and_exit(c, p);
	}		
}
static void	fourth_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	if (p[v->j].red_len > 0)
		check_exec_redirect(p, c, 1, v->j);
	if (input_check(p, c, v->j) == 0)
	{
		dup2(c->fd[0][0], STDIN_FILENO);
		if (dup2(c->fd[1][1], STDOUT_FILENO) == -1)
		{
			printf("error8\n");
			exit(0);
		}
	}
	else
	{
		if (input_check(p, c, v->j) == 0)
		{
			dup2(c->fd[0][0], STDIN_FILENO);
			printf("error9\n");
			exit(0);
		}
		if (dup2(c->fd[1][1], STDOUT_FILENO) == -1)
		{
				printf("error10\n");
				exit(0);
		}
		close(c->fd[0][0]);
		close(c->fd[1][1]);
	}
}
static void	fifth_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	if (p[v->j].red_len > 0)
		check_exec_redirect(p, c, 1, v->j);
	// else
	// {
		if (dup2(c->fd[1][0], STDIN_FILENO) == -1)
		{
				printf("error12\n");
				exit(0);
		}
		if (dup2(c->fd[0][1], STDOUT_FILENO) == -1)
		{
				printf("error14\n");
				exit(0);
		}
	// }
}
static void	sixth_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	closing_fds(c);
	if (builtins_pipes(p, c, c->fd[0][1], v->j) == 0)
		free_and_exit(c, p);
	c->cmd_exec = check_command_existence(p[v->j].cmd[0], c->m_path);
	if (!c->cmd_exec)
	{
		g_exit_code = 127;
		free_and_exit(c, p);
	}
	if (execve(c->cmd_exec, p[v->j].cmd, NULL) < 0)
	{
		write(2, p[v->j].cmd[0], ft_strlen(p[v->j].cmd[0]));
		write(2, ": command not found\n", 21);
		free(c->cmd_exec);
		g_exit_code = 126;
		free_and_exit(c, p);
	}
}
static void	closing_pipe(t_pipe *c, t_cmds *p, t_vars *v)
{
		if (v->i % 2 == 1 && v->i != 0)
		{
			close(c->fd[0][0]);
			close(c->fd[0][1]);
			if (v->j == p->cmd_len - 1)
			{
				close(c->fd[1][1]);
				close(c->fd[1][0]);
			}
			v->i = -1;
			c->p_f1 = 0;
		}
		else if (v->i % 2 == 0 && v->j != 0)
		{
			close(c->fd[1][0]);
			close(c->fd[1][1]);
			v->i = 0;
			c->p_f2 = 0;
		}
}

static void	before_cmd(t_pipe *c, t_cmds *p, t_vars *v)
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


static void	init1(t_vars *v)
{
	v->i = 0;
	v->j = 0;
	v->h = 0;
	v->i = 0;
	v->d = 0;
}

void	multiple_pipes(t_cmds *p, t_pipe *c)
{
	t_vars v;
	init1(&v);
	c->cmd_exec = NULL;
	c->cr = 1;
	while (v.j < p->cmd_len)
	{
		if (v.i % 2 == 0 || v.i == 0)
		{
			c->p_f1 = 1;
			pipe(c->fd[0]);
		}
		else if (v.i % 2 == 1)
		{
			c->p_f2 = 1;
			pipe(c->fd[1]);
		}
		if (check_heredoc(p, c) == 1)
			exec_heredoc(p, c, v.j);
		c->pid = fork();
		before_cmd(c, p, &v);
		closing_pipe(c, p, &v);
		v.i++;
		v.j++;
	}
	v.h = 0;
	// closing_fds(c);
	while (v.h < p->cmd_len)
	{
		wait(NULL);
		v.h++;
	}
	c->cr = 0;
	c->p_f1 = 0;
	c->p_f2 = 0;
}


// void	sec_cmd(t_pipe *c, int i, int j)
// {
// 	if (c->fd2 > 2)
// 	{
// 		dup2(c->fd2, STDIN_FILENO);
// 		close(c->fd2);
// 	}
// 	else
// 	{
// 		if (c->v.i % 2 == 0 && c->v.j == 1)
// 		{
// 			dup2(c->fd[0][0], STDIN_FILENO);
// 			closing_fds(c);
// 		}
// 		else if (c->v.i % 2 == 1)
// 		{
// 			dup2(c->fd[0][0], STDIN_FILENO);
// 			close(c->fd[0][1]);
// 			close(c->fd[0][0]);
// 		}
// 		else
// 		{
// 			dup2(c->fd[1][0], STDIN_FILENO);
// 			close(c->fd[1][1]);
// 			close(c->fd[1][0]);
// 		}
// 	}
// }

// void	third_cmd(t_cmds *p, t_pipe *c, int j)
// {
// 	if (p[v.j].red_len > 0)
// 	{
// 		c->fd2 = check_input_redirect(p, c);
// 		if (c->fd2 > 2)
// 		{
// 			dup2(c->fd2, STDIN_FILENO);
// 			close(c->fd2);
// 		}
// 		else
// 			dup2(c->fd[0][0], STDIN_FILENO);
// 		c->fd2 = check_exec_rederict(p, c);
// 		if (c->fd2 > 2)
// 		{
// 			dup2(c->fd2, STDOUT_FILENO);
// 			close(c->fd2);
// 		}
// 		else
// 			dup2(c->fd[1][1], STDOUT_FILENO);
// 	}
// 	else
// 	{
// 		dup2(c->fd[0][0], STDIN_FILENO);
// 		dup2(c->fd[1][1], STDOUT_FILENO);
// 		close(c->fd[0][0]);
// 		close(c->fd[1][1]);
// 	}
// }

	// c->c->v.i = 0;
	// c->c->v.j = 0;
	// c->c->v.h = 0;
	// c->c->c->cmd_exec = NULL;
	// while (c->c->v.j < p->cmd_len)
	// {
	// 	if (c->c->v.i % 2 == 0 || c->c->v.i == 0)
	// 		pipe(c->fd[0]);
	// 	else if (c->c->v.i % 2 == 1)
	// 		pipe(c->fd[1]);
	// 	if (check_heredoc(p, c) == 1)
	// 		exec_heredoc(p, c, v.j);
	// 	c->pid = fork();
	// 	if (c->pid == 0)
	// 	{
	// 		if (c->c->v.j == 0)
	// 		{
	// 			c->fd2 = check_input_redirect(p, c);
	// 			if (c->fd2 > 2)
	// 			{
	// 				dup2(c->fd2, STDIN_FILENO);
	// 				close(c->fd2);
	// 			}
	// 			dup2(c->fd[0][1], STDOUT_FILENO);
	// 			close(c->fd[0][1]);
	// 			close(c->fd[0][0]);
	// 			c->c->c->cmd_exec = check_command_existence(p[v.j].cmd[0], c->m_path);
	// 			if (execve(c->c->c->cmd_exec, p[v.j].cmd, NULL) < 0)
	// 			{
	// 				write(2, p[0].cmd[0], ft_strlen(p[0].cmd[0]));
	// 				write(2, ": command not found\n", 21);
	// 				free(c->c->c->cmd_exec);
	// 				closing_fds(c);
	// 				free_and_exit(c, p);
	// 			}
	// 		}
	// 		else if (c->c->v.j == p->cmd_len - 1)
	// 		{
	// 			c->fd2 = check_input_redirect(p, c);
	// 			if (c->fd2 > 2)
	// 			{
	// 				dup2(c->fd2, STDIN_FILENO);
	// 				close(c->fd2);
	// 			}
	// 			else
	// 			{
	// 				if (c->c->v.i % 2 == 0 && c->c->v.j == 1)
	// 				{
	// 					dup2(c->fd[0][0], STDIN_FILENO);
	// 					closing_fds(c);
	// 				}
	// 				else if (c->c->v.i % 2 == 1)
	// 				{
	// 					dup2(c->fd[0][0], STDIN_FILENO);
	// 					close(c->fd[0][1]);
	// 					close(c->fd[0][0]);
	// 				}
	// 				else
	// 				{
	// 					dup2(c->fd[1][0], STDIN_FILENO);
	// 					close(c->fd[1][1]);
	// 					close(c->fd[1][0]);
	// 				}
	// 			}
	// 			c->fd1 = check_exec_rederict(p, c);
	// 			if (c->fd1 > 2)
	// 			{
	// 				dup2(c->fd1, STDOUT_FILENO);
	// 				close(c->fd1);
	// 			}
	// 			c->c->c->cmd_exec = check_command_existence(p[v.j].cmd[0], c->m_path);
	// 			if (execve(c->c->c->cmd_exec, p[v.j].cmd, NULL) < 0)
	// 			{
	// 				write(2, p[0].cmd[0], ft_strlen(p[0].cmd[0]));
	// 				write(2, ": command not found\n", 21);
	// 				free(c->c->c->cmd_exec);
	// 				closing_fds(c);
	// 				free_and_exit(c, p);
	// 			}		
	// 		}
	// 		else
	// 		{
	// 			if (c->c->v.i % 2 == 1)
	// 			{
	// 				if (p[v.j].red_len > 0)
	// 				{
	// 					c->fd2 = check_input_redirect(p, c);
	// 					if (c->fd2 > 2)
	// 					{
	// 						dup2(c->fd2, STDIN_FILENO);
	// 						close(c->fd2);
	// 					}
	// 					else
	// 						dup2(c->fd[0][0], STDIN_FILENO);
	// 					c->fd2 = check_exec_rederict(p, c);
	// 					if (c->fd2 > 2)
	// 					{
	// 						dup2(c->fd2, STDOUT_FILENO);
	// 						close(c->fd2);
	// 					}
	// 					else
	// 						dup2(c->fd[1][1], STDOUT_FILENO);
	// 				}
	// 				else
	// 				{
	// 					dup2(c->fd[0][0], STDIN_FILENO);
	// 					dup2(c->fd[1][1], STDOUT_FILENO);
	// 					close(c->fd[0][0]);
	// 					close(c->fd[1][1]);
	// 				}
	// 			}
	// 			else
	// 			{
	// 				c->fd2 = check_input_redirect(p, c);
	// 				if (c->fd2 > 2)
	// 				{
	// 					dup2(c->fd2, STDIN_FILENO);
	// 					close(c->fd2);
	// 				}
	// 				else
	// 					dup2(c->fd[1][0], STDIN_FILENO);
	// 				c->fd2 = check_exec_rederict(p, c);
	// 				if (c->fd2 > 2)
	// 				{
	// 					dup2(c->fd2, STDOUT_FILENO);
	// 					close(c->fd2);
	// 				}
	// 				else
	// 					dup2(c->fd[0][1], STDOUT_FILENO);
	// 			}
	// 			closing_fds(c);
	// 			c->c->c->cmd_exec = check_command_existence(p[v.j].cmd[0], c->m_path);
	// 			if (execve(c->c->c->cmd_exec, p[v.j].cmd, NULL) < 0)
	// 			{
	// 				write(2, p[0].cmd[0], ft_strlen(p[0].cmd[0]));
	// 				write(2, ": command not found\n", 21);
	// 				free(c->c->c->cmd_exec);
	// 				closing_fds(c);
	// 				free_and_exit(c, p);
	// 			}
	// 		}
	// 	}
	// 	if (c->c->v.i % 2 == 1 && c->c->v.i != 0)
	// 	{
	// 		// close_first_pipe(p, c);
	// 		close(c->fd[0][0]);
	// 		close(c->fd[0][1]);
	// 		if (c->c->v.j == p->cmd_len - 1)
	// 		{
	// 			close(c->fd[1][1]);
	// 			close(c->fd[1][0]);
	// 		}
	// 		c->c->v.i = -1;
	// 	}
	// 	else if (c->c->v.i % 2 == 0 && c->c->v.j != 0)
	// 	{
	// 		// close_second_pipe(c);
	// 		close(c->fd[1][0]);
	// 		close(c->fd[1][1]);
	// 		c->c->v.i = 0;
	// 	}
	// 	v.i++;
	// 	v.j++;
	// }
	// c->c->v.h = 0;
	// closing_fds(c);
	// while (c->c->v.h < p->cmd_len)
	// {
	// 	wait(NULL);
	// 	v.h++;
	// }