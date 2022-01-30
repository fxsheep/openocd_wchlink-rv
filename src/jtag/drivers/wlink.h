/***************************************************************************
 *   Copyright (C) 2008 Lou Deluxe                                         *
 *   lou.openocd012@fixit.nospammail.net                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef OPENOCD_JTAG_DRIVERS_WLINK_H
#define OPENOCD_JTAG_DRIVERS_WLINK_H

#define VID 0x1a86
#define PID 0x8010

#define BULK_EP_IN_1  0x81
#define BULK_EP_IN_2  0x82

#define USB_TIMEOUT 3000

int pReadData(int index, int channel, uint8_t *data, int *size);
int pWriteData(int index, int endpoint, uint8_t *data, int *size);

#endif /* OPENOCD_JTAG_DRIVERS_WLINK_H */
