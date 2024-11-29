/**
*******************************************************************************
* @file           : file_mgr.c
* @brief          : Description of c file
* @author         : Gonzalo Rivera
* @date           : 08/06/2023
*******************************************************************************
* @attention
*
* Copyright (c) <date> Gonzalo Rivera. All rights reserved.
*
*/
/******************************************************************************
    Includes
******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "file_binary_pack.h"
/******************************************************************************
    Defines and constants
******************************************************************************/
#define DATA_PACKET_SIZE    (MAX_CHUNK_BYTES_SIZE + 2 + 5)
/******************************************************************************
    Data types
******************************************************************************/
typedef struct
{
    const char *path;
    int chunck_section;
    unsigned long offset;
    unsigned long size_f_bytes;
    uint32_t num_of_packets;
    uint32_t packet;
}fmgr_t;

/******************************************************************************
    Local variables
******************************************************************************/
static uint8_t data_packet[DATA_PACKET_SIZE] = {0};
static fmgr_t binary_file = {0};
/******************************************************************************
    Local function prototypes
******************************************************************************/
/**
 * @brief Function to calculate CRC with poly "0xA001"
 * 
 * @param uint8_t *: buff.
 * 
 * @param uint16_t size: buffer size.
 * 
 * @return uint16_t: crc 16.
 */
static uint16_t crc_16_calculator(uint8_t *buffer, uint16_t size);
/******************************************************************************
    Local function definitions
******************************************************************************/
static uint16_t crc_16_calculator(uint8_t *buffer, uint16_t size)
{
    uint16_t crc_output = 0xFFFF;
    uint16_t i = 0;
    uint16_t j = 0;

    for(i = 0; i < size; i++)
    {
        crc_output ^= (uint16_t) buffer[i];
        for(j = 0; j < 8; ++j)
        {
            if(0 != (crc_output & 0x0001))
            	crc_output = (crc_output >> 1) ^ 0xA001;
            else
            	crc_output = crc_output >> 1;
        }
    }

    return crc_output;
}
/******************************************************************************
    Public function definitions
******************************************************************************/
int load_file(const char *src_path, int set_chunck_section)
{
    FILE *fp = NULL;

    binary_file.path = src_path;
    binary_file.chunck_section = (set_chunck_section <= MAX_CHUNK_BYTES_SIZE) ? set_chunck_section : MAX_CHUNK_BYTES_SIZE;

    fp = fopen(binary_file.path, "rb");
    if(NULL == fp)
    {
        perror("file open\n");
        return EXIT_FAILURE;
    }

    fseek(fp, 0L, SEEK_END);
    binary_file.size_f_bytes = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    if(binary_file.size_f_bytes % binary_file.chunck_section == 0)
    {
        binary_file.num_of_packets = binary_file.size_f_bytes / binary_file.chunck_section;
    }
    else
    {
        binary_file.num_of_packets = (binary_file.size_f_bytes / binary_file.chunck_section) + 1;
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

int get_packets(void)
{
    return binary_file.num_of_packets;
}

unsigned long get_size_in_bytes(void)
{
    return binary_file.size_f_bytes;
}

int get_chunk_section_set(void)
{
    return binary_file.chunck_section;
}

uint8_t *get_data_packet(uint8_t da, uint32_t packet, int *len)
{
    unsigned long offset = 0; 
    unsigned long bytes = 0; 
    uint16_t crc = 0xFFFF;
    FILE *fp = NULL;

    /* check first */
    if(binary_file.path == NULL || binary_file.num_of_packets == 0 || binary_file.chunck_section == 0 || binary_file.size_f_bytes == 0)
    {
        perror("no load any file\n");
        return NULL;   
    }

    offset = packet * binary_file.chunck_section;
    
    fp = fopen(binary_file.path, "rb");
    if(NULL == fp)
    {
        perror("file open\n");
        return NULL;
    }

    fseek(fp, offset, SEEK_SET);
    bytes = fread(&data_packet[0] + 5, sizeof(uint8_t), binary_file.chunck_section, fp);

    if(bytes < binary_file.chunck_section && packet != (binary_file.num_of_packets - 1))
    {
        perror("get data error:\n");
        return NULL; // error.
    }

    data_packet[0] = da;
    data_packet[1] = (uint8_t) packet;
	data_packet[2] = (uint8_t) (packet >> 8);
    data_packet[3] = (uint8_t) binary_file.num_of_packets;
    data_packet[4] = (uint8_t) (binary_file.num_of_packets >> 8);

	crc = crc_16_calculator(data_packet, bytes + 5);

	data_packet[bytes + 5] = (uint8_t)(crc >> 8);   // crch
	data_packet[bytes + 6] = (uint8_t)crc;          // crcl
    *len = bytes + 7;

    fclose(fp);
    return data_packet;
}
