/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_VCP_H
#define __USBD_CDC_VCP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>

/* Exported typef ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern int usb_cdc_open(void);
extern int usb_cdc_close(void);
extern int usb_cdc_read(char *buf, size_t cnt);
extern int usb_cdc_write(const char *buf, size_t cnt);
extern int usb_cdc_putc(const char c);
extern int usb_cdc_getc(char *c);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CDC_VCP_H */
