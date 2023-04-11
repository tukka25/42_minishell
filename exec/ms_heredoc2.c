/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdamoha <abdamoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 03:58:54 by abdamoha          #+#    #+#             */
/*   Updated: 2023/04/11 04:03:07 by abdamoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	heredoc_exec(t_cmds *p, t_vars *v, int i)
{
	v->q = ft_strjoin(p[i].outs[v->k].file_name, "\n");
	if (ft_strcmp_heredoc(v->line, v->q) == 0)
	{
		break_condition(p, i, v);
		return (1);
	}
	ft_putstr_fd(v->line, v->tmp, 0);
	write(1, "> ", 2);
	free(v->line);
	v->line = get_next_line(0);
	free(v->q);
	if (!v->line)
		return (1);
	return (0);
}
