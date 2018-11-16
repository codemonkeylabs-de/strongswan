/*
 * Copyright (C) 2018 Tobias Brunner
 * Copyright (C) 2016 Andreas Steffen
 * HSR Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include "test_suite.h"

#include <time.h>

typedef struct sig_test_t sig_test_t;

struct sig_test_t {
	chunk_t key;
	chunk_t pubkey;
	chunk_t msg;
	chunk_t sig;
};

/**
 * Ed448 Test Vectors from RFC 8032
 */
static sig_test_t sig_tests[] = {
	/* Blank */
	{ chunk_from_chars(
		0x30,0x47,0x02,0x01,0x00,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x04,0x3b,0x04,0x39,
		0x6c,0x82,0xa5,0x62,0xcb,0x80,0x8d,0x10,0xd6,0x32,0xbe,0x89,0xc8,0x51,0x3e,0xbf,
		0x6c,0x92,0x9f,0x34,0xdd,0xfa,0x8c,0x9f,0x63,0xc9,0x96,0x0e,0xf6,0xe3,0x48,0xa3,
		0x52,0x8c,0x8a,0x3f,0xcc,0x2f,0x04,0x4e,0x39,0xa3,0xfc,0x5b,0x94,0x49,0x2f,0x8f,
		0x03,0x2e,0x75,0x49,0xa2,0x00,0x98,0xf9,0x5b),
	  chunk_from_chars(
		0x30,0x43,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x03,0x3a,0x00,0x5f,0xd7,0x44,0x9b,
		0x59,0xb4,0x61,0xfd,0x2c,0xe7,0x87,0xec,0x61,0x6a,0xd4,0x6a,0x1d,0xa1,0x34,0x24,
		0x85,0xa7,0x0e,0x1f,0x8a,0x0e,0xa7,0x5d,0x80,0xe9,0x67,0x78,0xed,0xf1,0x24,0x76,
		0x9b,0x46,0xc7,0x06,0x1b,0xd6,0x78,0x3d,0xf1,0xe5,0x0f,0x6c,0xd1,0xfa,0x1a,0xbe,
		0xaf,0xe8,0x25,0x61,0x80),
	  { NULL, 0 },
	  chunk_from_chars(
		0x53,0x3a,0x37,0xf6,0xbb,0xe4,0x57,0x25,0x1f,0x02,0x3c,0x0d,0x88,0xf9,0x76,0xae,
		0x2d,0xfb,0x50,0x4a,0x84,0x3e,0x34,0xd2,0x07,0x4f,0xd8,0x23,0xd4,0x1a,0x59,0x1f,
		0x2b,0x23,0x3f,0x03,0x4f,0x62,0x82,0x81,0xf2,0xfd,0x7a,0x22,0xdd,0xd4,0x7d,0x78,
		0x28,0xc5,0x9b,0xd0,0xa2,0x1b,0xfd,0x39,0x80,0xff,0x0d,0x20,0x28,0xd4,0xb1,0x8a,
		0x9d,0xf6,0x3e,0x00,0x6c,0x5d,0x1c,0x2d,0x34,0x5b,0x92,0x5d,0x8d,0xc0,0x0b,0x41,
		0x04,0x85,0x2d,0xb9,0x9a,0xc5,0xc7,0xcd,0xda,0x85,0x30,0xa1,0x13,0xa0,0xf4,0xdb,
		0xb6,0x11,0x49,0xf0,0x5a,0x73,0x63,0x26,0x8c,0x71,0xd9,0x58,0x08,0xff,0x2e,0x65,
		0x26,0x00)
	},
	/* 1 octet */
	{ chunk_from_chars(
		0x30,0x47,0x02,0x01,0x00,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x04,0x3b,0x04,0x39,
		0xc4,0xea,0xb0,0x5d,0x35,0x70,0x07,0xc6,0x32,0xf3,0xdb,0xb4,0x84,0x89,0x92,0x4d,
		0x55,0x2b,0x08,0xfe,0x0c,0x35,0x3a,0x0d,0x4a,0x1f,0x00,0xac,0xda,0x2c,0x46,0x3a,
		0xfb,0xea,0x67,0xc5,0xe8,0xd2,0x87,0x7c,0x5e,0x3b,0xc3,0x97,0xa6,0x59,0x94,0x9e,
		0xf8,0x02,0x1e,0x95,0x4e,0x0a,0x12,0x27,0x4e),
	  chunk_from_chars(
		0x30,0x43,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x03,0x3a,0x00,0x43,0xba,0x28,0xf4,
		0x30,0xcd,0xff,0x45,0x6a,0xe5,0x31,0x54,0x5f,0x7e,0xcd,0x0a,0xc8,0x34,0xa5,0x5d,
		0x93,0x58,0xc0,0x37,0x2b,0xfa,0x0c,0x6c,0x67,0x98,0xc0,0x86,0x6a,0xea,0x01,0xeb,
		0x00,0x74,0x28,0x02,0xb8,0x43,0x8e,0xa4,0xcb,0x82,0x16,0x9c,0x23,0x51,0x60,0x62,
		0x7b,0x4c,0x3a,0x94,0x80),
	  chunk_from_chars(
		0x03),
	  chunk_from_chars(
		0x26,0xb8,0xf9,0x17,0x27,0xbd,0x62,0x89,0x7a,0xf1,0x5e,0x41,0xeb,0x43,0xc3,0x77,
		0xef,0xb9,0xc6,0x10,0xd4,0x8f,0x23,0x35,0xcb,0x0b,0xd0,0x08,0x78,0x10,0xf4,0x35,
		0x25,0x41,0xb1,0x43,0xc4,0xb9,0x81,0xb7,0xe1,0x8f,0x62,0xde,0x8c,0xcd,0xf6,0x33,
		0xfc,0x1b,0xf0,0x37,0xab,0x7c,0xd7,0x79,0x80,0x5e,0x0d,0xbc,0xc0,0xaa,0xe1,0xcb,
		0xce,0xe1,0xaf,0xb2,0xe0,0x27,0xdf,0x36,0xbc,0x04,0xdc,0xec,0xbf,0x15,0x43,0x36,
		0xc1,0x9f,0x0a,0xf7,0xe0,0xa6,0x47,0x29,0x05,0xe7,0x99,0xf1,0x95,0x3d,0x2a,0x0f,
		0xf3,0x34,0x8a,0xb2,0x1a,0xa4,0xad,0xaf,0xd1,0xd2,0x34,0x44,0x1c,0xf8,0x07,0xc0,
		0x3a,0x00)
	},
	/* 11 octets */
	{ chunk_from_chars(
		0x30,0x47,0x02,0x01,0x00,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x04,0x3b,0x04,0x39,
		0xcd,0x23,0xd2,0x4f,0x71,0x42,0x74,0xe7,0x44,0x34,0x32,0x37,0xb9,0x32,0x90,0xf5,
		0x11,0xf6,0x42,0x5f,0x98,0xe6,0x44,0x59,0xff,0x20,0x3e,0x89,0x85,0x08,0x3f,0xfd,
		0xf6,0x05,0x00,0x55,0x3a,0xbc,0x0e,0x05,0xcd,0x02,0x18,0x4b,0xdb,0x89,0xc4,0xcc,
		0xd6,0x7e,0x18,0x79,0x51,0x26,0x7e,0xb3,0x28),
	  chunk_from_chars(
		0x30,0x43,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x03,0x3a,0x00,0xdc,0xea,0x9e,0x78,
		0xf3,0x5a,0x1b,0xf3,0x49,0x9a,0x83,0x1b,0x10,0xb8,0x6c,0x90,0xaa,0xc0,0x1c,0xd8,
		0x4b,0x67,0xa0,0x10,0x9b,0x55,0xa3,0x6e,0x93,0x28,0xb1,0xe3,0x65,0xfc,0xe1,0x61,
		0xd7,0x1c,0xe7,0x13,0x1a,0x54,0x3e,0xa4,0xcb,0x5f,0x7e,0x9f,0x1d,0x8b,0x00,0x69,
		0x64,0x47,0x00,0x14,0x00),
	  chunk_from_chars(
		0x0c,0x3e,0x54,0x40,0x74,0xec,0x63,0xb0,0x26,0x5e,0x0c),
	  chunk_from_chars(
		0x1f,0x0a,0x88,0x88,0xce,0x25,0xe8,0xd4,0x58,0xa2,0x11,0x30,0x87,0x9b,0x84,0x0a,
		0x90,0x89,0xd9,0x99,0xaa,0xba,0x03,0x9e,0xaf,0x3e,0x3a,0xfa,0x09,0x0a,0x09,0xd3,
		0x89,0xdb,0xa8,0x2c,0x4f,0xf2,0xae,0x8a,0xc5,0xcd,0xfb,0x7c,0x55,0xe9,0x4d,0x5d,
		0x96,0x1a,0x29,0xfe,0x01,0x09,0x94,0x1e,0x00,0xb8,0xdb,0xde,0xea,0x6d,0x3b,0x05,
		0x10,0x68,0xdf,0x72,0x54,0xc0,0xcd,0xc1,0x29,0xcb,0xe6,0x2d,0xb2,0xdc,0x95,0x7d,
		0xbb,0x47,0xb5,0x1f,0xd3,0xf2,0x13,0xfb,0x86,0x98,0xf0,0x64,0x77,0x42,0x50,0xa5,
		0x02,0x89,0x61,0xc9,0xbf,0x8f,0xfd,0x97,0x3f,0xe5,0xd5,0xc2,0x06,0x49,0x2b,0x14,
		0x0e,0x00)
	},
	/* 12 octets */
	{ chunk_from_chars(
		0x30,0x47,0x02,0x01,0x00,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x04,0x3b,0x04,0x39,
		0x25,0x8c,0xdd,0x4a,0xda,0x32,0xed,0x9c,0x9f,0xf5,0x4e,0x63,0x75,0x6a,0xe5,0x82,
		0xfb,0x8f,0xab,0x2a,0xc7,0x21,0xf2,0xc8,0xe6,0x76,0xa7,0x27,0x68,0x51,0x3d,0x93,
		0x9f,0x63,0xdd,0xdb,0x55,0x60,0x91,0x33,0xf2,0x9a,0xdf,0x86,0xec,0x99,0x29,0xdc,
		0xcb,0x52,0xc1,0xc5,0xfd,0x2f,0xf7,0xe2,0x1b),
	  chunk_from_chars(
		0x30,0x43,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x03,0x3a,0x00,0x3b,0xa1,0x6d,0xa0,
		0xc6,0xf2,0xcc,0x1f,0x30,0x18,0x77,0x40,0x75,0x6f,0x5e,0x79,0x8d,0x6b,0xc5,0xfc,
		0x01,0x5d,0x7c,0x63,0xcc,0x95,0x10,0xee,0x3f,0xd4,0x4a,0xdc,0x24,0xd8,0xe9,0x68,
		0xb6,0xe4,0x6e,0x6f,0x94,0xd1,0x9b,0x94,0x53,0x61,0x72,0x6b,0xd7,0x5e,0x14,0x9e,
		0xf0,0x98,0x17,0xf5,0x80),
	  chunk_from_chars(
		0x64,0xa6,0x5f,0x3c,0xde,0xdc,0xdd,0x66,0x81,0x1e,0x29,0x15),
	  chunk_from_chars(
		0x7e,0xee,0xab,0x7c,0x4e,0x50,0xfb,0x79,0x9b,0x41,0x8e,0xe5,0xe3,0x19,0x7f,0xf6,
		0xbf,0x15,0xd4,0x3a,0x14,0xc3,0x43,0x89,0xb5,0x9d,0xd1,0xa7,0xb1,0xb8,0x5b,0x4a,
		0xe9,0x04,0x38,0xac,0xa6,0x34,0xbe,0xa4,0x5e,0x3a,0x26,0x95,0xf1,0x27,0x0f,0x07,
		0xfd,0xcd,0xf7,0xc6,0x2b,0x8e,0xfe,0xaf,0x00,0xb4,0x5c,0x2c,0x96,0xba,0x45,0x7e,
		0xb1,0xa8,0xbf,0x07,0x5a,0x3d,0xb2,0x8e,0x5c,0x24,0xf6,0xb9,0x23,0xed,0x4a,0xd7,
		0x47,0xc3,0xc9,0xe0,0x3c,0x70,0x79,0xef,0xb8,0x7c,0xb1,0x10,0xd3,0xa9,0x98,0x61,
		0xe7,0x20,0x03,0xcb,0xae,0x6d,0x6b,0x8b,0x82,0x7e,0x4e,0x6c,0x14,0x30,0x64,0xff,
		0x3c,0x00)
	},
	/* 13 octets */
	{ chunk_from_chars(
		0x30,0x47,0x02,0x01,0x00,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x04,0x3b,0x04,0x39,
		0x7e,0xf4,0xe8,0x45,0x44,0x23,0x67,0x52,0xfb,0xb5,0x6b,0x8f,0x31,0xa2,0x3a,0x10,
		0xe4,0x28,0x14,0xf5,0xf5,0x5c,0xa0,0x37,0xcd,0xcc,0x11,0xc6,0x4c,0x9a,0x3b,0x29,
		0x49,0xc1,0xbb,0x60,0x70,0x03,0x14,0x61,0x17,0x32,0xa6,0xc2,0xfe,0xa9,0x8e,0xeb,
		0xc0,0x26,0x6a,0x11,0xa9,0x39,0x70,0x10,0x0e),
	  chunk_from_chars(
		0x30,0x43,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x03,0x3a,0x00,0xb3,0xda,0x07,0x9b,
		0x0a,0xa4,0x93,0xa5,0x77,0x20,0x29,0xf0,0x46,0x7b,0xae,0xbe,0xe5,0xa8,0x11,0x2d,
		0x9d,0x3a,0x22,0x53,0x23,0x61,0xda,0x29,0x4f,0x7b,0xb3,0x81,0x5c,0x5d,0xc5,0x9e,
		0x17,0x6b,0x4d,0x9f,0x38,0x1c,0xa0,0x93,0x8e,0x13,0xc6,0xc0,0x7b,0x17,0x4b,0xe6,
		0x5d,0xfa,0x57,0x8e,0x80),
	  chunk_from_chars(
		0x64,0xa6,0x5f,0x3c,0xde,0xdc,0xdd,0x66,0x81,0x1e,0x29,0x15,0xe7),
	  chunk_from_chars(
		0x6a,0x12,0x06,0x6f,0x55,0x33,0x1b,0x6c,0x22,0xac,0xd5,0xd5,0xbf,0xc5,0xd7,0x12,
		0x28,0xfb,0xda,0x80,0xae,0x8d,0xec,0x26,0xbd,0xd3,0x06,0x74,0x3c,0x50,0x27,0xcb,
		0x48,0x90,0x81,0x0c,0x16,0x2c,0x02,0x74,0x68,0x67,0x5e,0xcf,0x64,0x5a,0x83,0x17,
		0x6c,0x0d,0x73,0x23,0xa2,0xcc,0xde,0x2d,0x80,0xef,0xe5,0xa1,0x26,0x8e,0x8a,0xca,
		0x1d,0x6f,0xbc,0x19,0x4d,0x3f,0x77,0xc4,0x49,0x86,0xeb,0x4a,0xb4,0x17,0x79,0x19,
		0xad,0x8b,0xec,0x33,0xeb,0x47,0xbb,0xb5,0xfc,0x6e,0x28,0x19,0x6f,0xd1,0xca,0xf5,
		0x6b,0x4e,0x7e,0x0b,0xa5,0x51,0x92,0x34,0xd0,0x47,0x15,0x5a,0xc7,0x27,0xa1,0x05,
		0x31,0x00)
	},
	/* 64 octets */
	{ chunk_from_chars(
		0x30,0x47,0x02,0x01,0x00,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x04,0x3b,0x04,0x39,
		0xd6,0x5d,0xf3,0x41,0xad,0x13,0xe0,0x08,0x56,0x76,0x88,0xba,0xed,0xda,0x8e,0x9d,
		0xcd,0xc1,0x7d,0xc0,0x24,0x97,0x4e,0xa5,0xb4,0x22,0x7b,0x65,0x30,0xe3,0x39,0xbf,
		0xf2,0x1f,0x99,0xe6,0x8c,0xa6,0x96,0x8f,0x3c,0xca,0x6d,0xfe,0x0f,0xb9,0xf4,0xfa,
		0xb4,0xfa,0x13,0x5d,0x55,0x42,0xea,0x3f,0x01),
	  chunk_from_chars(
		0x30,0x43,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x03,0x3a,0x00,0xdf,0x97,0x05,0xf5,
		0x8e,0xdb,0xab,0x80,0x2c,0x7f,0x83,0x63,0xcf,0xe5,0x56,0x0a,0xb1,0xc6,0x13,0x2c,
		0x20,0xa9,0xf1,0xdd,0x16,0x34,0x83,0xa2,0x6f,0x8a,0xc5,0x3a,0x39,0xd6,0x80,0x8b,
		0xf4,0xa1,0xdf,0xbd,0x26,0x1b,0x09,0x9b,0xb0,0x3b,0x3f,0xb5,0x09,0x06,0xcb,0x28,
		0xbd,0x8a,0x08,0x1f,0x00),
	  chunk_from_chars(
		0xbd,0x0f,0x6a,0x37,0x47,0xcd,0x56,0x1b,0xdd,0xdf,0x46,0x40,0xa3,0x32,0x46,0x1a,
		0x4a,0x30,0xa1,0x2a,0x43,0x4c,0xd0,0xbf,0x40,0xd7,0x66,0xd9,0xc6,0xd4,0x58,0xe5,
		0x51,0x22,0x04,0xa3,0x0c,0x17,0xd1,0xf5,0x0b,0x50,0x79,0x63,0x1f,0x64,0xeb,0x31,
		0x12,0x18,0x2d,0xa3,0x00,0x58,0x35,0x46,0x11,0x13,0x71,0x8d,0x1a,0x5e,0xf9,0x44),
	  chunk_from_chars(
		0x55,0x4b,0xc2,0x48,0x08,0x60,0xb4,0x9e,0xab,0x85,0x32,0xd2,0xa5,0x33,0xb7,0xd5,
		0x78,0xef,0x47,0x3e,0xeb,0x58,0xc9,0x8b,0xb2,0xd0,0xe1,0xce,0x48,0x8a,0x98,0xb1,
		0x8d,0xfd,0xe9,0xb9,0xb9,0x07,0x75,0xe6,0x7f,0x47,0xd4,0xa1,0xc3,0x48,0x20,0x58,
		0xef,0xc9,0xf4,0x0d,0x2c,0xa0,0x33,0xa0,0x80,0x1b,0x63,0xd4,0x5b,0x3b,0x72,0x2e,
		0xf5,0x52,0xba,0xd3,0xb4,0xcc,0xb6,0x67,0xda,0x35,0x01,0x92,0xb6,0x1c,0x50,0x8c,
		0xf7,0xb6,0xb5,0xad,0xad,0xc2,0xc8,0xd9,0xa4,0x46,0xef,0x00,0x3f,0xb0,0x5c,0xba,
		0x5f,0x30,0xe8,0x8e,0x36,0xec,0x27,0x03,0xb3,0x49,0xca,0x22,0x9c,0x26,0x70,0x83,
		0x39,0x00)
	},
	/* 256 octets */
	{ chunk_from_chars(
		0x30,0x47,0x02,0x01,0x00,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x04,0x3b,0x04,0x39,
		0x2e,0xc5,0xfe,0x3c,0x17,0x04,0x5a,0xbd,0xb1,0x36,0xa5,0xe6,0xa9,0x13,0xe3,0x2a,
		0xb7,0x5a,0xe6,0x8b,0x53,0xd2,0xfc,0x14,0x9b,0x77,0xe5,0x04,0x13,0x2d,0x37,0x56,
		0x9b,0x7e,0x76,0x6b,0xa7,0x4a,0x19,0xbd,0x61,0x62,0x34,0x3a,0x21,0xc8,0x59,0x0a,
		0xa9,0xce,0xbc,0xa9,0x01,0x4c,0x63,0x6d,0xf5),
	  chunk_from_chars(
		0x30,0x43,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x03,0x3a,0x00,0x79,0x75,0x6f,0x01,
		0x4d,0xcf,0xe2,0x07,0x9f,0x5d,0xd9,0xe7,0x18,0xbe,0x41,0x71,0xe2,0xef,0x24,0x86,
		0xa0,0x8f,0x25,0x18,0x6f,0x6b,0xff,0x43,0xa9,0x93,0x6b,0x9b,0xfe,0x12,0x40,0x2b,
		0x08,0xae,0x65,0x79,0x8a,0x3d,0x81,0xe2,0x2e,0x9e,0xc8,0x0e,0x76,0x90,0x86,0x2e,
		0xf3,0xd4,0xed,0x3a,0x00),
	  chunk_from_chars(
		0x15,0x77,0x75,0x32,0xb0,0xbd,0xd0,0xd1,0x38,0x9f,0x63,0x6c,0x5f,0x6b,0x9b,0xa7,
		0x34,0xc9,0x0a,0xf5,0x72,0x87,0x7e,0x2d,0x27,0x2d,0xd0,0x78,0xaa,0x1e,0x56,0x7c,
		0xfa,0x80,0xe1,0x29,0x28,0xbb,0x54,0x23,0x30,0xe8,0x40,0x9f,0x31,0x74,0x50,0x41,
		0x07,0xec,0xd5,0xef,0xac,0x61,0xae,0x75,0x04,0xda,0xbe,0x2a,0x60,0x2e,0xde,0x89,
		0xe5,0xcc,0xa6,0x25,0x7a,0x7c,0x77,0xe2,0x7a,0x70,0x2b,0x3a,0xe3,0x9f,0xc7,0x69,
		0xfc,0x54,0xf2,0x39,0x5a,0xe6,0xa1,0x17,0x8c,0xab,0x47,0x38,0xe5,0x43,0x07,0x2f,
		0xc1,0xc1,0x77,0xfe,0x71,0xe9,0x2e,0x25,0xbf,0x03,0xe4,0xec,0xb7,0x2f,0x47,0xb6,
		0x4d,0x04,0x65,0xaa,0xea,0x4c,0x7f,0xad,0x37,0x25,0x36,0xc8,0xba,0x51,0x6a,0x60,
		0x39,0xc3,0xc2,0xa3,0x9f,0x0e,0x4d,0x83,0x2b,0xe4,0x32,0xdf,0xa9,0xa7,0x06,0xa6,
		0xe5,0xc7,0xe1,0x9f,0x39,0x79,0x64,0xca,0x42,0x58,0x00,0x2f,0x7c,0x05,0x41,0xb5,
		0x90,0x31,0x6d,0xbc,0x56,0x22,0xb6,0xb2,0xa6,0xfe,0x7a,0x4a,0xbf,0xfd,0x96,0x10,
		0x5e,0xca,0x76,0xea,0x7b,0x98,0x81,0x6a,0xf0,0x74,0x8c,0x10,0xdf,0x04,0x8c,0xe0,
		0x12,0xd9,0x01,0x01,0x5a,0x51,0xf1,0x89,0xf3,0x88,0x81,0x45,0xc0,0x36,0x50,0xaa,
		0x23,0xce,0x89,0x4c,0x3b,0xd8,0x89,0xe0,0x30,0xd5,0x65,0x07,0x1c,0x59,0xf4,0x09,
		0xa9,0x98,0x1b,0x51,0x87,0x8f,0xd6,0xfc,0x11,0x06,0x24,0xdc,0xbc,0xde,0x0b,0xf7,
		0xa6,0x9c,0xcc,0xe3,0x8f,0xab,0xdf,0x86,0xf3,0xbe,0xf6,0x04,0x48,0x19,0xde,0x11),
	  chunk_from_chars(
		0xc6,0x50,0xdd,0xbb,0x06,0x01,0xc1,0x9c,0xa1,0x14,0x39,0xe1,0x64,0x0d,0xd9,0x31,
		0xf4,0x3c,0x51,0x8e,0xa5,0xbe,0xa7,0x0d,0x3d,0xcd,0xe5,0xf4,0x19,0x1f,0xe5,0x3f,
		0x00,0xcf,0x96,0x65,0x46,0xb7,0x2b,0xcc,0x7d,0x58,0xbe,0x2b,0x9b,0xad,0xef,0x28,
		0x74,0x39,0x54,0xe3,0xa4,0x4a,0x23,0xf8,0x80,0xe8,0xd4,0xf1,0xcf,0xce,0x2d,0x7a,
		0x61,0x45,0x2d,0x26,0xda,0x05,0x89,0x6f,0x0a,0x50,0xda,0x66,0xa2,0x39,0xa8,0xa1,
		0x88,0xb6,0xd8,0x25,0xb3,0x30,0x5a,0xd7,0x7b,0x73,0xfb,0xac,0x08,0x36,0xec,0xc6,
		0x09,0x87,0xfd,0x08,0x52,0x7c,0x1a,0x8e,0x80,0xd5,0x82,0x3e,0x65,0xca,0xfe,0x2a,
		0x3d,0x00)
	},
	/* 1023 octets */
	{ chunk_from_chars(
		0x30,0x47,0x02,0x01,0x00,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x04,0x3b,0x04,0x39,
		0x87,0x2d,0x09,0x37,0x80,0xf5,0xd3,0x73,0x0d,0xf7,0xc2,0x12,0x66,0x4b,0x37,0xb8,
		0xa0,0xf2,0x4f,0x56,0x81,0x0d,0xaa,0x83,0x82,0xcd,0x4f,0xa3,0xf7,0x76,0x34,0xec,
		0x44,0xdc,0x54,0xf1,0xc2,0xed,0x9b,0xea,0x86,0xfa,0xfb,0x76,0x32,0xd8,0xbe,0x19,
		0x9e,0xa1,0x65,0xf5,0xad,0x55,0xdd,0x9c,0xe8),
	  chunk_from_chars(
		0x30,0x43,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x03,0x3a,0x00,0xa8,0x1b,0x2e,0x8a,
		0x70,0xa5,0xac,0x94,0xff,0xdb,0xcc,0x9b,0xad,0xfc,0x3f,0xeb,0x08,0x01,0xf2,0x58,
		0x57,0x8b,0xb1,0x14,0xad,0x44,0xec,0xe1,0xec,0x0e,0x79,0x9d,0xa0,0x8e,0xff,0xb8,
		0x1c,0x5d,0x68,0x5c,0x0c,0x56,0xf6,0x4e,0xec,0xae,0xf8,0xcd,0xf1,0x1c,0xc3,0x87,
		0x37,0x83,0x8c,0xf4,0x00),
	  chunk_from_chars(
		0x6d,0xdf,0x80,0x2e,0x1a,0xae,0x49,0x86,0x93,0x5f,0x7f,0x98,0x1b,0xa3,0xf0,0x35,
		0x1d,0x62,0x73,0xc0,0xa0,0xc2,0x2c,0x9c,0x0e,0x83,0x39,0x16,0x8e,0x67,0x54,0x12,
		0xa3,0xde,0xbf,0xaf,0x43,0x5e,0xd6,0x51,0x55,0x80,0x07,0xdb,0x43,0x84,0xb6,0x50,
		0xfc,0xc0,0x7e,0x3b,0x58,0x6a,0x27,0xa4,0xf7,0xa0,0x0a,0xc8,0xa6,0xfe,0xc2,0xcd,
		0x86,0xae,0x4b,0xf1,0x57,0x0c,0x41,0xe6,0xa4,0x0c,0x93,0x1d,0xb2,0x7b,0x2f,0xaa,
		0x15,0xa8,0xce,0xdd,0x52,0xcf,0xf7,0x36,0x2c,0x4e,0x6e,0x23,0xda,0xec,0x0f,0xbc,
		0x3a,0x79,0xb6,0x80,0x6e,0x31,0x6e,0xfc,0xc7,0xb6,0x81,0x19,0xbf,0x46,0xbc,0x76,
		0xa2,0x60,0x67,0xa5,0x3f,0x29,0x6d,0xaf,0xdb,0xdc,0x11,0xc7,0x7f,0x77,0x77,0xe9,
		0x72,0x66,0x0c,0xf4,0xb6,0xa9,0xb3,0x69,0xa6,0x66,0x5f,0x02,0xe0,0xcc,0x9b,0x6e,
		0xdf,0xad,0x13,0x6b,0x4f,0xab,0xe7,0x23,0xd2,0x81,0x3d,0xb3,0x13,0x6c,0xfd,0xe9,
		0xb6,0xd0,0x44,0x32,0x2f,0xee,0x29,0x47,0x95,0x2e,0x03,0x1b,0x73,0xab,0x5c,0x60,
		0x33,0x49,0xb3,0x07,0xbd,0xc2,0x7b,0xc6,0xcb,0x8b,0x8b,0xbd,0x7b,0xd3,0x23,0x21,
		0x9b,0x80,0x33,0xa5,0x81,0xb5,0x9e,0xad,0xeb,0xb0,0x9b,0x3c,0x4f,0x3d,0x22,0x77,
		0xd4,0xf0,0x34,0x36,0x24,0xac,0xc8,0x17,0x80,0x47,0x28,0xb2,0x5a,0xb7,0x97,0x17,
		0x2b,0x4c,0x5c,0x21,0xa2,0x2f,0x9c,0x78,0x39,0xd6,0x43,0x00,0x23,0x2e,0xb6,0x6e,
		0x53,0xf3,0x1c,0x72,0x3f,0xa3,0x7f,0xe3,0x87,0xc7,0xd3,0xe5,0x0b,0xdf,0x98,0x13,
		0xa3,0x0e,0x5b,0xb1,0x2c,0xf4,0xcd,0x93,0x0c,0x40,0xcf,0xb4,0xe1,0xfc,0x62,0x25,
		0x92,0xa4,0x95,0x88,0x79,0x44,0x94,0xd5,0x6d,0x24,0xea,0x4b,0x40,0xc8,0x9f,0xc0,
		0x59,0x6c,0xc9,0xeb,0xb9,0x61,0xc8,0xcb,0x10,0xad,0xde,0x97,0x6a,0x5d,0x60,0x2b,
		0x1c,0x3f,0x85,0xb9,0xb9,0xa0,0x01,0xed,0x3c,0x6a,0x4d,0x3b,0x14,0x37,0xf5,0x20,
		0x96,0xcd,0x19,0x56,0xd0,0x42,0xa5,0x97,0xd5,0x61,0xa5,0x96,0xec,0xd3,0xd1,0x73,
		0x5a,0x8d,0x57,0x0e,0xa0,0xec,0x27,0x22,0x5a,0x2c,0x4a,0xaf,0xf2,0x63,0x06,0xd1,
		0x52,0x6c,0x1a,0xf3,0xca,0x6d,0x9c,0xf5,0xa2,0xc9,0x8f,0x47,0xe1,0xc4,0x6d,0xb9,
		0xa3,0x32,0x34,0xcf,0xd4,0xd8,0x1f,0x2c,0x98,0x53,0x8a,0x09,0xeb,0xe7,0x69,0x98,
		0xd0,0xd8,0xfd,0x25,0x99,0x7c,0x7d,0x25,0x5c,0x6d,0x66,0xec,0xe6,0xfa,0x56,0xf1,
		0x11,0x44,0x95,0x0f,0x02,0x77,0x95,0xe6,0x53,0x00,0x8f,0x4b,0xd7,0xca,0x2d,0xee,
		0x85,0xd8,0xe9,0x0f,0x3d,0xc3,0x15,0x13,0x0c,0xe2,0xa0,0x03,0x75,0xa3,0x18,0xc7,
		0xc3,0xd9,0x7b,0xe2,0xc8,0xce,0x5b,0x6d,0xb4,0x1a,0x62,0x54,0xff,0x26,0x4f,0xa6,
		0x15,0x5b,0xae,0xe3,0xb0,0x77,0x3c,0x0f,0x49,0x7c,0x57,0x3f,0x19,0xbb,0x4f,0x42,
		0x40,0x28,0x1f,0x0b,0x1f,0x4f,0x7b,0xe8,0x57,0xa4,0xe5,0x9d,0x41,0x6c,0x06,0xb4,
		0xc5,0x0f,0xa0,0x9e,0x18,0x10,0xdd,0xc6,0xb1,0x46,0x7b,0xae,0xac,0x5a,0x36,0x68,
		0xd1,0x1b,0x6e,0xca,0xa9,0x01,0x44,0x00,0x16,0xf3,0x89,0xf8,0x0a,0xcc,0x4d,0xb9,
		0x77,0x02,0x5e,0x7f,0x59,0x24,0x38,0x8c,0x7e,0x34,0x0a,0x73,0x2e,0x55,0x44,0x40,
		0xe7,0x65,0x70,0xf8,0xdd,0x71,0xb7,0xd6,0x40,0xb3,0x45,0x0d,0x1f,0xd5,0xf0,0x41,
		0x0a,0x18,0xf9,0xa3,0x49,0x4f,0x70,0x7c,0x71,0x7b,0x79,0xb4,0xbf,0x75,0xc9,0x84,
		0x00,0xb0,0x96,0xb2,0x16,0x53,0xb5,0xd2,0x17,0xcf,0x35,0x65,0xc9,0x59,0x74,0x56,
		0xf7,0x07,0x03,0x49,0x7a,0x07,0x87,0x63,0x82,0x9b,0xc0,0x1b,0xb1,0xcb,0xc8,0xfa,
		0x04,0xea,0xdc,0x9a,0x6e,0x3f,0x66,0x99,0x58,0x7a,0x9e,0x75,0xc9,0x4e,0x5b,0xab,
		0x00,0x36,0xe0,0xb2,0xe7,0x11,0x39,0x2c,0xff,0x00,0x47,0xd0,0xd6,0xb0,0x5b,0xd2,
		0xa5,0x88,0xbc,0x10,0x97,0x18,0x95,0x42,0x59,0xf1,0xd8,0x66,0x78,0xa5,0x79,0xa3,
		0x12,0x0f,0x19,0xcf,0xb2,0x96,0x3f,0x17,0x7a,0xeb,0x70,0xf2,0xd4,0x84,0x48,0x26,
		0x26,0x2e,0x51,0xb8,0x02,0x71,0x27,0x20,0x68,0xef,0x5b,0x38,0x56,0xfa,0x85,0x35,
		0xaa,0x2a,0x88,0xb2,0xd4,0x1f,0x2a,0x0e,0x2f,0xda,0x76,0x24,0xc2,0x85,0x02,0x72,
		0xac,0x4a,0x2f,0x56,0x1f,0x8f,0x2f,0x7a,0x31,0x8b,0xfd,0x5c,0xaf,0x96,0x96,0x14,
		0x9e,0x4a,0xc8,0x24,0xad,0x34,0x60,0x53,0x8f,0xdc,0x25,0x42,0x1b,0xee,0xc2,0xcc,
		0x68,0x18,0x16,0x2d,0x06,0xbb,0xed,0x0c,0x40,0xa3,0x87,0x19,0x23,0x49,0xdb,0x67,
		0xa1,0x18,0xba,0xda,0x6c,0xd5,0xab,0x01,0x40,0xee,0x27,0x32,0x04,0xf6,0x28,0xaa,
		0xd1,0xc1,0x35,0xf7,0x70,0x27,0x9a,0x65,0x1e,0x24,0xd8,0xc1,0x4d,0x75,0xa6,0x05,
		0x9d,0x76,0xb9,0x6a,0x6f,0xd8,0x57,0xde,0xf5,0xe0,0xb3,0x54,0xb2,0x7a,0xb9,0x37,
		0xa5,0x81,0x5d,0x16,0xb5,0xfa,0xe4,0x07,0xff,0x18,0x22,0x2c,0x6d,0x1e,0xd2,0x63,
		0xbe,0x68,0xc9,0x5f,0x32,0xd9,0x08,0xbd,0x89,0x5c,0xd7,0x62,0x07,0xae,0x72,0x64,
		0x87,0x56,0x7f,0x9a,0x67,0xda,0xd7,0x9a,0xbe,0xc3,0x16,0xf6,0x83,0xb1,0x7f,0x2d,
		0x02,0xbf,0x07,0xe0,0xac,0x8b,0x5b,0xc6,0x16,0x2c,0xf9,0x46,0x97,0xb3,0xc2,0x7c,
		0xd1,0xfe,0xa4,0x9b,0x27,0xf2,0x3b,0xa2,0x90,0x18,0x71,0x96,0x25,0x06,0x52,0x0c,
		0x39,0x2d,0xa8,0xb6,0xad,0x0d,0x99,0xf7,0x01,0x3f,0xbc,0x06,0xc2,0xc1,0x7a,0x56,
		0x95,0x00,0xc8,0xa7,0x69,0x64,0x81,0xc1,0xcd,0x33,0xe9,0xb1,0x4e,0x40,0xb8,0x2e,
		0x79,0xa5,0xf5,0xdb,0x82,0x57,0x1b,0xa9,0x7b,0xae,0x3a,0xd3,0xe0,0x47,0x95,0x15,
		0xbb,0x0e,0x2b,0x0f,0x3b,0xfc,0xd1,0xfd,0x33,0x03,0x4e,0xfc,0x62,0x45,0xed,0xdd,
		0x7e,0xe2,0x08,0x6d,0xda,0xe2,0x60,0x0d,0x8c,0xa7,0x3e,0x21,0x4e,0x8c,0x2b,0x0b,
		0xdb,0x2b,0x04,0x7c,0x6a,0x46,0x4a,0x56,0x2e,0xd7,0x7b,0x73,0xd2,0xd8,0x41,0xc4,
		0xb3,0x49,0x73,0x55,0x12,0x57,0x71,0x3b,0x75,0x36,0x32,0xef,0xba,0x34,0x81,0x69,
		0xab,0xc9,0x0a,0x68,0xf4,0x26,0x11,0xa4,0x01,0x26,0xd7,0xcb,0x21,0xb5,0x86,0x95,
		0x56,0x81,0x86,0xf7,0xe5,0x69,0xd2,0xff,0x0f,0x9e,0x74,0x5d,0x04,0x87,0xdd,0x2e,
		0xb9,0x97,0xca,0xfc,0x5a,0xbf,0x9d,0xd1,0x02,0xe6,0x2f,0xf6,0x6c,0xba,0x87),
	  chunk_from_chars(
		0xe3,0x01,0x34,0x5a,0x41,0xa3,0x9a,0x4d,0x72,0xff,0xf8,0xdf,0x69,0xc9,0x80,0x75,
		0xa0,0xcc,0x08,0x2b,0x80,0x2f,0xc9,0xb2,0xb6,0xbc,0x50,0x3f,0x92,0x6b,0x65,0xbd,
		0xdf,0x7f,0x4c,0x8f,0x1c,0xb4,0x9f,0x63,0x96,0xaf,0xc8,0xa7,0x0a,0xbe,0x6d,0x8a,
		0xef,0x0d,0xb4,0x78,0xd4,0xc6,0xb2,0x97,0x00,0x76,0xc6,0xa0,0x48,0x4f,0xe7,0x6d,
		0x76,0xb3,0xa9,0x76,0x25,0xd7,0x9f,0x1c,0xe2,0x40,0xe7,0xc5,0x76,0x75,0x0d,0x29,
		0x55,0x28,0x28,0x6f,0x71,0x9b,0x41,0x3d,0xe9,0xad,0xa3,0xe8,0xeb,0x78,0xed,0x57,
		0x36,0x03,0xce,0x30,0xd8,0xbb,0x76,0x17,0x85,0xdc,0x30,0xdb,0xc3,0x20,0x86,0x9e,
		0x1a,0x00)
	},
};

START_TEST(test_ed448_sign)
{
	private_key_t *key;
	public_key_t *pubkey, *public;
	chunk_t sig, encoding;

	/* load private key */
	key = lib->creds->create(lib->creds, CRED_PRIVATE_KEY, KEY_ED448,
							 BUILD_BLOB_ASN1_DER, sig_tests[_i].key, BUILD_END);
	ck_assert(key != NULL);
	ck_assert(key->get_encoding(key, PRIVKEY_ASN1_DER, &encoding));
	ck_assert_chunk_eq(encoding, sig_tests[_i].key);
	chunk_free(&encoding);

	/* load public key */
	pubkey = lib->creds->create(lib->creds, CRED_PUBLIC_KEY, KEY_ED448,
								BUILD_BLOB_ASN1_DER, sig_tests[_i].pubkey, BUILD_END);
	ck_assert(pubkey != NULL);
	ck_assert(pubkey->get_encoding(pubkey, PUBKEY_SPKI_ASN1_DER, &encoding));
	ck_assert_chunk_eq(encoding, sig_tests[_i].pubkey);
	chunk_free(&encoding);

	/* compare public keys */
	public = key->get_public_key(key);
	ck_assert(public != NULL);
	ck_assert(public->equals(public, pubkey));

	/* sign */
	ck_assert(key->sign(key, SIGN_ED448, NULL, sig_tests[_i].msg, &sig));
	ck_assert_chunk_eq(sig, sig_tests[_i].sig);

	/* verify */
	ck_assert(pubkey->verify(pubkey, SIGN_ED448, NULL, sig_tests[_i].msg,
							 sig_tests[_i].sig));

	/* cleanup */
	key->destroy(key);
	pubkey->destroy(pubkey);
	public->destroy(public);
	chunk_free(&sig);
}
END_TEST

START_TEST(test_ed448_gen)
{
	private_key_t *key, *key2;
	public_key_t *pubkey, *pubkey2;
	chunk_t msg = chunk_from_str("Ed448"), sig, encoding, fp_priv, fp_pub;

	/* generate private key */
	key = lib->creds->create(lib->creds, CRED_PRIVATE_KEY, KEY_ED448,
							 BUILD_KEY_SIZE, 456, BUILD_END);
	ck_assert(key != NULL);
	ck_assert(key->get_type(key) == KEY_ED448);
	ck_assert(key->get_keysize(key) == 456);
	ck_assert(!key->get_encoding(key, PRIVKEY_PGP, &encoding));
	ck_assert(key->get_encoding(key, PRIVKEY_PEM, &encoding));
	ck_assert(encoding.ptr != NULL);
	ck_assert(strstr(encoding.ptr, "PRIVATE KEY"));
	chunk_free(&encoding);

	/* clone private key */
	key2 = key->get_ref(key);
	ck_assert(key2);
	key2->destroy(key2);

	/* decryption not supported */
	ck_assert(!key->decrypt(key, ENCRYPT_UNKNOWN, msg, NULL));

	/* wrong signature scheme */
	ck_assert(!key->sign(key, SIGN_ED25519, NULL, msg, &sig));

	/* correct signature scheme*/
	ck_assert(key->sign(key, SIGN_ED448, NULL, msg, &sig));

	/* export public key */
	pubkey = key->get_public_key(key);
	ck_assert(pubkey != NULL);
	ck_assert(pubkey->get_type(pubkey) == KEY_ED448);
	ck_assert(pubkey->get_keysize(pubkey) == 456);
	ck_assert(pubkey->get_encoding(pubkey, PUBKEY_PEM, &encoding));
	ck_assert(encoding.ptr != NULL);
	ck_assert(strstr(encoding.ptr, "PUBLIC KEY"));
	chunk_free(&encoding);

	/* generate and compare public and private key fingerprints */
	ck_assert(!key->get_fingerprint(key, KEYID_PGPV4, &fp_priv));
	ck_assert(key->get_fingerprint(key, KEYID_PUBKEY_SHA1, &fp_priv));
	ck_assert(key->get_fingerprint(key, KEYID_PUBKEY_SHA1, &fp_priv));
	ck_assert(fp_priv.ptr != NULL);
	ck_assert(!pubkey->get_fingerprint(pubkey, KEYID_PGPV4, &fp_pub));
	ck_assert(pubkey->get_fingerprint(pubkey, KEYID_PUBKEY_SHA1, &fp_pub));
	ck_assert(pubkey->get_fingerprint(pubkey, KEYID_PUBKEY_SHA1, &fp_pub));
	ck_assert(fp_pub.ptr != NULL);
	ck_assert_chunk_eq(fp_pub, fp_priv);

	/* clone public key */
	pubkey2 = pubkey->get_ref(pubkey);
	ck_assert(pubkey2 != NULL);
	pubkey2->destroy(pubkey2);

	/* encryption not supported */
	ck_assert(!pubkey->encrypt(pubkey, ENCRYPT_UNKNOWN, msg, NULL));

	/* verify with wrong signature scheme */
	ck_assert(!pubkey->verify(pubkey, SIGN_ED25519, NULL, msg, sig));

	/* verify with correct signature scheme */
	ck_assert(pubkey->verify(pubkey, SIGN_ED448, NULL, msg, sig));

	/* cleanup */
	key->destroy(key);
	pubkey->destroy(pubkey);
	chunk_free(&sig);
}
END_TEST

START_TEST(test_ed448_speed)
{
	private_key_t *key;
	public_key_t *pubkey;
	chunk_t msg = chunk_from_str("Hello Ed448"), sig;
	int i, count = 500;

#ifdef HAVE_CLOCK_GETTIME
	struct timespec start, stop;
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
#endif

	for (i = 0; i < count; i++)
	{
		key = lib->creds->create(lib->creds, CRED_PRIVATE_KEY, KEY_ED448,
								 BUILD_KEY_SIZE, 456, BUILD_END);
		ck_assert(key != NULL);
		ck_assert(key->sign(key, SIGN_ED448, NULL, msg, &sig));
		pubkey = key->get_public_key(key);
		ck_assert(pubkey != NULL);
		ck_assert(pubkey->verify(pubkey, SIGN_ED448, NULL, msg, sig));
		key->destroy(key);
		pubkey->destroy(pubkey);
		chunk_free(&sig);
	}

#ifdef HAVE_CLOCK_GETTIME
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop);
	DBG0(DBG_LIB, "%d Ed448 keys and signatures in %d ms\n", count,
				  (stop.tv_nsec - start.tv_nsec) / 1000000 +
				  (stop.tv_sec - start.tv_sec) * 1000);
#endif
}
END_TEST

static chunk_t zero_pk = chunk_from_chars(
	0x30,0x43,0x30,0x05,0x06,0x03,0x2b,0x65,0x71,0x03,0x3a,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00);

/* sig_tests[0].sig with s+L, note that only the 9 most significant bits are 0 */
static chunk_t malleable_sig = chunk_from_chars(
	0x53,0x3a,0x37,0xf6,0xbb,0xe4,0x57,0x25,0x1f,0x02,0x3c,0x0d,0x88,0xf9,0x76,0xae,
	0x2d,0xfb,0x50,0x4a,0x84,0x3e,0x34,0xd2,0x07,0x4f,0xd8,0x23,0xd4,0x1a,0x59,0x1f,
	0x2b,0x23,0x3f,0x03,0x4f,0x62,0x82,0x81,0xf2,0xfd,0x7a,0x22,0xdd,0xd4,0x7d,0x78,
	0x28,0xc5,0x9b,0xd0,0xa2,0x1b,0xfd,0x39,0x80,0xf2,0x52,0x78,0xd3,0x66,0x74,0x03,
	0xc1,0x4b,0xce,0xc5,0xf9,0xcf,0xde,0x99,0x55,0xeb,0xc8,0x33,0x3c,0x0a,0xe7,0x8f,
	0xc8,0x6e,0x51,0x83,0x17,0xc5,0xc7,0xcd,0xda,0x85,0x30,0xa1,0x13,0xa0,0xf4,0xdb,
	0xb6,0x11,0x49,0xf0,0x5a,0x73,0x63,0x26,0x8c,0x71,0xd9,0x58,0x08,0xff,0x2e,0x65,
	0x66,0x00);

START_TEST(test_ed448_fail)
{
	private_key_t *key;
	public_key_t *pubkey;
	chunk_t blob, sig;
	uint8_t sig1[114];

	/* Invalid private key format */
	key = lib->creds->create(lib->creds, CRED_PRIVATE_KEY, KEY_ED448,
					BUILD_BLOB_ASN1_DER, chunk_empty, BUILD_END);
	ck_assert(key == NULL);

	key = lib->creds->create(lib->creds, CRED_PRIVATE_KEY, KEY_ED448,
					BUILD_EDDSA_PRIV_ASN1_DER, chunk_empty, BUILD_END);
	ck_assert(key == NULL);

	blob = chunk_from_chars(0x04, 0x01, 0x9d);
	key = lib->creds->create(lib->creds, CRED_PRIVATE_KEY, KEY_ED448,
					BUILD_EDDSA_PRIV_ASN1_DER, blob, BUILD_END);
	ck_assert(key == NULL);

	/* Invalid public key format */
	pubkey = lib->creds->create(lib->creds, CRED_PUBLIC_KEY, KEY_ED448,
					BUILD_BLOB_ASN1_DER, chunk_empty, BUILD_END);
	ck_assert(pubkey == NULL);

	blob = chunk_from_chars(0x30, 0x0b, 0x30, 0x05, 0x06, 0x03, 0x2b, 0x65,
							0x71, 0x03, 0x02, 0x00, 0xd7);
	pubkey = lib->creds->create(lib->creds, CRED_PUBLIC_KEY, KEY_ED448,
					BUILD_BLOB_ASN1_DER, blob, BUILD_END);
	ck_assert(pubkey == NULL);

	blob = chunk_from_chars(0x30, 0x0b, 0x30, 0x05, 0x06, 0x03, 0x2b, 0x00,
							0x71, 0x03, 0x02, 0x00, 0xd7);
	pubkey = lib->creds->create(lib->creds, CRED_PUBLIC_KEY, KEY_ED448,
					BUILD_BLOB_ASN1_DER, blob, BUILD_END);
	ck_assert(pubkey == NULL);

	pubkey = lib->creds->create(lib->creds, CRED_PUBLIC_KEY, KEY_ED448,
					BUILD_KEY_SIZE, 456, BUILD_BLOB_ASN1_DER, blob, BUILD_END);
	ck_assert(pubkey == NULL);

	/* Invalid signature format */
	pubkey = lib->creds->create(lib->creds, CRED_PUBLIC_KEY, KEY_ED448,
					BUILD_BLOB_ASN1_DER, sig_tests[0].pubkey, BUILD_END);
	ck_assert(pubkey != NULL);

	ck_assert(!pubkey->verify(pubkey, SIGN_ED448, NULL, chunk_empty,
							  chunk_empty));

	/* RFC 8032, section 5.2.7 requires that 0 <= s < L to prevent signature
	 * malleability.  Only a warning because OpenSSL is vulnerable to this. */
	if (pubkey->verify(pubkey, SIGN_ED448, NULL, sig_tests[0].msg,
					   malleable_sig))
	{
		warn("Ed448 signature verification is vulnerable to malleable "
			 "signatures");
	}

	/* malformed signature */
	sig = chunk_from_thing(sig1);
	memcpy(sig1, sig_tests[0].sig.ptr, sig_tests[0].sig.len);
	sig1[113] |= 0xFF;
	ck_assert(!pubkey->verify(pubkey, SIGN_ED448, NULL, sig_tests[0].msg,
							  sig));

	/* wrong signature */
	memcpy(sig1, sig_tests[0].sig.ptr, sig_tests[0].sig.len);
	sig1[0] = 0xe4;
	ck_assert(!pubkey->verify(pubkey, SIGN_ED448, NULL, sig_tests[0].msg,
							  sig));

	/* detect all-zeroes public key */
	pubkey->destroy(pubkey);
	pubkey = lib->creds->create(lib->creds, CRED_PUBLIC_KEY, KEY_ED448,
					BUILD_BLOB_ASN1_DER, zero_pk, BUILD_END);
	ck_assert(pubkey != NULL);
	ck_assert(!pubkey->verify(pubkey, SIGN_ED448, NULL, sig_tests[0].msg,
							  sig));
	pubkey->destroy(pubkey);
}
END_TEST

Suite *ed448_suite_create()
{
	Suite *s;
	TCase *tc;

	s = suite_create("ed448");

	tc = tcase_create("ed448_sign");
	tcase_add_loop_test(tc, test_ed448_sign, 0, countof(sig_tests));
	suite_add_tcase(s, tc);

	tc = tcase_create("ed448_gen");
	tcase_add_test(tc, test_ed448_gen);
	suite_add_tcase(s, tc);

	tc = tcase_create("ed448_fail");
	tcase_add_test(tc, test_ed448_fail);
	suite_add_tcase(s, tc);

	tc = tcase_create("ed448_speed");
	test_case_set_timeout(tc, 10);
	tcase_add_test(tc, test_ed448_speed);
	suite_add_tcase(s, tc);

	return s;
}
