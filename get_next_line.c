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

static char	*read_line(int fd, char *buffer, char *stash);
static char	*clean_line(char *line, char **stash);
static char	*read_buffer(int fd, char *buffer, char *line);
void		*ft_calloc(size_t elementCount, size_t elementSize);

char	*get_next_line(int fd)
{
	static char		*stash = NULL;
	char			*buffer;
	char			*line;
	char			*sub_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (NULL);
	line = read_line(fd, buffer, stash);
	if (!line)
	{
		free(buffer);
		if (stash)
		{
			free(stash);
			stash = NULL;
		}
		return (NULL);
	}
	sub_line = clean_line(line, &stash);
	if (!sub_line)
		return (NULL);
	return (sub_line);
}

static char	*read_buffer(int fd, char *buffer, char *line)
{
	char	*tmp;
	int		bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read <= 0)
	{
		free(line);
		return (NULL);
	}
	buffer[bytes_read] = '\0';
	tmp = line;
	line = ft_strjoin(line, buffer);
	free(tmp);
	return (line);
}

static char	*read_line(int fd, char *buffer, char *stash)
{
	char	*line;
	char	*tmp;

	line = ft_strdup("");
	if (stash)
	{
		tmp = line;
		line = ft_strjoin(stash, line);
		free(tmp);
	}
	while (!ft_strchr(buffer, '\n'))
	{
		line = read_buffer(fd, buffer, line);
		if (!line)
			return (NULL);
	}
	free(buffer);
	return (line);
}

static char	*clean_line(char *line, char **stash)
{
	char		*new_line;
	char		*new_stash;
	size_t		i;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	new_stash = ft_substr(line, i + 1, ft_strlen(line));
	new_line = ft_substr(line, 0, i);
	free(line);
	if (*stash)
		free(*stash);
	*stash = new_stash;
	return (new_line);
}

void	*ft_calloc(size_t elementCount, size_t elementSize)
{
	unsigned char	*ptr;
	size_t			i;

	if (elementSize * elementCount > SIZE_MAX)
		return (NULL);
	ptr = malloc(elementSize * elementCount);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < (elementCount * elementSize))
	{
		ptr[i] = 0;
		i++;
	}
	return (ptr);
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
// 		free(line);
// 	}

// 	close(fd);
// 	return (0);
// }