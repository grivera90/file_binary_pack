/**
*******************************************************************************
* @file           : file_binary_pack.h
* @brief          : Description of header file
* @author         : Gonzalo Rivera
* @date           : 08/06/2023
*******************************************************************************
* @attention
*
* Copyright (c) <date> Gonzalo Rivera. All rights reserved.
*
*/
#ifndef __FILE_BINARY_PACK_H__
#define __FILE_BINARY_PACK_H__
/******************************************************************************
        Includes
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>
/******************************************************************************
        Constants
 ******************************************************************************/
#define MAX_CHUNK_BYTES_SIZE 1024
/******************************************************************************
        Data types
 ******************************************************************************/

/******************************************************************************
        Public function prototypes
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function to load a file and set a reading portion.
 * 
 * @param const char *src_path:
 * 
 * @param int set_chunck_section:
 * 
 * @return int: 0 to OK 1 for Error.
 */
int load_file(const char *src_path, int set_chunck_section);

/**
 * @brief Function to get the packets count of file divided.
 * 
 * @return int: packets.
 */
int get_packets(void);

/**
 * @brief Function to get the size of file loaded.
 * 
 * @return unsigned long: size in bytes.
 */
unsigned long get_size_in_bytes(void);

/**
 * @brief Function to get the portion set.
 * 
 * @return int: chunck set.
 */
int get_chunk_section_set(void);

/**
 * @brief Function to get a packet in specific ready to use with a defined format.
 * 
 * @param uint8_t da: destination address.
 * 
 * @param uint32_t packet: packet number.
 * 
 * @param int *len: return the length of packet requested.
 * 
 * @return uint8_t *: pointer to the packet. 
 */
uint8_t *get_data_packet(uint8_t da, uint32_t packet, int *len);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // EOF __FILE_BINARY_PACK_H__