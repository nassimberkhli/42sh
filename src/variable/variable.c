#include "hash_map.h"
#include "variable.h"
#include "my_itoa.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

extern struct hash_map *hash_map;

static void calcul_variable_ext(char * value, char * value_2, char * value_3, char c)
{
	if (c == '+')
		value = my_itoa(atoi(value_2) + atoi(value_3), value);
	if (c == '-')
		value = my_itoa(atoi(value_2) - atoi(value_3), value);
	if (c == '*')
		value = my_itoa(atoi(value_2) * atoi(value_3), value);
	if (c == '/')
		value = my_itoa(atoi(value_2) / atoi(value_3), value);
	if (c == '%')
		value = my_itoa(atoi(value_2) % atoi(value_3), value);
}

void special_variable()
{
	bool a = false;
	hash_map_insert(hash_map, "?", "0", &a);
}

void variable_calcul(char* value)
{
	size_t i = 0;
	int free_1 = 0;
	int free_2 = 0;
	while (value[i] != ' ')
		i++;
	char *value_2 = malloc(2);
	size_t a = 3;
	for (; value[a] != ' '; a++)
	{
		value_2[a - 3] = value[a];
		value_2 = realloc(value_2, a - 2);
	}
	value_2[a - 2] = '\0';
	if (value_2[0] == '$')
	{
		free_1 = 1;
		value_2 = get_variable(value_2);
	}
	char c = ' ';
	while (value[a] == ' ')
		a++;
	size_t b = a + 1;
	while (value[b] == ' ')
		b++;
	c = value[a];
	char *value_3 = malloc(2);
	size_t indice = 0;
	for (; value[b] != ')'; b++)
	{
		value_3[indice] = value[b];
		value_3 = realloc(value_3, indice + 1);
		indice++;
	}
	value_3[indice] = '\0';
	if (value_3[0] == '$')
	{
		free_2 = 1;
		value_3 = get_variable(value_3);
	}
	calcul_variable_ext(value, value_2, value_3, c);
	if (free_1 == 0)
		free(value_2);
	if (free_2 == 0)
		free(value_3);
}

void variable(char** data)
{
	char* key = malloc(1);
	size_t i = 0;
	for (; data[0][i] != '=' ; i++)
	{
		key[i] = data[0][i];
		key = realloc(key, i + 1);
	}
	i++;
	key[i - 1] = '\0';
	char* value = malloc(2);
	size_t o = 0;
	for (; i < strlen(data[0]) ; i++)
	{
		value[o] = data[0][i];
		value = realloc(value, o + 1);
		o++;
	}
	value[o] = '\0';
	if (value[0] == '$')
	{	
		if (value[1] == '(')
		{
			int i = 1;
			while (data[i] != NULL)
			{
				value = strcat(value, " ");
				value = strcat(value, data[i]);
				i++;
			}
			variable_calcul(value);
		}
		else
			value = get_variable(value);
	}
	bool a = false;
	hash_map_insert(hash_map, key, value, &a);
}

char *get_variable(char* data)
{
	if (strcmp(data, "RANDOM") == 0)
		return my_itoa(rand(), data);
	char * data2 = malloc(strlen(data));
	data2 = strcpy(data2, data + 1);
	data = hash_map_get(hash_map, data2);
	free(data2);
	if (!data)
        return "1";
	return data;
}
