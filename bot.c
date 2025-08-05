#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "dyad.h"
#include "util.h"
#include "knowledge.h"

static char *hst  = "irc.undernet.org";
static char *chn = "#ama";
static char *nck = "frio";
static int prt = 6667;

Knowledge **knowledges=NULL;
size_t nknowledges=0;

static void raw(dyad_Stream *stream,const char *fmt,...) {
	va_list args;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);
	va_start(args,fmt);
	dyad_vwritef(stream,fmt,args);
	va_end(args);
}

static void onConnect(dyad_Event *e) {
  /* Introduce ourselves to the server */
  dyad_writef(e->stream,"NICK %s\r\n",nck);
  dyad_writef(e->stream,"USER %s 0 * :%s\r\n",nck,nck);
}

static void onError(dyad_Event *e) {
  printf("error: %s\n", e->msg);
}

static void onLine(dyad_Event *e) {
	char *cmd, *usr, *par, *txt;

	printf("%s\n", e->data);

	cmd=e->data;

	usr = hst;
	if(!cmd || !*cmd)
		return;
	if(cmd[0] == ':') {
		usr = cmd + 1;
		cmd = skip(usr, ' ');
		if(cmd[0] == '\0')
			return;
		skip(usr, '!');
	}
	skip(cmd, '\r');
	par = skip(cmd, ' ');
	txt = skip(par, ':');
	trim(par);
	trim(txt);

	/* Handle PING */
	if (!strcmp(cmd,"PING")) {
    	raw(e->stream,"PONG :%s\r\n",txt);
  	}

  	/* Handle RPL_WELCOME */
	if(!strcmp(cmd,"001")) {
	    /* Join channel */
    	raw(e->stream,"JOIN %s\r\n",chn);
  	}

	if(!strcmp(cmd,"PRIVMSG")) {
		if(!strncmp(txt,".teach ",7)) {
			char *sep=strchr(txt+7,'|');
			if(sep) {
				*sep='\0';
				char *inp=txt+7;
				char *res=sep+1;
				if(*inp && *res) {
					Knowledge_Add(&knowledges,&nknowledges,Knowledge_New(inp,res));
				}
			}
		} else if(!strcmp(txt,".load")) {
			Knowledge_Free(&knowledges,&nknowledges);
			Knowledge_Load(&knowledges,&nknowledges);
		} else if(!strcmp(txt,".save")) {
			Knowledge_Save(knowledges,nknowledges);
		} else if(txt && *txt) {
			char *res=Knowledge_Reply(knowledges,nknowledges,txt);
			if(res) {
				raw(e->stream,"PRIVMSG %s :%s: %s\r\n",chn,usr,res);
			}
		}
	}
}

int main(void) {
	dyad_Stream *s;

	srand(time(NULL));

	Knowledge_Load(&knowledges,&nknowledges);

	dyad_init();

	s = dyad_newStream();

	dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
	dyad_addListener(s, DYAD_EVENT_ERROR,   onError,   NULL);
	dyad_addListener(s, DYAD_EVENT_LINE,    onLine,    NULL);
	dyad_connect(s, hst, prt);

	while (dyad_getStreamCount() > 0) {
		dyad_update();
	}

	dyad_shutdown();
	return 0;
}
