/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdamoha <abdamoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:20:54 by abdamoha          #+#    #+#             */
/*   Updated: 2023/04/10 23:47:31 by abdamoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(t_cmds *p, int i, int fd, t_pipe *c)
{
	int	j;

	j = 0;
	(void)fd;
	if (p->red_len > 0 && c->cr != 1)
		check_exec_redirect(p, c, 0, 0);
	if (!p[i].cmd[j + 1])
		return ;
	j = 1;
	while (p[i].cmd[j])
	{
		if (ft_isalpha_str(p[i].cmd[j]) == 0)
		{
			unset_cmp(&c->m_env, p[i].cmd[j], c);
			unset_cmp(&c->m_export, p[i].cmd[j], c);
		}
		else
			print_error(i, j, p);
		j++;
	}
	c->env_count -= 1;
}

char	*check_command_existence(char *av, char **path)
{
	int		i;
	char	*str;
	char	*join;
	int		j;

	j = 0;
	i = ft_strlen(av);
	if (i == 0)
		return (NULL);
	if (ft_strchr(av, '/') != NULL)
	{
		return (backslash_case(av, i));
	}
	str = malloc(i + 2);
	str[0] = '/';
	i = 1;
	while (av[j] != '\0' && av[i - 1] != ' ')
	{
		str[i] = av[j];
		j++;
		i++;
	}
	str[i] = '\0';
	join = check_with_access(path, str);
	return (join);
}

char	*check_with_access(char **path, char *str)
{
	int		i;
	char	*join;

	i = 0;
	join = NULL;
	if (path)
	{
		while (path[i] != NULL)
		{
			join = ft_strjoin(path[i], str);
			if (access(join, X_OK) != -1)
			{
				return (free(str), join);
			}
			i++;
			free(join);
			join = NULL;
		}
	}
	return (free(join), free(str), NULL);
}

char	*backslash_case(char *av, int i)
{
	int		j;
	char	*join;

	j = 0;
	join = malloc(i + 1);
	while (av[j] != '\0')
	{
		join[j] = av[j];
		j++;
	}
	join[j] = '\0';
	return (join);
}

void	last_sorting(t_pipe *p)
{
	t_list	*tmp;
	char	*str;

	tmp = p->m_export;
	while (tmp)
	{
		if (tmp->next)
		{
			if (strncmp_orginal(tmp->content, tmp->next->content,
					len_till_equal(tmp->content)) > 0)
			{
				str = tmp->content;
				tmp->content = tmp->next->content;
				tmp->next->content = str;
			}
		}
		tmp = tmp->next;
	}
}
