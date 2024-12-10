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

char	*join_and_free(char *stash, char *buffer)
{
	char	*joined_str;

	joined_str = ft_strjoin(stash, buffer);
	if (!joined_str)
	{
		free(stash);
		return (NULL);
	}
	free(stash);
	return (joined_str);
}

char	*extract_stash(char *buffer)
{
	int		newline_pos;
	int		dest_index;
	char	*remaining_text;

	newline_pos = 0;
	while (buffer[newline_pos] && buffer[newline_pos] != '\n')
		newline_pos++;
	if (!buffer[newline_pos])
	{
		free(buffer);
		return (NULL);
	}
	remaining_text = ft_calloc(ft_strlen(buffer) - newline_pos + 1, 1);
	if (!remaining_text)
	{
		free(buffer);
		return (NULL);
	}
	newline_pos++;
	dest_index = 0;
	while (buffer[newline_pos])
		remaining_text[dest_index++] = buffer[newline_pos++];
	free(buffer);
	return (remaining_text);
}

char	*extract_current_line(char *buffer)
{
	char	*line;
	int		len;
	int		has_newline;

	if (!buffer[0])
		return (NULL);
	len = 0;
	has_newline = 0;
	while (buffer[len] && buffer[len] != '\n')
		len++;
	if (buffer[len] == '\n')
		has_newline = 1;
	line = ft_calloc(len + has_newline + 1, 1);
	if (!line)
		return (NULL);
	len = 0;
	while (buffer[len] && buffer[len] != '\n')
	{
		line[len] = buffer[len];
		len++;
	}
	if (has_newline)
		line[len] = '\n';
	return (line);
}

char	*read_and_store_buffer(int fd, char *stash)
{
	char	*buffer;
	int		bytes_read;

	if (!stash)
		stash = ft_calloc(1, 1);
	bytes_read = 1;
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (NULL);
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			free(stash);
			return (NULL);
		}
		buffer[bytes_read] = 0;
		stash = join_and_free(stash, buffer);
		if (ft_strchr(stash, '\n'))
			break ;
	}
	free(buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (stash)
		{
			free(stash);
			stash = NULL;
		}
		return (NULL);
	}
	stash = read_and_store_buffer(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_current_line(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = extract_stash(stash);
	return (line);
}

/*#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	char *line;

	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}*/
