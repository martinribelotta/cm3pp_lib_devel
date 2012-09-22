#include "stm32f10x.h"
#include "usbd_cdc_vcp.h"
#include "usb_conf.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_dcd_int.h"

#include <ring_buffer.h>

static ring_buffer_t ring_rx;
static uint8_t ring_buffer_rx[128];

struct {
	uint32_t bitrate;
	uint8_t format;
	uint8_t paritytype;
	uint8_t datatype;
} linecoding = { 115200, 0x00, 0x00, 0x08 };

/* These are external variables imported from CDC core to be used for IN 
 transfer management. */
extern uint8_t APP_Rx_Buffer[]; /* Write CDC received data in this buffer.
 These data will be sent over USB IN endpoint
 in the CDC core functions. */

extern uint32_t APP_Rx_ptr_in; /* Increment this pointer or roll it back to
 start address when writing received data
 in the buffer APP_Rx_Buffer. */
extern uint32_t APP_Rx_ptr_out;

static uint16_t VCP_Init(void);
static uint16_t VCP_DeInit(void);
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataRx(uint8_t* Buf, uint32_t Len);

CDC_IF_Prop_TypeDef VCP_fops = { //
		//
				VCP_Init, //
				VCP_DeInit, //
				VCP_Ctrl, //
				VCP_DataTx, //
				VCP_DataRx //
		};

/**
 * @brief  VCP_Init
 *         Initializes the Media on the STM32
 * @param  None
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
static uint16_t VCP_Init(void) {
	rb_init(&ring_rx, sizeof(ring_buffer_rx), ring_buffer_rx);
	return USBD_OK;
}

/**
 * @brief  VCP_DeInit
 *         DeInitializes the Media on the STM32
 * @param  None
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
static uint16_t VCP_DeInit(void) {
	return USBD_OK;
}

/**
 * @brief  VCP_Ctrl
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len) {
	switch (Cmd) {
	case SEND_ENCAPSULATED_COMMAND:
		/* Not  needed for this driver */
		break;

	case GET_ENCAPSULATED_RESPONSE:
		/* Not  needed for this driver */
		break;

	case SET_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case GET_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case CLEAR_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case SET_LINE_CODING:
		linecoding.bitrate = (uint32_t) (Buf[0] | (Buf[1] << 8) | (Buf[2] << 16)
				| (Buf[3] << 24));
		linecoding.format = Buf[4];
		linecoding.paritytype = Buf[5];
		linecoding.datatype = Buf[6];
		break;

	case GET_LINE_CODING:
		Buf[0] = (uint8_t) (linecoding.bitrate);
		Buf[1] = (uint8_t) (linecoding.bitrate >> 8);
		Buf[2] = (uint8_t) (linecoding.bitrate >> 16);
		Buf[3] = (uint8_t) (linecoding.bitrate >> 24);
		Buf[4] = linecoding.format;
		Buf[5] = linecoding.paritytype;
		Buf[6] = linecoding.datatype;
		break;

	case SET_CONTROL_LINE_STATE:
		/* Not  needed for this driver */
		break;

	case SEND_BREAK:
		/* Not  needed for this driver */
		break;

	default:
		break;
	}

	return USBD_OK;
}

/**
 * @brief  VCP_DataTx
 *         CDC received data to be send over USB IN endpoint are managed in
 *         this function.
 * @param  Buf: Buffer of data to be sent
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
 */
static uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len) {
	return USBD_OK;
}

/**
 * @brief  VCP_DataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 *
 *         @note
 *         This function will block any OUT packet reception on USB endpoint
 *         untill exiting this function. If you exit this function before transfer
 *         is complete on CDC interface (ie. using DMA controller) it will result
 *         in receiving more data while previous ones are still not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
 */
static uint16_t VCP_DataRx(uint8_t* Buf, uint32_t Len) {
	while (Len--)
		rb_push_insert(&ring_rx, *Buf++);
	return USBD_OK;
}

static USB_OTG_CORE_HANDLE USB_OTG_dev;

void OTG_FS_IRQHandler(void) {
	USBD_OTG_ISR_Handler(&USB_OTG_dev);
}

int usb_cdc_open(void) {
	USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb,
			&USR_cb);
	return 0;
}

int usb_cdc_close(void) {
	USBD_DeInit(&USB_OTG_dev);
	return 0;
}

int usb_cdc_read(char *buf, size_t cnt) {
#if 0
	int readed;
	for (readed = 0; (readed < cnt) && !rb_is_empty(&ring_rx); readed--)
		*buf++ = rb_remove(&ring_rx);
	return readed;
#else
	int readed = 0;
	while(readed < cnt) {
		if (usb_cdc_getc(&buf[readed]) == -1)
			break;
		readed++;
	}
#endif
	return readed;
}

int usb_cdc_getc(char *c) {
	if (rb_is_empty(&ring_rx)) {
		if (c)
			*c = rb_remove(&ring_rx);
		return 0;
	} else
		return -1;
}

static int usb_is_full(void) {
	return APP_Rx_ptr_in == (APP_Rx_ptr_out - 1);
}

int usb_cdc_write(const char *buf, size_t cnt) {
#if 0
	int writed;
	for (writed = 0; (writed < cnt) && !usb_is_full(); writed++) {
		APP_Rx_Buffer[APP_Rx_ptr_in] = *buf++;
		APP_Rx_ptr_in++;
		/* To avoid buffer overflow */
		if (APP_Rx_ptr_in == APP_RX_DATA_SIZE)
			APP_Rx_ptr_in = 0;
	}
#else
	int writed = 0;
	while(writed < cnt) {
		if (usb_cdc_putc(buf[writed]) == -1)
			break;
		writed++;
	}
#endif
	return writed;
}

int usb_cdc_putc(const char c) {
	if (!usb_is_full()) {
		APP_Rx_Buffer[APP_Rx_ptr_in] = c;
		APP_Rx_ptr_in++;
		/* To avoid buffer overflow */
		if (APP_Rx_ptr_in == APP_RX_DATA_SIZE)
			APP_Rx_ptr_in = 0;
		return 0;
	} else
		return -1;
}
