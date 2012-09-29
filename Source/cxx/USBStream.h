/*
 * USBStream.h
 *
 *  Created on: 23/09/2012
 *      Author: ruso
 */

#ifndef USBSTREAM_H_
#define USBSTREAM_H_

#include "Stream.h"
#include <usbd_cdc_vcp.h>

namespace Stream {

class USBStream: public AbstractStream {
protected:
	virtual void write(char c) {
		usb_cdc_putc(c);
	}

	virtual void write(const char *ptr, int size) {
		usb_cdc_write(ptr, size);
	}

private:
	USBStream() {
		usb_cdc_open();
	}

public:
	static USBStream singleton;
};

static USBStream& usb = USBStream::singleton;

}

#endif /* USBSTREAM_H_ */
