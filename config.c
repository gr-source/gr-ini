#include "config.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static struct Section *oldsection = NULL;

Config *config_Create()
{
    Config *config = (Config *)malloc(sizeof(struct Section));
    if (config == NULL)
    {
        return NULL;
    }
    config->section = NULL;
    config->count = 0;

    return config;
}

void config_foreach_free(struct Section *section)
{
    for (uint32_t i=0;i<section->subcount;i++)
    {
        config_foreach_free(&section->subsection[i]);
    }

    for (uint32_t i=0;i<section->count;i++)
    {
        KeyValue *keyValue = &section->keyValue[i];

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

void config_release(Config *config)
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

struct Section *config_create_section(Config *config, const char *name, const char *value)
{
    if (config->section == NULL)
    {
        config->section = (struct Section *)malloc(sizeof(struct Section) * (config->count + 1));
    } else
    {
        config->section = (struct Section *)realloc(config->section, sizeof(struct Section) * (config->count + 1));
    }

    struct Section *section = config->section + config->count;
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

struct Section *config_create_sub_section(struct Section *section, const char *name, const char *value)
{
    char new_key[512] = {0};
    sprintf(new_key, "%s.%s", section->key, name);

    if (section->subsection == NULL)
    {
        section->subsection = (struct Section *)malloc(sizeof(struct Section) * (section->subcount + 1));
    }
    else {
        section->subsection = (struct Section *)realloc(section->subsection, sizeof(struct Section) * (section->subcount + 1));
    }

    struct Section *subsection = section->subsection + section->subcount;
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

struct Section *config_foreach_section(struct Section *section, const char *key)
{
    if (strcmp(section->key, key) == 0)
    {
        return section;
    }

    struct Section *result = NULL;
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

struct Section *config_find_section(const Config *config, const char *key)
{
    struct Section *section = NULL;
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

void config_set_string(struct Section *section, const char *key, const char *value)
{
    if (section->keyValue == NULL)
    {
        section->keyValue = (KeyValue *)malloc(sizeof(KeyValue) * (section->count + 1));
    } else
    {
        section->keyValue = (KeyValue *)realloc(section->keyValue, sizeof(KeyValue) * (section->count + 1));
    }

    char typed[1024] = {0};
    sprintf(typed, "\"%s\"", value);

    KeyValue *keyValue = section->keyValue + section->count;
    keyValue->value = strdup(typed);
    keyValue->name = strdup(key);

    ++section->count;
}

const char *config_get_string(const struct Section *section, const char *key)
{
    for (uint32_t i=0;i<section->count;i++)
    {
        KeyValue *keyValue = &section->keyValue[i];
        if (strcmp(keyValue->name, key) == 0)
        {
            return keyValue->value;
        }
    }
    return NULL;
}

void config_set_float(struct Section *section, const char *key, float value)
{
    if (section->keyValue == NULL)
    {
        section->keyValue = (KeyValue *)malloc(sizeof(KeyValue) * (section->count + 1));
    } else
    {
        section->keyValue = (KeyValue *)realloc(section->keyValue, sizeof(KeyValue) * (section->count + 1));
    }

    char typed[1024] = {0};
    sprintf(typed, "%f", value);

    KeyValue *keyValue = section->keyValue + section->count;
    keyValue->value = strdup(typed);
    keyValue->name = strdup(key);

    ++section->count;
}

float config_get_float(const struct Section *section, const char *key)
{
    for (uint32_t i=0;i<section->count;i++)
    {
        KeyValue *keyValue = &section->keyValue[i];
        if (strcmp(keyValue->name, key) == 0)
        {
            return atof(keyValue->value);
        }
    }
    return 0.0f;
}

void config_set_int(struct Section *section, const char *key, int value)
{
    if (section->keyValue == NULL)
    {
        section->keyValue = (KeyValue *)malloc(sizeof(KeyValue) * (section->count + 1));
    } else
    {
        section->keyValue = (KeyValue *)realloc(section->keyValue, sizeof(KeyValue) * (section->count + 1));
    }

    char typed[1024] = {0};
    sprintf(typed, "%d", value);

    KeyValue *keyValue = section->keyValue + section->count;
    keyValue->value = strdup(typed);
    keyValue->name = strdup(key);

    ++section->count;
}

int config_get_int(const struct Section *section, const char *key)
{
    for (uint32_t i=0;i<section->count;i++)
    {
        KeyValue *keyValue = &section->keyValue[i];
        if (strcmp(keyValue->name, key) == 0)
        {
            return atoi(keyValue->value);
        }
    }
    return 0;
}

void config_set_bool(struct Section *section, const char *key, bool value)
{
    if (section->keyValue == NULL)
    {
        section->keyValue = (KeyValue *)malloc(sizeof(KeyValue) * (section->count + 1));
    } else
    {
        section->keyValue = (KeyValue *)realloc(section->keyValue, sizeof(KeyValue) * (section->count + 1));
    }

    KeyValue *keyValue = section->keyValue + section->count;
    keyValue->value = strdup(value ? "true" : "false");
    keyValue->name = strdup(key);

    ++section->count;
}

bool config_get_bool(const struct Section *section, const char *key)
{
    for (uint32_t i=0;i<section->count;i++)
    {
        KeyValue *keyValue = &section->keyValue[i];
        if (strcmp(keyValue->name, key) == 0)
        {
            return strcmp(keyValue->value, "true") == 0 ? true : false;
        }
    }
    return false;
}
ConfigStep_ config_foreach_write(ConfigType_ type, void *data, void *fp)
{
    switch (type) {
        case ConfigType_Section: {
            struct Section *section = (struct Section *)data;
            
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
        case ConfigType_Subsection: {
            struct Section *section = (struct Section *)data;
            
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
        case ConfigType_KeyValue: {
            KeyValue *keyValue = (KeyValue *)data;

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
    return ConfigStep_Recuse;
}

void config_save(const char *file, const Config *config)
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

void config_load(const char *file, Config *config)
{
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        return perror("Fail");
    }

    char *line = NULL;
    size_t len = 0;

    struct Section *section = NULL;
    struct Section *subsection = NULL;

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


bool config_for_earch(struct Section *section, struct Section *parent, ConfigFunc func, void *argv)
{
    ConfigStep_ step = func(parent == NULL ? ConfigType_Section : ConfigType_Subsection, (void *)section, argv);
    if (step == ConfigStep_Break)
    {
        return false;
    }

    if (step == ConfigStep_Continue)
    {
        return true;
    }

    if (step == ConfigStep_Recuse)
    {
        for (uint32_t i=0;i<section->count;i++)
        {
            step = func(ConfigType_KeyValue, (void *) &section->keyValue[i], argv);
            if (step == ConfigStep_Break)
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

void config_foreach(const Config *config, ConfigFunc func, void *argv)
{
    for (uint32_t i=0;i<config->count;i++)
    {
        if (!config_for_earch(&config->section[i], NULL, func, argv))
        {
            /*return;*/
        }
    }
}

ConfigStep_ forearch(ConfigType_ type, void *data, void *argv)
{
    if (type == ConfigType_Section)
    {
        struct Section *section = (struct Section *)data;

        printf("[ %s %s ]\n", section->key, section->value);
        return ConfigStep_Recuse;
    }

    if (type == ConfigType_Subsection)
    {
        struct Section *section = (struct Section *)data;

        printf("\t[ %s %s ]\n", section->key, section->value);
        return ConfigStep_Recuse;
    }

    if (type == ConfigType_KeyValue)
    {
        KeyValue *keyValue = (KeyValue *)data;

        printf("\t\t%s=%s\n", keyValue->name, keyValue->value);
    }
    return ConfigStep_Continue;
}

void config_print(Config *config)
{
    config_foreach(config, (ConfigFunc) &forearch, NULL);
}



