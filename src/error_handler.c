#ifndef __ERROR_HANDLER_H
#define __ERROR_HANDLER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "error_handler.h"
#include "stm32f0xx_hal.h"

void Error_Handler(void)
{
  __ASM("nop");
}

#ifdef __cplusplus
}
#endif
#endif /* __ERROR_HANDLER_H */
