#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

enum gConfigStep
{
    gConfigStep_Continue = 1 << 1,
    gConfigStep_Recuse   = 1 << 2,
    gConfigStep_Break    = 1 << 3
};

typedef int gConfigStep_;
typedef int gConfigType_;

enum gConfigType
{
    gConfigType_Section    = 1 << 1,
    gConfigType_Subsection = 1 << 2,
    gConfigType_KeyValue   = 1 << 3
};


typedef struct
{
    char *name;
    char *value;
} gKeyValue;

struct gSection
{
    char *key;
    char *value;

    char *name;

    gKeyValue *keyValue;
    uint32_t count;

    struct gSection *subsection;
    uint32_t subcount;
};

typedef struct  {
    struct gSection *section;
    uint32_t count;
} gConfig;

typedef gConfigType_ (*ConfigFunc)(gConfigType_, void *, void *);

gConfig *config_Create();

void config_release(gConfig *config);

struct gSection *config_create_section(gConfig *config, const char *name, const char *value);

struct gSection *config_create_sub_section(struct gSection *section, const char *name, const char *value);

struct gSection *config_find_section(const gConfig *config, const char *key);

void config_set_string(struct gSection *section, const char *key, const char *value);

const char *config_get_string(const struct gSection *section, const char *key);

void config_set_float(struct gSection *section, const char *key, float value);

float config_get_float(const struct gSection *section, const char *key);

void config_set_int(struct gSection *section, const char *key, int value);

int config_get_int(const struct gSection *section, const char *key);

void config_set_bool(struct gSection *section, const char *key, bool value);

bool config_get_bool(const struct gSection *section, const char *key);

void config_save(const char *file, const gConfig *config);

void config_load(const char *file, gConfig *config);

void config_foreach(const gConfig *config, ConfigFunc func, void *argv);

void config_print(gConfig *config);

#endif // CONFIG_H
