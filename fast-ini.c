#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATE_KEY 0
#define STATE_VALUE 1

typedef struct KeyVal {
	char *key;
	char *value;
} KeyVal;

typedef struct IniNode {
	KeyVal *current;
	struct IniNode *head;
} IniNode;



IniNode *new_ini_node()
{
	IniNode *node = malloc(sizeof(IniNode));

	node->head = NULL;;
	node->current = NULL;

	return node;
}

void ini_add(IniNode *node,KeyVal *value)
{
	IniNode *tmp = new_ini_node();

	if(node->current == NULL){
		node->current = value;
	}else{
		IniNode *it;
		it = node;

		while(it->head != NULL){
			it = it->head;
		}

		tmp->head = NULL;
		tmp->current = value;

		it->head = tmp;
	}
}

IniNode *parse_config(char* path)
{
	IniNode *node = new_ini_node();

	FILE *fp = fopen(path,"rb");
	int pp = 0; // Parser pointer
	size_t size;
	char *buffer;

	// Storing temporary values
	char w_buff[512];
	int w_size = 0;
	int state = STATE_KEY;

	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	rewind(fp);

	buffer = malloc(sizeof(char) * size);
	fread(buffer,size,1,fp);

	fclose(fp);

	KeyVal *temp = malloc(sizeof(KeyVal));

	while(pp < size){
		char tok = buffer[pp];

		if(tok == '=' || tok == '\n'){
			if(state == STATE_KEY){
				temp->key = strndup(&w_buff,w_size);
				w_size = 0;
				state = STATE_VALUE;
			}else{
				temp->value = strndup(&w_buff,w_size);
				w_size = 0;
				state = STATE_KEY;

				ini_add(node,temp);

				temp = malloc(sizeof(KeyVal));
			}
		}else{
				w_buff[w_size] = tok;
				w_size++;
		}

	pp++;

	}

	free(temp);
	free(buffer);

	return node;
}


int main()
{
	// Will return a linked list of key-values nodes
	IniNode *node = parse_config("./test.ini");
	
	IniNode *it = node;

	while(it->head != NULL){
		printf("%s : %s \n",it->current->key,it->current->value);
		it = it->head;
	}

	printf("Last -> %s : %s\n",it->current->key,it->current->value);

	return 0;
}
