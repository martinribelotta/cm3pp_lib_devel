/*
 * USBStream.h
 *
 *  Created on: 23/09/2012
 *      Author: ruso
 */

#ifndef USBSTREAM_H_
#define USBSTREAM_H_

#include "WriteStream.h"
#include "ReadStream.h"
#include <usbd_cdc_vcp.h>

namespace Stream {

class USBUpStream: public AbstractWriteStream {
protected:
	virtual void write(char c) {
		usb_cdc_putc(c);
	}

	virtual void write(const char *ptr, int size) {
		usb_cdc_write(ptr, size);
	}

private:
	USBUpStream() {
		usb_cdc_open();
	}

public:
	static USBUpStream singleton;
};

class USBDownStream: public AbstractReadStream {
protected:
	virtual bool read(char *c) {
		return usb_cdc_getc(c) != -1;
	}

	virtual int read(char *ptr, int size) {
		return usb_cdc_read(ptr, size);
	}

private:
	USBDownStream() {
	}

public:
	static USBDownStream singleton;
};

static USBUpStream& usbup = USBUpStream::singleton;
static USBDownStream& usbdown = USBDownStream::singleton;

}

#endif /* USBSTREAM_H_ */
