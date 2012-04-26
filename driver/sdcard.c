/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "hardware.h"
#include "board.h"

#include "ff.h"

#include "debug.h"

#define CHUNK_SIZE	0x40000

unsigned int load_sdcard(void *dest)
{
	FATFS	fs;
	FIL 	file;
	FRESULT	fret;
	
	BYTE *pdata = dest;
	UINT byte_to_read = CHUNK_SIZE;
	UINT byte_read;
	char *filename = OS_IMAGE_NAME;

	at91_mci_hw_init();

	dbg_log(1, "Reading file %s from SD Card\n\r", filename);
	
	fret = f_mount(0, &fs);
	if (fret != FR_OK) {
		dbg_log(1, "*** FATFS: f_mount error **\n\r");
		return 1;
	}

	dbg_log(1, "load_sdcard01\n\r");
	fret = f_open(&file, filename, FA_OPEN_EXISTING | FA_READ);
	if (fret != FR_OK) {
		dbg_log(1, "*** FATFS: f_open, filename: [%s]: error\n\r", filename);
		return 1 ;
	}
	dbg_log(1, "load_sdcard02\n\r");

	do {
		byte_read = 0;
		fret = f_read(&file, (void *)(pdata), byte_to_read, &byte_read);
		pdata += byte_to_read;
	} while (byte_read >= byte_to_read);

	if (fret != FR_OK) {
		dbg_log(1, "*** FATFS: f_read: error\n\r");
		 return 1;
	}
	dbg_log(1, "load_sdcard03\n\r");

	fret = f_close(&file);

	return 0;
}
