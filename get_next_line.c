/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouras <abbouras@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:55:16 by abbouras          #+#    #+#             */
/*   Updated: 2024/11/25 13:10:51 by abbouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

char	*read_line(int fd, char *buffer, char *stash);
char	*clean_line(char *line, char **stash);

char	*get_next_line(int fd)
{
	static char		*stash = NULL;
	char			*buffer;
	char			*line;
	char			*sub_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	line = read_line(fd, buffer, stash);
	if (!line)
	{
		free(buffer);
		return (NULL);
	}
	sub_line = clean_line(line, &stash);
	if (!sub_line)
		return (NULL);
	return (sub_line);
}

char	*read_line(int fd, char *buffer, char *stash)
{
	int			bytes_read;
	char		*line;

	line = ft_strdup("");
	if (stash)
		line = ft_strjoin(stash, line);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == 0)
			return (NULL);
		line = ft_strjoin(line, buffer);
		if (!line)
			return (NULL);
		if (ft_strchr(buffer, '\n'))
		{
			free(buffer);
			return (line);
		}
	}
	return (line);
}

char	*clean_line(char *line, char **stash)
{
	size_t	i;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	*stash = ft_substr(line, i + 1, ft_strlen(line));
	line = ft_substr(line, 0, i);
	return (line);
}

// int main(void)
// {
// 	int     fd;
// 	char    *line;

// 	fd = open("file.txt", O_RDONLY);
// 	if (fd == -1)
// 	{
// 	  printf("Erreur d'ouverture du fichier\n");
// 	  return (1);
// 	}
// 	while ((line = get_next_line(fd)) != NULL)
// 	{
// 		printf("%s\n", line);
// 		free(line); // Assurez-vous de libérer la mémoire allouée
// 	}

// 	close(fd);
// 	return (0);
// }