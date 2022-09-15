#include "../inc/libft.h"
#include <stdarg.h>


#define BUFFER_SIZE 1024


#define FLAGS_LEFT      (1U <<  1U)
#define FLAGS_PRECISION (1U <<  2U)
#define FLAGS_LONG      (1U <<  3U)


char buffer[BUFFER_SIZE];


void copy_arg_to_buffer(char *buffer, char *arg, size_t *idx)
{
  size_t j = 0;
  for (size_t i = 0; arg[i]; i++) {
    buffer[j++] = arg[i];
  }
  *idx += j;
} 


int my_printf(int fd, va_list ap, char *format)
{
	static int ret_value = 0;

  unsigned int flags, width/*, precision*/, n;
  size_t idx = 0U;
  char *arg;

	ft_bzero(buffer, BUFFER_SIZE);
	while (*format && idx < BUFFER_SIZE) {

		if (*format == '%') { // %[flags][width][.precision][length]
			format++;

      flags = 0U;
      do {
        if (*format == '-'){
          flags |= FLAGS_LEFT;
          format++; 
          n = 1U;
        } else {
          n = 0U;
        }
      } while (n);

      width = 0U;
      if (ft_isdigit(*format)) {
        width = skip_atoi((const char**)&format);
      } else if (*format == '*') {
        const int w = va_arg(ap, int);
        if (w < 0) {
          flags |= FLAGS_LEFT;    // reverse padding
          width = (unsigned int)-w;
        } else {
          width = (unsigned int)w;
        }
        format++;
      }

      if (*format == 'l') {
        flags |= FLAGS_LONG;
        format++;
      }

      switch (*format) {
        case 'd' :
        case 'i' :
        case 'u' : {
          if (flags & FLAGS_LONG) {
            arg = ft_itoa_long(va_arg(ap, long));
          } else {
            arg = ft_itoa(va_arg(ap, int));
          }
          format++;
          break;
        }
        case 's' : {
          arg = ft_strdup(va_arg(ap, char*));
          format++;
          break;
        }
        case 'c' : {
          char buf[2];
          buf[0] = va_arg(ap, int);
          buf[1] = '\0';
          arg = ft_strdup(buf);
          format++;
          break;
        }
      }

      size_t size_arg = ft_strlen(arg);  
      if (width > size_arg && (width -= size_arg)) {
        if (flags & FLAGS_LEFT) {
          copy_arg_to_buffer(buffer + idx, arg, &idx);
          for ( ; width; width--) {
            buffer[idx++] = ' ';
          }
        } else {
          for ( ; width; width--) {
            buffer[idx++] = ' ';
          }
          copy_arg_to_buffer(buffer +idx, arg, &idx);
        } 
      } else {
        copy_arg_to_buffer(buffer +idx, arg, &idx);
      }
      
      free(arg);

		}
		else {
			buffer[idx++] = *format++;
    }
	}
	ft_putstr_fd(buffer, fd);
	ret_value += idx;
	return (*format) ? my_printf(fd, ap, format) : ret_value;
}

int my_fd_printf(int fd, const char *format, ...)
{
  va_list ap;
  va_start(ap, format);
  ft_bzero(buffer, BUFFER_SIZE);
  int ret = my_printf(fd, ap, (char*)format);
  va_end(ap);
  return ret;
}

// #include <stdio.h>

// int main(void)
// {
//   printf("blabla .%10s. salut .%*u.\n", "coucou", -1, 22);
//   my_fd_printf(1, "blabla .%10s. salut .%*u.\n", "coucou", -1, 22);
//   return 0;
// }