#include "hash_map.h"
#include "variable.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

extern struct hash_map *hash_map;

void variable(char* data)
{
	char* key = malloc(1);
	size_t i = 0;
	for (; data[i] != '=' ; i++)
	{
		key[i] = data[i];
		key = realloc(key, i + 1);
	}
	i++;
	key[i - 1] = '\0';
	char* value = malloc(2);
	size_t o = 0;
	for (; i < strlen(data) ; i++)
	{
		value[o] = data[i];
		value = realloc(value, o + 1);
		o++;
	}
	value[o] = '\0';
	bool a = false;
	hash_map_insert(hash_map, key, value, &a);
}

char *get_variable(char* data)
{
	char * data2 = malloc(strlen(data));
	data2 = strcpy(data2, data + 1);
	data = hash_map_get(hash_map, data2);
	free(data2);
	return data;
}
