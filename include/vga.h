extern void cls();
extern void putscrnc(char c);
extern void putscrns(char *str);
extern void putscrni(unsigned int i);
extern void putscrnp(void *p);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GREY 7
#define DARK_GREY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define LIGHT_BROWN 14
#define WHITE 15

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
