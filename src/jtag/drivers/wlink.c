#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>

#include <jtag/interface.h>
#include <jtag/commands.h>

#include "wlink.h"

#include "libusb_helper.h"

struct libusb_device_handle *wfd;

int pReadData(int index, int channel, uint8_t *data, int *size)
{
	int transferred;

	if (channel == 1)
		jtag_libusb_bulk_read(wfd, BULK_EP_IN_1, (char *)data, *size, USB_TIMEOUT, &transferred);
	else
		jtag_libusb_bulk_read(wfd, BULK_EP_IN_2, (char *)data, *size, USB_TIMEOUT, &transferred);
	return 1;
}

int pWriteData(int index, int endpoint, uint8_t *data, int *size)
{
	int transferred;

	jtag_libusb_bulk_write(wfd, endpoint, (char *)data, *size, USB_TIMEOUT, &transferred);
	return 1;
}

static int wlink_init(void)
{
	uint8_t wdata[9] = {0x81, 0xD, 0x1, 0x1}, rdata[9];
	int length;

	const uint16_t vids[] = { VID, 0 };
	const uint16_t pids[] = { PID, 0 };

	if (jtag_libusb_open(vids, pids, &wfd, NULL) != ERROR_OK) {
		LOG_ERROR("Failed to open or find the device");
		goto err;
	}
	jtag_libusb_set_configuration(wfd, 0);
	if (libusb_claim_interface(wfd, 0) != ERROR_OK) {
		LOG_ERROR("Failed to claim interface");
		goto err;
	}

	length = 4;
	pWriteData(0, 1, wdata, &length);
	length = 6;
	pReadData(0, 1, rdata, &length);
	LOG_INFO("WCH-Link version %d.%d", rdata[3], rdata[4]);

	length = 4;
	wdata[3] = 0x2;
	pWriteData(0, 1, wdata, &length);
	length = 6;
	pReadData(0, 1, rdata, &length);
	if (rdata[0] == 0x81 && rdata[1] == 0x55 && rdata[2] == 1 && rdata[3] == 1) {
		LOG_ERROR("Failed to detect chip");
		goto err;
	}

	return ERROR_OK;
err:
	if (wfd)
		jtag_libusb_close(wfd);
	return ERROR_FAIL;
}

static int wlink_execute_queue(void)
{
	return ERROR_OK;
}

static int wlink_quit(void)
{
	jtag_libusb_close(wfd);
	return ERROR_OK;
}

static const struct command_registration wlink_command_handlers[] = {
//TODO
	COMMAND_REGISTRATION_DONE
};

static struct jtag_interface wlink_interface = {
	.execute_queue = wlink_execute_queue,
};

struct adapter_driver wlink_adapter_driver = {
	.name = "wlink",
	.transports = jtag_only,
	.commands = wlink_command_handlers,

	.init = wlink_init,
	.quit = wlink_quit,

	.jtag_ops = &wlink_interface,
};