#include "../../incs/parse.h"
#include "../../libs/libmlx/incs/mlx.h"
#include <stdlib.h>
#include <fcntl.h>

t_bool			parse_texture(t_game *game, int fd);
static t_bool	set_texture_file_path(t_game *game, int fd);
static int		get_idx(char *dir);
static t_bool	xpm_to_img(t_game *game);

t_bool	parse_texture(t_game *game, int fd)
{
	game->space->walls = ft_calloc(sizeof(t_texture), 4, "");
	return (set_texture_file_path(game, fd) && \
		xpm_to_img(game)
	);
}

static t_bool	set_texture_file_path(t_game *game, int fd)
{
	t_texture *const	walls = game->space->walls;
	size_t				i;
	int					idx;
	char				*line;

	i = 0;
	while (i++ < 4)
	{
		line = get_next_line_which_is_not_empty(fd);
		if (line == NULL)
			return (FALSE);
		idx = get_idx(line);
		if (0 <= idx)
			walls[idx].file = ft_substr(line + 3, 0, ft_strlen(line + 3) - 1);
		free(line);
		if (idx < 0)
			game->err = IDENTIFIER_ERR;
		else if (check_file_extension(walls[idx].file, ".xpm") == FALSE)
			game->err = FILE_ERR;
		if (game->err != 0)
			return (FALSE);
	}
	return (TRUE);
}

static int	get_idx(char *dir)
{
	if (ft_strncmp(dir, "NO ", 3) == 0)
		return (0);
	else if (ft_strncmp(dir, "SO ", 3) == 0)
		return (1);
	else if (ft_strncmp(dir, "WE ", 3) == 0)
		return (2);
	else if (ft_strncmp(dir, "EA ", 3) == 0)
		return (3);
	return (-1);
}

static t_bool	xpm_to_img(t_game *game)
{
	t_texture	*wall;
	size_t		i;

	i = 0;
	while (i < 4)
	{
		wall = &game->space->walls[i];
		wall->img = ft_calloc(sizeof(t_img), 1, "");
		wall->img->img = mlx_xpm_file_to_image(
				game->mlx->mlx, wall->file,
				&wall->img->width, &wall->img->height);
		if (wall->img->img == NULL)
			game->err = FILE_ERR;
		else
			wall->img->data = (int *) mlx_get_data_addr(
					wall->img->img, &wall->img->bits_per_pixel,
					&wall->img->size_line, &wall->img->endian);
		if (wall->img->data == NULL)
			game->err = IMG_ERR;
		if (game->err != 0)
			return (FALSE);
		i++;
	}
	return (TRUE);
}
