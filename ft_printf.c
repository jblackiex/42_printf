/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbordoni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 13:36:33 by nbordoni          #+#    #+#             */
/*   Updated: 2023/05/06 13:36:35 by nbordoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_s(char *str, int *count, char c)
{
	if (c != -1)
	{
		*count += write(1, &c, 1);
		return ;
	}
	if (!str)
		str = "(null)";
	while (*str)
		*count += write(1, str++, 1);
}

void	print_p(unsigned long int p, int base, int *count)
{
	char	*bas;

	bas = "0123456789abcdef";
	if (p && base == -16)
		*count += write(1, "0x", 2);
	if (!p)
	{
		*count += write(1, "(nil)", 5);
		return ;
	}
	if (p >= 16)
		print_p((p / 16), 16, count);
	*count += write(1, &bas[p % 16], 1);
}

void	print_xd(long long int num, int base, int flag, int *count)
{
	char	*bas;

	if (flag)
		bas = "0123456789ABCDEF";
	else
		bas = "0123456789abcdef";
	if (num < 0)
	{
		num *= -1;
		if (base != -1)
			*count += write(1, "-", 1);
	}
	if (base == -1)
		base = 10;
	if (num >= base)
		print_xd((num / base), base, flag, count);
	*count += write(1, &bas[num % base], 1);
}

void	core(const char *str, int *count, va_list args)
{
	if (*str == 's')
		print_s(va_arg(args, char *), count, -1);
	else if (*str == 'c')
		print_s(NULL, count, va_arg(args, int));
	else if (*str == 'd' || *str == 'i')
		print_xd((long long int)va_arg(args, int), 10, 0, count);
	else if (*str == 'u')
		print_xd(va_arg(args, unsigned int), -1, 0, count);
	else if (*str == 'p')
		print_p(va_arg(args, unsigned long int), -16, count);
	else if (*str == 'x')
		print_xd(va_arg(args, unsigned int), 16, 0, count);
	else if (*str == 'X')
		print_xd(va_arg(args, unsigned int), 16, 1, count);
}

int	ft_printf(const char *str, ...)
{
	int		count;
	va_list	args;

	count = 0;
	va_start(args, str);
	while (str && *str)
	{
		if (*str == '%' && (*(str + 1) == 's' || *(str + 1) == 'd'
				|| *(str + 1) == 'x' || *(str + 1) == 'X' || *(str + 1) == 'u'
				|| *(str + 1) == 'i' || *(str + 1) == 'p'
				|| *(str + 1) == 'c' || *(str + 1) == '%'))
		{
			str++;
			if (*str == '%')
				count += write(1, "%", 1);
			else
				core(str, &count, args);
		}
		else
			count += write(1, str, 1);
		str++;
	}
	return (va_end(args), count);
}
