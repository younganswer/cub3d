#include "../../incs/parse.h"
#include "../../libs/libgnl/incs/get_next_line.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

t_bool	parse(t_game *game, char *file);
char	*get_next_line_which_is_not_empty(int fd);
t_bool	check_file_extension(char *file, char *ext);

t_bool	parse(t_game *game, char *file)
{
	const int	fd = open(file, O_RDONLY);
	t_bool		ret;

	ret = FALSE;
	if (fd == -1)
		return (ret);
	game->space = ft_calloc(sizeof(t_space), 1, "");
	if (parse_texture(game, fd) && \
		parse_floor_ceiling(game, fd) && \
		parse_map(game, fd))
		ret = TRUE;
	close(fd);
	return (ret);
}

char	*get_next_line_which_is_not_empty(int fd)
{
	char	*ret;

	ret = get_next_line(fd);
	if (ret == NULL)
		return (NULL);
	while (*ret == '\n')
	{
		free(ret);
		ret = get_next_line(fd);
		if (ret == NULL)
			return (NULL);
	}
	return (ret);
}

t_bool	check_file_extension(char *file, char *ext)
{
	size_t	len;
	size_t	ext_len;

	len = ft_strlen(file);
	ext_len = ft_strlen(ext);
	if (len < ext_len)
		return (FALSE);
	return (ft_strncmp(file + len - ext_len, ext, ext_len) == 0);
}
