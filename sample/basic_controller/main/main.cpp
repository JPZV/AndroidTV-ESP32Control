#include <main.h>

#define LOG_TAG "BASIC_TV"

TVControl tvControl;
TVControlConfiguration tvControlConfig;

TaskHandle_t loopTaskHandle = NULL;

void init_buttons();
bool is_button_pressed(gpio_num_t gpio);

void app_loop(void *params)
{
    while (true)
    {
        if (tvControl.isConnected())
        {
            tvControl.setState(TV_BUTTON_MENU_PICK,             is_button_pressed((gpio_num_t)MENU_PICK_PIN));
            tvControl.setState(TV_BUTTON_MENU_UP,               is_button_pressed((gpio_num_t)MENU_UP_PIN));
            tvControl.setState(TV_BUTTON_MENU_DOWN,             is_button_pressed((gpio_num_t)MENU_DOWN_PIN));
            tvControl.setState(TV_BUTTON_MENU_LEFT,             is_button_pressed((gpio_num_t)MENU_LEFT_PIN));
            tvControl.setState(TV_BUTTON_MENU_RIGHT,            is_button_pressed((gpio_num_t)MENU_RIGHT_PIN));
            tvControl.setState(TV_BUTTON_POWER,                 is_button_pressed((gpio_num_t)POWER_PIN));
            tvControl.setState(TV_BUTTON_VOL_INCREMENT,         is_button_pressed((gpio_num_t)VOL_INC_PIN));
            tvControl.setState(TV_BUTTON_VOL_DECREMENT,         is_button_pressed((gpio_num_t)VOL_DEC_PIN));
            tvControl.setState(TV_BUTTON_LAUNCH_SELECT,         is_button_pressed((gpio_num_t)LAUNCHER_PIN));
            tvControl.setState(TV_BUTTON_LAUNCH_MOVIE_BROWSER,  is_button_pressed((gpio_num_t)MOVIES_PIN));
            tvControl.setState(TV_BUTTON_MENU_GREEN,            is_button_pressed((gpio_num_t)MENU_GREEN_PIN));
            tvControl.setState(TV_BUTTON_MENU_BLUE,             is_button_pressed((gpio_num_t)MENU_BLUE_PIN));
            tvControl.setState(TV_BUTTON_MENU_YELLOW,           is_button_pressed((gpio_num_t)MENU_YELLOW_PIN));
            tvControl.setState(TV_BUTTON_LAUNCH_EMAIL_READER,   is_button_pressed((gpio_num_t)EMAIL_PIN));
            tvControl.setState(TV_BUTTON_LAUNCH_CALCULATOR,     is_button_pressed((gpio_num_t)CALC_PIN));
            tvControl.setState(TV_BUTTON_LAUNCH_LOCAL_BROWSER,  is_button_pressed((gpio_num_t)LOCAL_PIN));
            tvControl.setState(TV_BUTTON_APP_HOME,              is_button_pressed((gpio_num_t)HOME_PIN));
            tvControl.setState(TV_BUTTON_APP_BACK,              is_button_pressed((gpio_num_t)BACK_PIN));
            tvControl.setState(TV_BUTTON_APP_FORWARD,           is_button_pressed((gpio_num_t)FORWARD_PIN));
            tvControl.setState(TV_BUTTON_APP_STOP,              is_button_pressed((gpio_num_t)STOP_PIN));
            tvControl.setState(TV_BUTTON_APP_REFRESH,           is_button_pressed((gpio_num_t)REFRESH_PIN));
            tvControl.setState(TV_BUTTON_APP_BOOKMARKS,         is_button_pressed((gpio_num_t)BOOKMARKS_PIN));

            tvControl.sendReport();
        }
    }
}

extern "C" void app_main(void)
{
    tvControlConfig.setAutoReport(false);
    tvControl.begin(&tvControlConfig);

    init_buttons();

    xTaskCreatePinnedToCore(app_loop, "APP_LOOP", 4096, NULL, tskIDLE_PRIORITY, &loopTaskHandle, 1);
    configASSERT(loopTaskHandle);

    ESP_LOGI(LOG_TAG, "Started!");
}

void init_buttons()
{
    gpio_config_t io_conf = {};
    int8_t buttons[22] =
    {
        MENU_PICK_PIN,
        MENU_UP_PIN,
        MENU_DOWN_PIN,
        MENU_LEFT_PIN,
        MENU_RIGHT_PIN,
        POWER_PIN,
        VOL_INC_PIN,
        VOL_DEC_PIN,
        LAUNCHER_PIN,
        MOVIES_PIN,
        MENU_GREEN_PIN,
        MENU_BLUE_PIN,
        MENU_YELLOW_PIN,
        EMAIL_PIN,
        CALC_PIN,
        LOCAL_PIN,
        HOME_PIN,
        BACK_PIN,
        FORWARD_PIN,
        STOP_PIN,
        REFRESH_PIN,
        BOOKMARKS_PIN
    };

    for (int i = 0; i < 22; i++)
    {
        if (buttons[i] < 0)
        {
            continue;
        }
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.pin_bit_mask = (1ULL<<buttons[i]);
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        gpio_config(&io_conf);
    }
}

bool is_button_pressed(gpio_num_t gpio)
{
    if (gpio < 0)
    {
        return false;
    }
    return !gpio_get_level(gpio);
}