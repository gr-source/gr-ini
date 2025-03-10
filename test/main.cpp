#include <iostream>

#include <string.h>

extern "C" {
#include <gr-ini/config.h>
}

gConfigStep_ foreach(gConfigType_ type, void *data, void *argv)
{
    switch (type) {
        case gConfigType_Section: {
            struct gSection *section = (struct gSection *)data;
            if (strcmp(section->name, "MenuBar") == 0)
            {
                printf("MenuBar found name: %s\n", section->value);

                return gConfigStep_Recuse;
            }
            break;
        }
        case gConfigType_Subsection: {
            struct gSection *section = (struct gSection *)data;

            printf("Subsection: %s-%s\n", section->name, section->value);
            /*if ()*/
            break;
        }
        default:
            break;
    }
    return gConfigStep_Continue;
}

int main()
{
    gConfig *config = config_Create();
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
        config_set_bool(section, "runner", true);
        config_set_int(section, "max_count", 89);
        config_set_float(section, "timeout", 1000.24f);
    } 

    config_save("config.ini", config);
    */
    
    config_load("config.ini", config);

    struct gSection *section = config_find_section(config, "Game");

    std::cout << "Result" << std::endl;
    /*printf("path: %s, runner: %s, max: %d, time: %f\n",*/
    /*        config_get_string(section, "path"),*/
    /*       (config_get_bool(section, "runner") ? "true" : false),*/
    /*       config_get_int(section, "max_count"), config_get_float(section, "timeout"));*/

    /*config_foreach(config, (ConfigFunc) &foreach, NULL);*/
    /*config_print(config);*/

    config_release(config);
}
