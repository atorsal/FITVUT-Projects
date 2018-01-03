/**
 * main.c
 *
 *  Created on: 5.11.2017
 *      Author: Dávid Bolvanský (xbolva00)
 */

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#include "uart.h"
#include "crc_impl.h"

#include "stdio.h"
#include "string.h"

// Valid CRC values for our used data
const uint32_t CRC32_NO_ERRORS = 0x353ea28c;
const uint32_t CRC16_NO_ERRORS = 0x8d2f;

const uint32_t CRC32_ONE_ERROR = 0xF8D55062;
const uint32_t CRC16_ONE_ERROR = 0x776F;

const uint32_t CRC32_MORE_ERRORS = 0x2A978941;
const uint32_t CRC16_MORE_ERRORS = 0x1DBE;

/**
 * CRC16/32 tests, error detections
 */
void CRC_test(void) {
	// No errors in data
	char * sample_data =
			"Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet.";

	UART5_print("| No errors in data |\r\n");

	if (CRC32_NO_ERRORS
			!= crc_module_compute_crc32(sample_data, strlen(sample_data))) {
		UART5_print("MODULE CRC32\tFAILED\r\n");
	} else {
		UART5_print("MODULE CRC32\tOK\r\n");
	}

	if (CRC16_NO_ERRORS
			!= crc_module_compute_crc16(sample_data, strlen(sample_data))) {
		UART5_print("MODULE CRC16\tFAILED\r\n");
	} else {
		UART5_print("MODULE CRC16\tOK\r\n");
	}

	if (CRC32_NO_ERRORS
			!= crc_table_compute_crc32(sample_data, strlen(sample_data))) {
		UART5_print("TABLE CRC32\tFAILED\r\n");
	} else {
		UART5_print("TABLE CRC32\tOK\r\n");
	}

	if (CRC16_NO_ERRORS
			!= crc_table_compute_crc16(sample_data, strlen(sample_data))) {
		UART5_print("TABLE CRC16\tFAILED\r\n");
	} else {
		UART5_print("TABLE CRC16\tOK\r\n");
	}

	if (CRC32_NO_ERRORS
			!= crc_basic_compute_crc32(sample_data, strlen(sample_data))) {
		UART5_print("BASIC CRC62\tFAILED\r\n");
	} else {
		UART5_print("BASIC CRC32\tOK\r\n");
	}

	if (CRC16_NO_ERRORS
			!= crc_basic_compute_crc16(sample_data, strlen(sample_data))) {
		UART5_print("BASIC CRC16\tFAILED\r\n");
	} else {
		UART5_print("BASIC CRC16\tOK\r\n");
	}

	// One error in data
	sample_data =
			"Loram ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet.";

	UART5_print("| One error in data |\r\n");

	if (CRC32_ONE_ERROR
			!= crc_module_compute_crc32(sample_data, strlen(sample_data))) {
		UART5_print("MODULE CRC32\tFAILED\r\n");
	} else {
		UART5_print("MODULE CRC32\tOK\r\n");
	}

	if (CRC16_ONE_ERROR
			!= crc_module_compute_crc16(sample_data, strlen(sample_data))) {
		UART5_print("MODULE CRC16\tFAILED\r\n");
	} else {
		UART5_print("MODULE CRC16\tOK\r\n");
	}

	if (CRC32_ONE_ERROR
			!= crc_table_compute_crc32(sample_data, strlen(sample_data))) {
		UART5_print("TABLE CRC32\tFAILED\r\n");
	} else {
		UART5_print("TABLE CRC32\tOK\r\n");
	}

	if (CRC16_ONE_ERROR
			!= crc_table_compute_crc16(sample_data, strlen(sample_data))) {
		UART5_print("TABLE CRC16\tFAILED\r\n");
	} else {
		UART5_print("TABLE CRC16\tOK\r\n");
	}

	if (CRC32_ONE_ERROR
			!= crc_basic_compute_crc32(sample_data, strlen(sample_data))) {
		UART5_print("BASIC CRC32\tFAILED\r\n");
	} else {
		UART5_print("BASIC CRC32\tOK\r\n");
	}

	if (CRC16_ONE_ERROR
			!= crc_basic_compute_crc16(sample_data, strlen(sample_data))) {
		UART5_print("BASIC CRC16\tFAILED\r\n");
	} else {
		UART5_print("BASIC CRC16\tOK\r\n");
	}

	// More errors in data
	sample_data =
			"Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commoda ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat b, tellus. Phasellus viverra nulla at metus varius laoreet.";

	UART5_print("| More errors in data |\r\n");

	if (CRC32_MORE_ERRORS
			!= crc_module_compute_crc32(sample_data, strlen(sample_data))) {
		UART5_print("MODULE CRC32\tFAILED\r\n");
	} else {
		UART5_print("MODULE CRC32\tOK\r\n");
	}

	if (CRC16_MORE_ERRORS
			!= crc_module_compute_crc16(sample_data, strlen(sample_data))) {
		UART5_print("MODULE CRC16\tFAILED\r\n");
	} else {
		UART5_print("MODULE CRC16\tOK\r\n");
	}

	if (CRC32_MORE_ERRORS
			!= crc_table_compute_crc32(sample_data, strlen(sample_data))) {
		UART5_print("TABLE CRC32\tFAILED\r\n");
	} else {
		UART5_print("TABLE CRC32\tOK\r\n");
	}

	if (CRC16_MORE_ERRORS
			!= crc_table_compute_crc16(sample_data, strlen(sample_data))) {
		UART5_print("TABLE CRC16\tFAILED\r\n");
	} else {
		UART5_print("TABLE CRC16\tOK\r\n");
	}

	if (CRC32_MORE_ERRORS
			!= crc_basic_compute_crc32(sample_data, strlen(sample_data))) {
		UART5_print("BASIC CRC32\tFAILED\r\n");
	} else {
		UART5_print("BASIC CRC32\tOK\r\n");
	}

	if (CRC16_MORE_ERRORS
			!= crc_basic_compute_crc16(sample_data, strlen(sample_data))) {
		UART5_print("BASIC CRC16\tFAILED\r\n");
	} else {
		UART5_print("BASIC CRC16\tOK\r\n");
	}

}

/*!
 * @brief Application entry point.
 */
int main(void) {
	/* Init board hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	UART5_Init();

	CRC_test();

	/* Add your code here */

	for (;;) { /* Infinite loop to avoid leaving the main function */
		__asm("NOP");
		/* something to use as a breakpoint stop while looping */
	}
}
