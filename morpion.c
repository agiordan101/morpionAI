//IA pour un morpion en arbre

#include "libft/libft.h"
#include <stdio.h>

/*# define TRUE 1
# define FALSE 0
typedef	int	boolean;*/

typedef struct	s_morpion
{
	char	turn;
	char	**grid;
	char	**savegrid;
	char	*line;
	int	size;
	int	AI;
	int	game;
}		t_morpion;

typedef struct	s_AI
{
	int	step;
	int	nchoice;
	int	*choices;
}		t_AI;

void	init(t_morpion *var)
{
	int	i;
	
	var->size = 3;
	var->grid = (char **)malloc(sizeof(char *) * (var->size + 1));
	i = -1;
	while (++i < var->size)
	{
		var->grid[i] = ft_strnew(var->size);
		ft_strfill(var->grid[i], ' ', var->size);
	}
	var->grid[var->size] = NULL;
	var->line = NULL;
	var->choice = 0;
	var->game = 1;
}

void	ft_print_grid(t_morpion *var)
{
	ft_putchar(var->grid[0][0]);
	ft_putchar('|');
	ft_putchar(var->grid[0][1]);
	ft_putchar('|');
	ft_putchar(var->grid[0][2]);
	ft_putstr("\n-----\n");
	ft_putchar(var->grid[1][0]);
	ft_putchar('|');
	ft_putchar(var->grid[1][1]);
	ft_putchar('|');
	ft_putchar(var->grid[1][2]);
	ft_putstr("\n-----\n");
	ft_putchar(var->grid[2][0]);
	ft_putchar('|');
	ft_putchar(var->grid[2][1]);
	ft_putchar('|');
	ft_putchar(var->grid[2][2]);
	ft_putstr("\n\n");
}

int	ft_n_empty(char **grid)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = -1;
	while (grid[++i])
	{
		j = -1;
		while (grid[i][++j])
			if (grid[i][j] == ' ')
				count++;
	}
	return (count);
}

int	ft_end(char **grid)
{
	if (grid[0][0] == grid[0][1] && grid[0][0] == grid[0][2] && grid[0][0] != ' ')
		return (1);
	if (grid[1][0] == grid[1][1] && grid[1][0] == grid[1][2] && grid[1][0] != ' ')
		return (1);
	if (grid[2][0] == grid[2][1] && grid[2][0] == grid[2][2] && grid[2][0] != ' ')
		return (1);
	if (grid[0][0] == grid[1][0] && grid[0][0] == grid[2][0] && grid[0][0] != ' ')
		return (1);
	if (grid[0][1] == grid[1][1] && grid[0][1] == grid[2][1] && grid[0][1] != ' ')
		return (1);
	if (grid[0][2] == grid[1][2] && grid[0][2] == grid[2][2] && grid[0][2] != ' ')
		return (1);
	if (grid[0][0] == grid[1][1] && grid[0][0] == grid[2][2] && grid[0][0] != ' ')
		return (1);
	if (grid[0][2] == grid[1][1] && grid[0][2] == grid[2][0] && grid[0][2] != ' ')
		return (1);
	return (0);
}

void	fill_i(char ***grid, int n, char turn)
{
	int i;
	int j;
	int count;

	count = 1; 
	i = -1;
	while (*grid[++i])
	{
		j = -1;
		while (*grid[i][++j])
		{
			if (*grid[i][j] == ' ' && count == n)
				*grid[i][j] = turn;
			else if (*grid[i][j] == ' ')
				count++;
		}	
	}
}

char	**ft_gridcpy(char **grid)
{
	char	**savegrid;
	int	i;

	savegrid = (char **)malloc(sizeof(char *) * (3 + 1));
	savegrid[3] = NULL;
	i = -1;
	while (++i < 3)
	{
		savegrid[i] = ft_strnew(3);
		ft_strcpy(savegrid[i], grid[i]);
	}
	return (savegrid);
}

void	ft_griddel(char ***grid)
{
	int	i;

	i = -1;
	while (++i < 3)
		ft_strdel(*grid + i);
	free(*grid);
	grid = NULL;
}

int	ft_eval(char **grid)
{
	int	grade;
	int	sign;
	int	i;
	int	j;

	grade = 0;
	i = -1;
	while (++i < 3)
	{
		j = -1;
		while (++j < 3)
		{
			sign = (grid[i][j] == 'O' ? -1 : 1);
			if (i == 0 && (j == 0 || j == 2) || i == 2 && (j == 0 || j == 2))
				grade += 3 * sign;
			else if (j == 1 && (i == 0 || i == 2) || i == 1 && (j == 0 || j == 2))
				grade += 2 * sign;
			else
				grade += 5* sign;
		}
	}
	return (grade);
}

int	ft_previsions(t_morpion *var, char **grid, t_AI *AI, int choice, char turn)
{
	char	**savegrid;
	int	npos;
	int	sum;
	int	i;

	turn = (turn == 'O' ? 'X' : 'O');
	AI->step++;
	npos = ft_n_empt(grid);
	savegrid = ft_gridcpy(grid);
	sum = 0;
	i = -1;
	if (AI->step == var->AI)
		sum = ft_eval(grid);
	else
		while (++i < npos)
		{
			fill_i(grid, i, turn);
			if (ft_end(grid))
				sum += (turn == 'O' ? -50 : 50);
			else
				sum += ft_previsions(var, grid, AI, i, turn);	
			ft_griddel(&grid);
			grid = ft_gridcpy(savegrid);
		}
	AI->step--;
	return (sum);
}

void	ft_AI_turn(t_morpion *var, t_AI *AI)
{
	int	i;
	int	best;
	int	besti;

	if (AI->savegrid)
		ft_griddel(&AI->savegrid);	
	AI->savegrid = ft_gridcpy(AI->grid);
	best = 0;
	i = -1;
	while (++i < AI->nchoice)
	{
		//printf(step);
		AI->step = -1;
		fill_i(&var->grid, i, 'X');
		if (ft_end(grid))
			AI->choices[i] = 50;
		else
			AI->choices[i] = previsions(var, var->grid, AI, i, var->turn);
		if (AI->choices[i] > best)
		{
			best = AI->choices[i];
			besti = i;
		}
		ft_griddel(&AI->grid);
		AI->grid = ft_gridcpy(AI->savegrid);
	}
	//var->turn = 'X';
	fill_i(&var->grid, besti, 'X');
}

void	morpion(t_morpion *var, t_AI *AI)
{
	int firsttry;

	ft_print_grid(var);
	while (!(var->game = ft_end(var)))
	{
		var->turn = (var->turn == 'O' ? 'X' : 'O');
		if (var->turn == 'O')
		{
			firsttry = 1;
			while (firsttry == 1 ||
			var->grid[(var->choice - 1) / 3]
				[(var->choice - 1) % 3] != ' ')
			{
				firsttry = 1;
				while (var->choice < 1 || var->choice > 9 || firsttry == 1)
				{
					ft_putstr("Make your choose : ");
					firsttry = 0;
					get_next_line(1, &var->line);				//()
					var->choice = ft_atoi(var->line);
				}
			}
			var->grid[(var->choice - 1) / 3][(var->choice - 1) % 3] = 'O';	
		}
		else
		{
			AI->nchoice = ft_n_empty(var->grid);
			AI->choice = (int *)malloc(sizeof(int) * AI->pos);
			ft_AI_turn(var, AI);
			ft_putendl("AI has played :");
			ft_print_grid(var);
		}
	}
	if (var->turn == 'O')
		ft_print_grid(var);
	ft_putstr("Player win : ");
	ft_putchar(var->turn);
}

int	main(int ac, char **av)
{
	t_morpion	var;
	t_AI		AI;
	float		rand;

	if (ac == 2 && (var.AI = ft_atoi(av[1])) != 0 &&
	var.AI >= 1 && var.AI <= 5)
	{
		init(&var);
		var.turn = (random() % 2 ? 'X' : 'O');
		morpion(&var, &AI);
		printf("%i\n", var.game);
	}
	else
		ft_putstr("usage: ./morpion_AI strenght_of_AI[1;5]");
	ft_putchar('\n');
	return (0);
}
/*
			while (!ft_strequ(get_next_line(1, &var->line), "O")
			&& !ft_strequ(get_next_line(1, &var->line), "X"))
			{
				ft_putendl("");
			}*/
