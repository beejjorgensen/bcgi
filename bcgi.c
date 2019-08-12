/*
** Beej's CGI C programming library (bcgi)
**
** Version 0.9
**
** This source is Copyright 1994 Brian Hall and Associates.  All
** rights reserved.  This source may be freely distributed as long as
** it remains unchanged, and as long as the user understands that
** there is no warranty, implied or otherwise, concerning its
** reliability or safety, and that Brian Hall ("Beej") is not
** responsible for any damage that this library may cause your data,
** computer equipment, person, pets, food, car, house, lawn, or any
** other items that you or any other person might claim as their own.
*/

/*
todo:

make a read_next_var() function for reading from stdin
make a copy_next_var() to copy stdin to another file

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bcgi.h"

void get_pdata(char *buf, int maxsize)
{
    int len;

    len = get_content_length();
    if (maxsize > 0)
        if (len > maxsize-1) len = maxsize-1;
    fread(buf,sizeof(char),len,stdin);
    buf[len] = '\0';
}

char *getvar(char *buf, char *var)
{
    static char val[MAXVARLEN];
    static char tmpvar[MAXVARLEN];
    static char *leftoff=NULL;
    char *p, *q;
    int tvl;
 
    if (leftoff==NULL)
        leftoff = buf;

    if (var != NULL)
        sprintf(tmpvar,"%s=",var);
    else 
        buf=leftoff;

    if (buf == NULL)  /* called with NULL var too soon */
        return NULL;

    tvl = strlen(tmpvar);
    if ((p = strstr(buf, tmpvar)) == NULL)
        return NULL;
    if ((q = strchr(p, '&')) == NULL) {
        strcpy(val,p+tvl);
        leftoff = p+tvl+strlen(p+tvl);
    } else {
        strncpy(val,p+tvl,q-p-tvl);
        *(val+(q-p-tvl)) = '\0';
        leftoff = q+1;
    }
    un_url(val);

    return val;
} 
    
void un_url(char *s)
{
    char tmp[3], *p;

    for(p=s;*p != '\0';p++) {  /* translate special chars */
        switch(*p) {
            case '+' : *p =' ';  /* handle spaces */
                       break;
            case '%' : memcpy(tmp,p+1,2);  /* handle hex numbers */
                       tmp[2] = '\0';
                       *p = (char)strtoul(tmp,NULL,16);
                       memmove(p+1,p+3,strlen(p+3)+1);
                       break;
        }
    }
}

char *get_request_method(void)
{
    return getenv("REQUEST_METHOD");
}

int get_content_length(void)
{
    char *p;

    if ((p=getenv("CONTENT_LENGTH")) == NULL)
        return 0;  /* error, or no data */
    else
        return atoi(p);
}


char *get_content_type(void)
{
    return getenv("CONTENT_TYPE");
}

char *get_query_string(void)
{
    return getenv("QUERY_STRING");
}

char *get_remote_addr(void)
{
    return getenv("REMOTE_ADDR");
}

char *get_remote_host(void)
{
    return getenv("REMOTE_HOST");
}

char *get_remote_user(void)
{
    return getenv("REMOTE_USER");
}

char *get_script_name(void)
{
    return getenv("SCRIPT_NAME");
}

char *get_server_name(void)
{
    return getenv("SERVER_NAME");
}

unsigned short get_server_port(void)
{
    char *p;

    if ((p=getenv("SERVER_PORT")) == NULL)
        return 0;  /* error */
    else
        return atoi(p);
}

char *get_server_protocol(void)
{
    return getenv("SERVER_PROTOCOL");
}

char *get_server_software(void)
{
    return getenv("SERVER_SOFTWARE");
}

char *get_http_user_agent(void)
{
    return getenv("HTTP_USER_AGENT");
}

char *get_http_accept(void)
{
    return getenv("HTTP_ACCEPT");
}

char *get_gateway_interface(void)
{
    return getenv("GATEWAY_INTERFACE");
}

char *escape(char *s)
{
    char escapechars[] = "$`~^()[]{}*?&;<>|\\\"' ";
    char *p, *q;
    int found;

    for(p=s;*p != '\0';p++) {
        for(q=escapechars,found=0;*q != '\0' && !found;q++)
            if (*p == *q) found = 1;
        if (found) {
            memmove(p+1,p,strlen(p)+1);
            *p = '\\';
            p++;
        }
    }
}

int xyinrect(int x, int y, int x1, int y1, int x2, int y2)
{
    return (x<=x2&&x>=x1&&y<=y2&&y>=y1);
}

int xyincircle(int x, int y, int cx, int cy, int r)
{
    return (distance(x,y,cx,cy)<r);
}

int distance(int x1, int y1, int x2, int y2)
{
    return (sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
}

int nearest(int x, int y, int point[], int numpoints)
{
    int i,nearest=0,index=0,tmp;

    if (index < 0) return -1;

    for(i=0;i<numpoints;i++) {
        tmp = distance(x,y,point[i*2],point[i*2+1]);
        if (i==0)
            nearest = tmp;
        else if (tmp < nearest) {
            index = i;
            nearest = tmp;
        }
    }

    return index;
}
