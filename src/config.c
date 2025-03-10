#include "gr-ini/config.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static struct gSection *oldsection = NULL;

gConfig *config_Create()
{
    gConfig *config = (gConfig *)malloc(sizeof(gConfig));
    if (config == NULL)
    {
        return NULL;
    }
    config->section = NULL;
    config->count = 0;

    return config;
}

void config_foreach_free(struct gSection *section)
{
    for (uint32_t i=0;i<section->subcount;i++)
    {
        config_foreach_free(&section->subsection[i]);
    }

    for (uint32_t i=0;i<section->count;i++)
    {
        gKeyValue *keyValue = &section->keyValue[i];

        if (keyValue->name != NULL)
        {
            free(keyValue->name);
        }

        if (keyValue->value != NULL)
        {
            free(keyValue->value);
        }
    }
    if (section->key != NULL)
    {
        free(section->key);
    }

    if (section->value != NULL)
    {
        free(section->value);
    }

    if (section->name != NULL)
    {
        free(section->name);
    }

    if (section->subsection != NULL)
    {
        free(section->subsection);
    }

    if (section->keyValue != NULL)
    {
        free(section->keyValue);
    }
}

void config_release(gConfig *config)
{
    for (uint32_t i=0;i<config->count;i++)
    {
        config_foreach_free(&config->section[i]);
    }

    if (config->section != NULL)
    {
        free(config->section);
    }

    free(config);
}

struct gSection *config_create_section(gConfig *config, const char *name, const char *value)
{
    if (config->section == NULL)
    {
        config->section = (struct gSection *)malloc(sizeof(struct gSection) * (config->count + 1));
    } else
    {
        config->section = (struct gSection *)realloc(config->section, sizeof(struct gSection) * (config->count + 1));
    }

    struct gSection *section = config->section + config->count;
    section->subsection = NULL;
    section->subcount = 0;

    section->key   = strdup(name);
    section->name  = strdup(name);

    section->value = NULL;

    section->keyValue = NULL;
    section->count = 0;

    if (value != NULL)
        config->section[config->count].value = strdup(value);

    ++config->count;

    return section;
}

struct gSection *config_create_sub_section(struct gSection *section, const char *name, const char *value)
{
    char new_key[512] = {0};
    sprintf(new_key, "%s.%s", section->key, name);

    if (section->subsection == NULL)
    {
        section->subsection = (struct gSection *)malloc(sizeof(struct gSection) * (section->subcount + 1));
    }
    else {
        section->subsection = (struct gSection *)realloc(section->subsection, sizeof(struct gSection) * (section->subcount + 1));
    }

    struct gSection *subsection = section->subsection + section->subcount;
    ++section->subcount;

    subsection->subsection = NULL;
    subsection->subcount = 0;

    subsection->key   = strdup(new_key);
    subsection->name  = strdup(name);

    subsection->value = NULL;

    if (value != NULL)
        subsection->value = strdup(value);

    subsection->keyValue = NULL;
    subsection->count = 0;

    return subsection;
}

struct gSection *config_foreach_section(struct gSection *section, const char *key)
{
    if (strcmp(section->key, key) == 0)
    {
        return section;
    }

    struct gSection *result = NULL;
    for (uint32_t i=0;i<section->subcount;i++)
    {
        result = config_foreach_section(&section->subsection[i], key);
        if (result != NULL)
        {
            break;
        }
    }
    return result;
}

struct gSection *config_find_section(const gConfig *config, const char *key)
{
    struct gSection *section = NULL;
    for (uint32_t i=0;i<config->count;i++)
    {
        section = config_foreach_section(&config->section[i], key);
        if (section != NULL)
        {
            break;
        }
    }
    return section;

    return NULL;
}

void config_set_string(struct gSection *section, const char *key, const char *value)
{
    if (section->keyValue == NULL)
    {
        section->keyValue = (gKeyValue *)malloc(sizeof(gKeyValue) * (section->count + 1));
    } else
    {
        section->keyValue = (gKeyValue *)realloc(section->keyValue, sizeof(gKeyValue) * (section->count + 1));
    }

    char typed[1024] = {0};
    sprintf(typed, "\"%s\"", value);

    gKeyValue *keyValue = section->keyValue + section->count;
    keyValue->value = strdup(typed);
    keyValue->name = strdup(key);

    ++section->count;
}

const char *config_get_string(const struct gSection *section, const char *key)
{
    for (uint32_t i=0;i<section->count;i++)
    {
        gKeyValue *keyValue = &section->keyValue[i];
        if (strcmp(keyValue->name, key) == 0)
        {
            return keyValue->value;
        }
    }
    return NULL;
}

void config_set_float(struct gSection *section, const char *key, float value)
{
    if (section->keyValue == NULL)
    {
        section->keyValue = (gKeyValue *)malloc(sizeof(gKeyValue) * (section->count + 1));
    } else
    {
        section->keyValue = (gKeyValue *)realloc(section->keyValue, sizeof(gKeyValue) * (section->count + 1));
    }

    char typed[1024] = {0};
    sprintf(typed, "%f", value);

    gKeyValue *keyValue = section->keyValue + section->count;
    keyValue->value = strdup(typed);
    keyValue->name = strdup(key);

    ++section->count;
}

float config_get_float(const struct gSection *section, const char *key)
{
    for (uint32_t i=0;i<section->count;i++)
    {
        gKeyValue *keyValue = &section->keyValue[i];
        if (strcmp(keyValue->name, key) == 0)
        {
            return atof(keyValue->value);
        }
    }
    return 0.0f;
}

void config_set_int(struct gSection *section, const char *key, int value)
{
    if (section->keyValue == NULL)
    {
        section->keyValue = (gKeyValue *)malloc(sizeof(gKeyValue) * (section->count + 1));
    } else
    {
        section->keyValue = (gKeyValue *)realloc(section->keyValue, sizeof(gKeyValue) * (section->count + 1));
    }

    char typed[1024] = {0};
    sprintf(typed, "%d", value);

    gKeyValue *keyValue = section->keyValue + section->count;
    keyValue->value = strdup(typed);
    keyValue->name = strdup(key);

    ++section->count;
}

int config_get_int(const struct gSection *section, const char *key)
{
    for (uint32_t i=0;i<section->count;i++)
    {
        gKeyValue *keyValue = &section->keyValue[i];
        if (strcmp(keyValue->name, key) == 0)
        {
            return atoi(keyValue->value);
        }
    }
    return 0;
}

void config_set_bool(struct gSection *section, const char *key, bool value)
{
    if (section->keyValue == NULL)
    {
        section->keyValue = (gKeyValue *)malloc(sizeof(gKeyValue) * (section->count + 1));
    } else
    {
        section->keyValue = (gKeyValue *)realloc(section->keyValue, sizeof(gKeyValue) * (section->count + 1));
    }

    gKeyValue *keyValue = section->keyValue + section->count;
    keyValue->value = strdup(value ? "true" : "false");
    keyValue->name = strdup(key);

    ++section->count;
}

bool config_get_bool(const struct gSection *section, const char *key)
{
    for (uint32_t i=0;i<section->count;i++)
    {
        gKeyValue *keyValue = &section->keyValue[i];
        if (strcmp(keyValue->name, key) == 0)
        {
            return strcmp(keyValue->value, "true") == 0 ? true : false;
        }
    }
    return false;
}
gConfigStep_ config_foreach_write(gConfigType_ type, void *data, void *fp)
{
    switch (type) {
        case gConfigType_Section: {
            struct gSection *section = (struct gSection *)data;
            
            char buff[1024] = {0};
            if (section->value != NULL)
            {
                sprintf(buff, "[%s %s]\n", section->key, section->value);
            } else
            {
                sprintf(buff, "[%s]\n", section->key);
            }


            size_t len = strlen(buff);

            if (fwrite(buff, 1, len, fp) != len)
            {
                printf("%s: %s - %ld\n", strerror(errno), buff, len);
            }
            break;
        }
        case gConfigType_Subsection: {
            struct gSection *section = (struct gSection *)data;
            
            char buff[1024] = {0};
            if (section->value != NULL)
            {
                sprintf(buff, "[%s %s]\n", section->key, section->value);
            } else
            {
                sprintf(buff, "[%s]\n", section->key);
            }


            size_t len = strlen(buff);

            if (fwrite(buff, 1, len, fp) != len)
            {
                printf("%s: %s - %ld\n", strerror(errno), buff, len);
            }
            break;
        }
        case gConfigType_KeyValue: {
            gKeyValue *keyValue = (gKeyValue *)data;

            char buff[1024] = {0};
            sprintf(buff, "%s=%s\n", keyValue->name, keyValue->value);

            size_t len = strlen(buff);

            if (fwrite(buff, 1, len, fp) != len)
            {
                printf("%s: %s - %ld\n", strerror(errno), buff, len);
            }
            break;
        }
        default:
            break;
    }
    return gConfigStep_Recuse;
}

void config_save(const char *file, const gConfig *config)
{
    FILE *fp = fopen(file, "w");
    if (fp == NULL)
    {
        return perror("Fail");
    }

    config_foreach(config, (ConfigFunc) &config_foreach_write, fp);

    fclose(fp);
}

void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

void config_load(const char *file, gConfig *config)
{
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        return perror("Fail");
    }

    char *line = NULL;
    size_t len = 0;

    struct gSection *section = NULL;
    struct gSection *subsection = NULL;

    while (getline(&line, &len, fp) > 0)
    {
        if (strstr(line, "[") != NULL && strstr(line, "]") != NULL)
        {
            removeChar(line, '[');
            removeChar(line, ']');
            removeChar(line, '\n');

            /*printf("AA: %s\n", line);*/

            char *key = strtok(line, " ");
            char *value = strtok(NULL, " ");
            
            if (strstr(key, ".") != NULL)
            {
                char *temp = strdup(key);

                char *token = strtok(temp, ".");
                while (token != NULL)
                {
                    char *token_end = strtok(NULL, ".");
                    if (token_end == NULL)
                    {
                        break;
                    }
                    token = token_end;
                }

                subsection = config_create_sub_section(section, token, value);

                free(temp);
            } else {
                section = subsection = config_create_section(config, key, value);
            }
        } else
        {
            removeChar(line, '\n');

            char *key = strtok(line, "=");
            char *value = strtok(NULL, "=");
            if (value == NULL)
            {
                continue;
            }

            size_t endc = strlen(value) - 1;
            if (value[0] == '"' && value[endc] == '"')
            {
                removeChar(value, '"');
                config_set_string(subsection, key, value);
            } else if (strcmp(value, "true") == 0)
            {
                config_set_bool(subsection, key, true);
            } else if (strcmp(value, "false") == 0)
            {
                config_set_bool(subsection, key, false);
            } else if (strstr(value, "."))
            {
                config_set_float(subsection, key, atof(value));
            } else
            {
                config_set_int(subsection, key, atoi(value));
            }
        }
    }

    free(line);

    fclose(fp);
}


bool config_for_earch(struct gSection *section, struct gSection *parent, ConfigFunc func, void *argv)
{
    gConfigStep_ step = func(parent == NULL ? gConfigType_Section : gConfigType_Subsection, (void *)section, argv);
    if (step == gConfigStep_Break)
    {
        return false;
    }

    if (step == gConfigStep_Continue)
    {
        return true;
    }

    if (step == gConfigStep_Recuse)
    {
        for (uint32_t i=0;i<section->count;i++)
        {
            step = func(gConfigType_KeyValue, (void *) &section->keyValue[i], argv);
            if (step == gConfigStep_Break)
            {
                return false;
            }
        }
    }

    for (uint32_t i=0;i<section->subcount;i++)
    {
        if (!config_for_earch(&section->subsection[i], section, func, argv))
        {
            return false;
        }
    }

    return true;
}

void config_foreach(const gConfig *config, ConfigFunc func, void *argv)
{
    for (uint32_t i=0;i<config->count;i++)
    {
        if (!config_for_earch(&config->section[i], NULL, func, argv))
        {
            /*return;*/
        }
    }
}

gConfigStep_ forearch(gConfigType_ type, void *data, void *argv)
{
    if (type == gConfigType_Section)
    {
        struct gSection *section = (struct gSection *)data;

        printf("[ %s %s ]\n", section->key, section->value);
        return gConfigStep_Recuse;
    }

    if (type == gConfigType_Subsection)
    {
        struct gSection *section = (struct gSection *)data;

        printf("\t[ %s %s ]\n", section->key, section->value);
        return gConfigStep_Recuse;
    }

    if (type == gConfigType_KeyValue)
    {
        gKeyValue *keyValue = (gKeyValue *)data;

        printf("\t\t%s=%s\n", keyValue->name, keyValue->value);
    }
    return gConfigStep_Continue;
}

void config_print(gConfig *config)
{
    config_foreach(config, (ConfigFunc) &forearch, NULL);
}



