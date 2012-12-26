#include "config.h"

int gitorium_config_close(void)
{
    config_destroy(&aCfg);
    return 0;
}

int gitorium_config_save(void)
{
    /* This won't work if we aren't sudo
    if(!config_write_file(&aCfg, RC_FILE))
    {
        PRINT_ERROR("Could not save configuration file.")
        gitorium_config_close();
        return EXIT_FAILURE;
    }*/
    return 0;
}

int gitorium_config_init(void)
{
    config_init(&aCfg);

    if(!config_read_file(&aCfg, RC_FILE))
    {
        config_setting_t *root, *setting;

        root = config_root_setting(&aCfg);

        setting = config_setting_add(root, "repositories", CONFIG_TYPE_STRING);
        config_setting_set_string(setting, "/var/repositories/");
    }
    return 0;
}
