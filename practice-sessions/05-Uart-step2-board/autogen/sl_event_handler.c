#include "sl_event_handler.h"

#include "sl_board_init.h"
#include "sl_clock_manager.h"
#include "sl_board_control.h"
#include "sl_gpio.h"

void sli_driver_permanent_allocation(void)
{
}

void sli_service_permanent_allocation(void)
{
}

void sli_stack_permanent_allocation(void)
{
}

void sli_internal_permanent_allocation(void)
{
}

void sl_platform_init(void)
{
  sl_board_preinit();
  sl_clock_manager_runtime_init();
  sl_board_init();
}

void sli_internal_init_early(void)
{
}

void sl_driver_init(void)
{
  sl_gpio_init();
}

void sl_service_init(void)
{
  sl_board_configure_vcom();
}

void sl_stack_init(void)
{
}

void sl_internal_app_init(void)
{
}

void sli_platform_process_action(void)
{
}

void sli_service_process_action(void)
{
}

void sli_stack_process_action(void)
{
}

void sli_internal_app_process_action(void)
{
}

