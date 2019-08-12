/*
** bcgi.h - header file for bcgi.c
**
** Version 0.9
*/

#define MAXVARLEN 500

void get_pdata(char *buf, int maxsize);
char *getvar(char *buf, char *var);
char *get_request_method(void);
int get_content_length(void);
char *get_content_type(void);
char *get_query_string(void);
char *get_remote_addr(void);
char *get_remote_host(void);
char *get_remote_user(void);
char *get_script_name(void);
char *get_server_name(void);
unsigned short get_server_port(void);
char *get_server_protocol(void);
char *get_server_software(void);
char *get_http_user_agent(void);
char *get_http_accept(void);
char *get_gateway_interface(void);
void un_url(char *s);
char *escape(char *s);
int xyinrect(int x, int y, int x1, int y1, int x2, int y2);
int xyincircle(int x, int y, int cx, int cy, int r);
int distance(int x1, int y1, int x2, int y2);
int nearest(int x, int y, int point[], int numpoints);

