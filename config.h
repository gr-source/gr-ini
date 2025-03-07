#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

enum ConfigStep
{
    ConfigStep_Continue = 1 << 1,
    ConfigStep_Recuse   = 1 << 2,
    ConfigStep_Break    = 1 << 3
};

typedef int ConfigStep_;
typedef int ConfigType_;

enum ConfigType
{
    ConfigType_Section    = 1 << 1,
    ConfigType_Subsection = 1 << 2,
    ConfigType_KeyValue   = 1 << 3
};


typedef struct
{
    char *name;
    char *value;
} KeyValue;

struct Section
{
    char *key;
    char *value;

    KeyValue *keyValue;
    uint32_t count;

    struct Section *subsection;
    uint32_t subcount;
};

typedef struct  {
    struct Section *section;
    uint32_t count;
} Config;

typedef ConfigType_ (*ConfigFunc)(ConfigType_, void *, void *);

Config *config_Create();

void config_release(Config *config);

struct Section *config_create_section(Config *config, const char *name, const char *value);

struct Section *config_create_sub_section(struct Section *section, const char *name, const char *value);

struct Section *config_find_section(const Config *config, const char *key);

struct Section *config_get_section(const Config *config, const char *key);

void config_set_string(struct Section *section, const char *key, const char *value);

void config_set_float(struct Section *section, const char *key, float value);

void config_set_int(struct Section *section, const char *key, int value);

void config_set_bool(struct Section *section, const char *key, bool value);

void config_save(const char *file, const Config *config);

void config_load(const char *file, Config *config);

void config_foreach(const Config *config, ConfigFunc func, void *argv);

void config_print(Config *config);

#endif // CONFIG_H
