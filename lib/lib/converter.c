#include "../inc/libft.h"

int		ft_atoi(const char *str)
{
	long	nb;
	int		signe;

	nb = 0;
	signe = 1;
	while ((*str < 14 && *str >= 9) || *str == 32)
		str++;
	signe = (*str == '-') ? -1 : 1;
	str = (*str == '-' || *str == '+') ? str + 1 : str;
	while (ft_isdigit(*str))
	{
		nb = nb * 10 + *str - '0';
		str++;
	}
	return (nb * signe);
}

int ft_toupper(int c)
{
	return (ft_isalpha(c) && (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c);
}

int ft_tolower(int c)
{
	return (ft_isalpha(c) && (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c);
}

static int		ft_nblen(unsigned int nb)
{
	int len = 0;

	if (nb < 10)
		return 1;
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return len;
}

char			*ft_itoa(int n)
{
	int				len;
	char			*str;
	unsigned int	nb;

	len = (n < 0) ? ft_nblen(-n) + 1 : ft_nblen(n);
	if (!(str = (char*)malloc(sizeof(char) * len + 1)))
		return (NULL);
	ft_bzero(str, len + 1);
	nb = n;
	if (n < 0)
	{
		*str = '-';
		nb = -n;
	}
	*(str + len--) = '\0';
	while (len > (*str == '-' ? 0 : -1))
	{
		if (nb > 9)
		{
			str[len--] = (nb % 10) + '0';
			nb /= 10;
		}
		else
			str[len--] = nb + '0';
	}
	return (str);
}

char			*ft_itoa_long(long n)
{
	int				len;
	char			*str;
	unsigned long	nb;

	len = (n < 0) ? ft_nblen(-n) + 1 : ft_nblen(n);
	if (!(str = (char*)malloc(sizeof(char) * len + 1)))
		return (NULL);
	ft_bzero(str, len + 1);
	nb = n;
	if (n < 0)
	{
		*str = '-';
		nb = -n;
	}
	*(str + len--) = '\0';
	while (len > (*str == '-' ? 0 : -1))
	{
		if (nb > 9)
		{
			str[len--] = (nb % 10) + '0';
			nb /= 10;
		}
		else
			str[len--] = nb + '0';
	}
	return (str);
}

static	unsigned int	length_n_unsigned(unsigned int n, char *base)
{
	unsigned int		length;

	length = (n == 0) ? 1 : 0;
	while (n)
	{
		n = n / ft_strlen(base);
		length++;
	}
	return (length);
}

char					*ft_itoahexa(long n)
{
	char				*p;
	unsigned int		length;
	char				*base;

	base = "0123456789abcdef";
	length = length_n_unsigned(n, base) + 2;
	if (!(p = malloc(sizeof(char) * (length + 1))))
		return (0);
	p[length] = '\0';
	while (length > 1)
	{
		p[--length] = base[n % 16];
		n = n / 16;
	}
	p[1] = 'x';
	p[0] = '0';
	return (p);
}


char					*ft_unitoa_base(unsigned int n, char *base)
{
	char				*p;
	int					size;
	unsigned int		length;

	length = length_n_unsigned(n, base);
	size = ft_strlen(base);
	if (!(p = malloc(sizeof(char) * (length + 1))))
		return (0);
	p[length] = '\0';
	while (length)
	{
		p[--length] = base[n % size];
		n = n / size;
	}
	return (p);
}

int		skip_atoi(const char **s)
{
	int	nb;

	nb = 0;
	while (**s >= '0' && **s <= '9')
		nb = nb * 10 + *((*s)++) - '0';
	return (nb);
}
