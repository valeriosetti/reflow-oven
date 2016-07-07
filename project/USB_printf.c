
#include "USB_printf.h"
#include "pcd8544.h"

// Macros
#define USB_put_string(_text_, _size_) 		CDC_Transmit_FS(_text_, _size_)
#define USB_put_char(c) 					CDC_Transmit_FS((uint8_t*)c, 1)
#define PCD8544_put_string(_text_)			PCD8544_Puts(_text_, PCD8544_Pixel_Set, PCD8544_FontSize_5x7)

// Local variables
#define OUTPUT_BUFFER_SIZE		1024		// Size of the output buffer
char output_buffer[OUTPUT_BUFFER_SIZE];		// Local output buffer for buffered writesreflow_list[i].seconds,


static void printchar(char **str, char c)
{
	if (str) {
		**str = c;
		++(*str);
	} else {
		while(USB_put_char(&c) != USBD_OK);
	}
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			printchar (out, padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		printchar (out, *string);
		++pc;
	}
	for ( ; width > 0; --width) {
		printchar (out, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints (out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
//			printchar (out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints (out, s, width, pad);
}

static int print(char **out, const char *format, va_list args )
{
	register int width, pad;
	register int pc = 0;
	char scr[2];

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				register char *s = (char *)va_arg( args, int );
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}
			if( *format == 'd' ) {
				pc += printi (out, va_arg( args, int ), 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'x' ) {
				pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'X' ) {
				pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'A');
				continue;
			}
			if( *format == 'u' ) {
				pc += printi (out, va_arg( args, int ), 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = (char)va_arg( args, int );
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		}
		else {
		out:
			printchar (out, *format);
			++pc;
		}
	}
	if (out) **out = '\0';
	va_end( args );
	return pc;
}

int USB_printf(const char *format, ...)
{
	va_list args;
	va_start( args, format );
	return print( 0, format, args );
}

int USB_printf_buff(const char *format, ...)
{
	int written_chars;
	char* p_output_buffer = &output_buffer[0];

	va_list args;
	va_start( args, format );
	// First print the string to the local output buffer
	written_chars = print(&p_output_buffer, format, args );
	// Then send it to the output
	while(USB_put_string(output_buffer, written_chars) == USBD_BUSY);

	return written_chars;
}

int PCD8544_printf_buff(const char *format, ...)
{
	int written_chars;
	char* p_output_buffer = &output_buffer[0];

	va_list args;
	va_start( args, format );
	// First print the string to the local output buffer
	written_chars = print(&p_output_buffer, format, args );
	// Then send it to the output
	PCD8544_put_string(output_buffer);

	return written_chars;
}
