#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dlfcn.h>

#include "config.h"


static void OnClick()
{
    printf("OKKKKKKKKKKKK\n");
}

ConfigStep_ foreach(ConfigType_ type, void *data, void *argv)
{
    switch (type) {
        case ConfigType_Section: {
            struct Section *section = (struct Section *)data;
            if (strcmp(section->key, "MenuBar") == 0)
            {
                printf("MenBar found name: %s\n", section->value);

                return ConfigStep_Recuse;
            }
            break;
        }
        case ConfigType_Subsection: {
            struct Section *section = (struct Section *)data;

            printf("Subsection: %s-%s\n", section->key, section->value);
            /*if ()*/
            break;
        }
        default:
            break;
    }
    return ConfigStep_Continue;
}

int main()
{
    Config *config = config_Create();
    if (config == NULL)
    {
        printf("Fail to create config.\n");
        return 1;
    }

    /*
    {
        struct Section *section = config_create_section(config, "MenuBar", "Tools");
        {
            struct Section *subsection = config_create_sub_section(section, "MenuItem", NULL);
            config_set_string(subsection, "name", "Plugin Manager");
        }

        {
            struct Section *subsection = config_create_sub_section(section, "MenuItem", NULL);
            config_set_string(subsection, "name", "Shader Manager");
        }


        {
            struct Section *subsection = config_create_sub_section(section, "MenuItem", NULL);
            config_set_string(subsection, "name", "Texture Manager");
        }
    }

    {
        struct Section *section = config_create_section(config, "MenuBar", "Project");

        {
            struct Section *subsection = config_create_sub_section(section, "MenuItem", NULL);
            config_set_string(subsection, "name", "Recarregar Script");

            config_set_string(subsection, "title", "KKKKKKKKK");
            config_set_float(subsection, "timeout", 1000.5f);
            config_set_int(subsection, "reload", 13);
            config_set_bool(subsection, "enable", true);
        }


        {
            struct Section *subsection = config_create_sub_section(section, "MenuItem", NULL);
            config_set_string(subsection, "name", "Fecha Projeto");

            config_set_float(subsection, "timeout", 900.5f);
            config_set_int(subsection, "reload", 7);
            config_set_bool(subsection, "enable", false);
        }


        {
            struct Section *subsection = config_create_sub_section(section, "MenuItem", NULL);
            config_set_string(subsection, "name", "Reconfigurar");
        }
    }

    {
        struct Section *section = config_create_section(config, "Game", NULL);
        
        {
            struct Section *subsection = config_create_sub_section(section, "Info", NULL);
        }

        config_set_string(section, "name", "My First Game");
        config_set_string(section, "path", "/home/felypy/ssa");
    }
    

    config_save("config.ini", config);
    */

    config_load("config.ini", config);

    config_foreach(config, (ConfigFunc) &foreach, NULL);
    /*config_print(config);*/

    config_release(config);
}
