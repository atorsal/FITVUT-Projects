/*
 * crc_impl.c
 *
 *  Created on: 5.11.2017
 *      Author: Dávid Bolvanský
 */

#include "fsl_crc.h"
#include "crc_impl.h"

uint32_t crc_basic_compute_crc32(char *buf, size_t len) {
	uint32_t crc;

	crc = CRC32_SEED;
	while (len--) {
		crc = crc ^ *buf++;
		for (int i = 0; i < 8; ++i) {
			crc = (crc >> 1) ^ (CRC32_POLY_REVERSED & -(crc & 1));
		}
	}
	return ~crc;
}
uint16_t crc_basic_compute_crc16(char *buf, size_t len) {
	uint16_t crc;

	crc = CRC16_SEED;
	while (len--) {
		crc = crc ^ *buf++;
		for (int i = 0; i < 8; ++i) {
			crc = (crc >> 1) ^ (CRC16_POLY_REVERSED & -(crc & 1));
		}
	}
	return crc;
}

uint32_t crc_table_compute_crc32(char *buf, size_t len) {
	uint32_t crc = CRC32_SEED;

	while (len--) {
		crc = crc32_table[(crc ^ *buf++) & 0xFFL] ^ (crc >> 8);
	}

	return ~crc;
}

uint16_t crc_table_compute_crc16(char *buf, size_t len) {
	uint16_t crc = CRC16_SEED;

	while (len--) {
		crc = crc16_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
	}

	return crc;
}

uint32_t crc_module_compute_crc32(char * data, size_t len) {
	crc_config_t config;
	CRC_Type *base = CRC0;

	config.polynomial = CRC32_POLY;
	config.seed = CRC32_SEED;
	config.crcBits = kCrcBits32;
	config.reflectIn = true;
	config.reflectOut = true;
	config.complementChecksum = true;
	config.crcResult = kCrcFinalChecksum;

	CRC_Init(base, &config);

	CRC_WriteData(base, (uint8_t*) data, len);
	return CRC_Get32bitResult(base);
}

uint16_t crc_module_compute_crc16(char * data, size_t len) {
	crc_config_t config;
	CRC_Type *base = CRC0;

	config.polynomial = CRC16_POLY;
	config.seed = CRC16_SEED;
	config.reflectIn = true;
	config.reflectOut = true;
	config.complementChecksum = false;
	config.crcBits = kCrcBits16;
	config.crcResult = kCrcFinalChecksum;

	CRC_Init(base, &config);

	CRC_WriteData(base, (uint8_t*) data, len);
	return CRC_Get16bitResult(base);
}

