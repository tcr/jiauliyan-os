#ifndef __STDIO_H
#define __STDIO_H

#include <stream.h>
#include <stdarg.h>

#define L_tmpnam 16

/* FILE */

typedef struct {
	stream_s *stream;		// stream implementation
	long pos;
	
	unsigned int flags; 	// file status flags
	unsigned int istemp;	// temporary file indicator
	
	unsigned char *buffer;	// data transfer buffer (min size 256)
	int bsize;				// buffer size
	int hold;				// ungetc char if no buffer
	
	unsigned char open;
} FILE;

/*
 * file functions
 */

extern void clearerr(FILE *file);
extern int fclose(FILE *file);
extern int feof(FILE *file);
extern int ferror(FILE *file);
extern int fflush(FILE *file);
extern int fgetpos(FILE *file, int *pos);
extern FILE *fopen(const char *filename, const char *mode);
extern size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
extern FILE *freopen(const char *filename, const char *mode, FILE *stream);
extern int fseek(FILE *file, long int offset, int whence);
extern long int ftell(FILE *file);
extern size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *file);
extern int remove(const char *filename);
extern int rename(const char *old_filename, const char *new_filename);
extern void rewind(FILE *file);
extern void setbuf(FILE *stream, char *buffer);
extern int setvbuf(FILE *stream, char *buffer, int mode, size_t size);
extern FILE *tmpfile(void);
extern char *tmpnam(char *str);

/*
 * formatted I/O
 */
 
extern int fprintf(FILE *stream, const char *format, ...);
extern int printf(const char *format, ...);
extern int sprintf(char *str, const char *format, ...);
extern int vfprintf(FILE *stream, const char *format, va_list arg);
extern int vprintf(const char *format, va_list arg);
extern int vsprintf(char *str, const char *format, va_list arg);

/*
 * parse I/O
 */
 
extern int fscanf(FILE *stream, const char *format, ...);
extern int scanf(const char *format, ...);
extern int sscanf(const char *str, const char *format, ...);

/*
 * character I/O
 */
 
extern int fgetc(FILE *file);
#define getc(S) fgetc(S)
#define getchar() fgetc(stdin)
extern char *fgets(char *str, int n, FILE *file);
#define gets(C) fgets(C, stdin)

extern int fputc(int character, FILE *file);
#define putc(C, S) fputc(C, S)
#define putchar(C) fputc(C, stdout)
extern int fputs(const char *str, FILE *file);
#define puts(C) fputs(C, stdout)

extern int ungetc(int c, FILE *file);

/*
 * standard streams
 */

extern FILE *stdout;
extern FILE *stdin;
extern FILE *stderr;

extern void stdio_init();

#endif
