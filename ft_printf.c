/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdussert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 13:21:32 by jdussert          #+#    #+#             */
/*   Updated: 2020/01/29 15:29:45 by jdussert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct		s_printf
{
	char	type;
	int		width;
	int		precision;
}					t_printf;

int		ft_free(t_printf *args, va_list parameters, int res)
{
	free(args);
	args = NULL;
	va_end(parameters);
	return (res);
}

void	ft_putchar(char c, int *res)
{
	write(1, &c, 1);
	res++;
}

void	ft_putstr(char *str, int *res)
{
	int i;

	i = 0;
	while(str[i] != '\0')
		ft_putchar(str[i++], res);
}

int		ft_l_atoi(char *str, int *i)
{
	int	res;

	res = 0;
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		res = (res * 10) + (str[*i] - '0');
		(*i)++;
	}
	return (res);
}

int		ft_str_len(int n, int base)
{
	int i;

	i = 0;
	if (n < 0)
		i++;
	while (n != 0 && i++ < base)
		n /= base;
	return (i);
}

char	*ft_itoa_base(int n, int base, char *tab)
{
	int		tmp;
	char	*str;
	int		len;

	tmp = (int)n;
	len = ft_str_len(n, base);
	if (n < 0)
		tmp *= -1;
	if (n == 0)
	{
		if (!(str = malloc(sizeof(char) * 2)))
			return (NULL);
		str[0] = 48;
		str[1] = '\0';
	}
	else
	{
		if (!(str = malloc(sizeof(char) * len)))
			return (NULL);
		if (n < 0)
			str[0] = '-';
		str[len] = '\0';
		while (tmp != 0 && base > 0)
		{
			str[--len] = tab[tmp % base];
			tmp /= base;
		}
	}
	return (str);
}

void	ft_my_args(char *format, int *i, t_printf *args)
{
	while (format[*i] >= '0' && format[*i] <= '9')
		args->width = ft_l_atoi(format, i);
	if (format[*i] == '.')
	{
		++(*i);
		while (format[*i] >= '0' && format[*i] <= '9')
			args->precision = ft_l_atoi(format, i);
	}
}

int		ft_my_type(char	*format, t_printf *args, int *i)
{
	if (format[*i] == 's' || format[*i] == 'd' || format[*i] == 'x')
	{
		args->type = format[*i];
		(*i)++;
		return (1);
	}
	return (0);
}

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

void	ft_s_type(va_list parameters, t_printf *args, int *res)
{
	char	*str;
	int		len;
	int		i;

	i = 0;
	str = (char *)va_arg(parameters, char *);
	if (!str)
		str = "(null)";
	len = ft_strlen(str);
	if (args->width < len && (args->precision == -1 || args->precision > len))
		ft_putstr(str, res);
	else if (args->width > len && args->precision == -1)
	{
		while (i++ < args->width - len)
			ft_putchar(' ', res);
		ft_putstr(str, res);
	}
	else if (args->width > len && args->precision < len)
	{
		while (args->width-- > args->precision)
			ft_putchar(' ', res);
		while (args->precision > i)
			ft_putchar(str[i++], res);
	}
	else if (args->precision < len)
	{
		while (args->precision > i)
			ft_putchar(str[i++], res);
	}
}

void	ft_d_type(va_list parameters, t_printf *args, int *res)
{
	long int	n;
	int			len;
	char		*str;
	int			i;

	i = 0;
	n = (int)va_arg(parameters, int);
	str = ft_itoa_base(n, 10, "0123456789");
	len = ft_strlen(str);
	if (n == 0 && (args->precision == 0 || args->precision == -1))
	{
		while (i++ < args->width)
			ft_putchar(' ', res);
		str = "\0";
	}
	if (args->precision <= len && args->width <= len)
		ft_putstr(str, res);
	else if (args->width > len && args->precision <= len)
	{
		while (i++ < args->width - len)
			ft_putchar(' ', res);
		ft_putstr(str, res);
	}
	else if (args->width > len && args->precision > len)
	{
		while (args->width-- > args->precision)
			ft_putchar(' ', res);
		while (args->precision-- > len)
			ft_putchar('0', res);
		ft_putstr(str, res);
	}
	else if (args->precision > len)
	{
		while (args->precision-- > len)
			ft_putchar('0', res);
		ft_putstr(str, res);
	}
}

void	ft_x_type(va_list parameters, t_printf *args, int *res)
{
	unsigned int	n;
	int			len;
	char		*str;
	int			i;

	i = 0;
	n = (unsigned int)va_arg(parameters, unsigned int);
	str = ft_itoa_base(n, 16, "0123456789abcdef");
	len = ft_strlen(str);
	if (n == 0 && (args->precision == 0 || args->precision == -1))
	{
		while (i++ < args->width)
			ft_putchar(' ', res);
		str = "\0";
	}
	if (args->precision <= len && args->width <= len)
		ft_putstr(str, res);
	else if (args->width > len && args->precision <= len)
	{
		while (i++ < args->width - len)
			ft_putchar(' ', res);
		ft_putstr(str, res);
	}
	else if (args->width > len && args->precision > len)
	{
		while (args->width-- > args->precision)
			ft_putchar(' ', res);
		while (args->precision-- > len)
			ft_putchar('0', res);
		ft_putstr(str, res);
	}
	else if (args->precision > len)
	{
		while (args->precision-- > len)
			ft_putchar('0', res);
		ft_putstr(str, res);
	}

}

int 	ft_printf(const char *format, ... )
{
	int			res;
	int			i;
	va_list		parameters;
	t_printf	*args;

	res = 0;
	i = 0;
	va_start(parameters, format);
	if (!(args = (t_printf *)malloc(sizeof(t_printf))))
		return(-1);
	args->width = -1;
	args->precision = -1;
	args->type = 'n';
	while(format[i] != '\0')
	{
		while (format[i] != '\0' && format[i] != '%')
			ft_putchar((char)format[i++], &res);
		if (format[i] == '%')
		{
			i++;
			ft_my_args((char *)format, &i, args);
			if (ft_my_type((char *)format, args, &i) == 1)
			{
				if (args->type == 's')
					ft_s_type(parameters, args, &res);
				else if (args->type == 'd')
					ft_d_type(parameters, args, &res);
				else if (args->type == 'x')
					ft_x_type(parameters, args, &res);
			}
			else
				break ;
		}
	}
	return(ft_free(args, parameters, res));
}

int	main(void)
{
	printf("test :\n%10.0d\n", 0);
	ft_printf("mon test :\n%10.0d\n", 0);
	ft_printf("%10.0s\n", "HEY BEBE");
	ft_printf("Magic %s is %5d\n", "number", 42);
	ft_printf("Hexadecimal for %.10d is %.10x\n", 0, 0);
}
