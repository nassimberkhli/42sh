#include "hash_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hash_map *hash_map_init(size_t size)
{
    struct hash_map *hash = malloc(sizeof(struct hash_map));
    hash->size = size;
    hash->data = calloc(size, sizeof(struct pair_list *));
    return hash;
}


bool hash_map_insert(struct hash_map *hash_map, const char *key, char *value,
                     bool *updated)
{
	bool a;
    if (updated == NULL)
    {
        a = false;
        updated = &a;
    }
    *updated = false;
    if (hash_map == NULL || hash_map->size == 0)
    {
        return false;
    }
    struct pair_list *paire = malloc(sizeof(struct pair_list));
    paire->key = key;
    paire->value = value;
    paire->next = NULL;
    size_t hashe = hash(key);
    if (hashe > hash_map->size)
    {
        hashe = hashe % hash_map->size;
    }
    if (hash_map->data[hashe] == NULL)
    {
        hash_map->data[hashe] = paire;
    }
    else
    {
        struct pair_list *paire_2 = hash_map->data[hashe];
        while (paire_2 != NULL)
        {
            if (strcmp(paire_2->key, key) == 0)
            {
                paire_2->value = value;
                free(paire);
                *updated = true;
                return *updated;
            }
            paire_2 = paire_2->next;
        }
        paire->next = hash_map->data[hashe];
        hash_map->data[hashe] = paire;
    }
    return true;
}


void hash_map_free_ann(struct pair_list *paire)
{
    if (paire->next != NULL)
    {
        hash_map_free_ann(paire->next);
    }
    free(paire);
}

void hash_map_free(struct hash_map *hash_map)
{
    if (hash_map == NULL)
    {
        return;
    }
    for (size_t i = 0; i < hash_map->size; i++)
    {
        if (hash_map->data[i] != NULL)
        {
            hash_map_free_ann(hash_map->data[i]);
        }
    }
    free(hash_map->data);
    free(hash_map);
}

void hash_map_dump(struct hash_map *hash_map)
{
    if (hash_map == NULL || hash_map->size == 0)
    {
        return;
    }
    struct pair_list *paire;
    for (size_t i = 0; i < hash_map->size; i++)
    {
        if (hash_map->data[i])
        {
            paire = hash_map->data[i];
            printf("%s: %s", paire->key, paire->value);
            paire = paire->next;
            for (; paire != NULL; paire = paire->next)
            {
                printf(", %s: %s", paire->key, paire->value);
            }
            printf("\n");
        }
    }
}

char *hash_map_get(const struct hash_map *hash_map, const char *key)
{
    if (hash_map == NULL || hash_map->size == 0)
    {
        return NULL;
    }
    size_t hashe = hash(key);
    if (hashe >= hash_map->size)
    {
        hashe = hash(key) % hash_map->size;
    }
    if (hash_map->data[hashe] == NULL)
    {
        return NULL;
    }
    struct pair_list *paire = hash_map->data[hashe];
    for (; paire != NULL; paire = paire->next)
    {
        if (strcmp(paire->key, key) == 0)
        {
            return paire->value;
        }
    }
    return NULL;
}

bool hash_map_remove(struct hash_map *hash_map, const char *key)
{
    if (hash_map == NULL || hash_map->size == 0)
    {
        return false;
    }
    size_t hashe = hash(key);
    if (hashe >= hash_map->size)
    {
        hashe = hash(key) % hash_map->size;
    }
    if (hash_map->data[hashe] == NULL)
    {
        return false;
    }
    struct pair_list *paire_1 = hash_map->data[hashe];
    if (strcmp(paire_1->key, key) == 0)
    {
        hash_map->data[hashe] = paire_1->next;
        free(paire_1);
        return true;
    }
    struct pair_list *paire_2 = paire_1->next;
    for (; paire_2 != NULL; paire_2 = paire_2->next)
    {
        if (strcmp(paire_2->key, key) == 0)
        {
            if (paire_2->next != NULL)
            {
                paire_1->next = paire_2->next;
            }
            else
            {
                paire_1->next = NULL;
            }
            free(paire_2);
            return true;
        }
        paire_1 = paire_1->next;
    }
    return false;
}
