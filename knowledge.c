#include "knowledge.h"

Knowledge *Knowledge_New(char *inp,char *res) {
	Knowledge *knowledge=malloc(sizeof(*knowledge));
	knowledge->inp=strdup(inp);
	knowledge->res=strdup(res);
	return knowledge;
}

void Knowledge_Free(Knowledge ***knowledges,size_t *nknowledges) {
	for(size_t i=0;i<(*nknowledges);i++) {
		free((*knowledges)[i]->inp);
		(*knowledges)[i]->inp=NULL;
		free((*knowledges)[i]->res);
		(*knowledges)[i]->res=NULL;
		free((*knowledges)[i]);
		(*knowledges)[i]=NULL;
	}
	free(*knowledges);
	(*knowledges)=NULL;
	(*nknowledges)=0;
}

void Knowledge_Add(Knowledge ***knowledges,size_t *nknowledges,Knowledge *knowledge) {
	(*knowledges)=realloc(*knowledges,sizeof(**knowledges)*(*nknowledges+1));
	(*knowledges)[(*nknowledges)++]=knowledge;
}

void Knowledge_Print(Knowledge **knowledges,size_t nknowledges) {
	for(size_t i=0;i<nknowledges;i++) {
		printf("%s|%s\n",knowledges[i]->inp,knowledges[i]->res);
	}
}

void Knowledge_Load(Knowledge ***knowledges,size_t *nknowledges) {
	char line[512];
	FILE *fin=fopen(KNOWLEDGE_FILE,"r");
	while(fgets(line,512,fin)) {
		char *p=strchr(line,'\n');
		if(p) *p='\0';
		char *sep=strchr(line,'|');
		if(sep) {
			*sep='\0';
			char *inp=line;
			char *res=sep+1;
			trim(inp);
			trim(res);
			if(*inp && *res) {
				Knowledge_Add(knowledges,nknowledges,Knowledge_New(inp,res));
			}
		}
	}
	fclose(fin);
}

void Knowledge_Save(Knowledge **knowledges,size_t nknowledges) {
	FILE *fout=fopen(KNOWLEDGE_FILE,"w");
	for(size_t i=0;i<nknowledges;i++) {
		fprintf(fout,"%s|%s\n",knowledges[i]->inp,knowledges[i]->res);
	}
	fclose(fout);
}

char *Knowledge_Reply(Knowledge **knowledges,size_t nknowledges,char *inp) {
	static ssize_t i=0,j=0;
	char *res=NULL;
	if(nknowledges==0) return NULL;
	while(1) {
		i++;
		if(i>=nknowledges) i=0;
		if(!strcmp(inp,knowledges[i]->inp)) {
			j=i;
			res=knowledges[i]->res;
			break;
		}
		if(i==j) break;
	}
	return res;
}
