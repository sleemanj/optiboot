/** Read an intel hex file, take the reset vector (0) and put it into 
 *  some other vector (argv[1]) and then put an rjmp to a given address.
 * 
 *  In other words, rewrite the reset vector so it points to a bootloader.
 * 
 *  Used for virtual boot with combined tiny tuner
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint8_t readByteHex(const char *firstDigit);
void writeByteHex(uint8_t byteToWrite, char *firstDigit);
void updateChecksum(char *buffer);
uint8_t  inThisPage(uint16_t address, const char *buffer);

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Usage: %s [new_vector_decimal_index] [bootloader_address_hex]\n\n", argv[0]);
		return 1;
	}
	
	uint8_t  newVector = atoi(argv[1])*2;
	uint16_t bootloader= strtoul(argv[2], NULL, 16);
	uint16_t firmware  = bootloader;
	
	fprintf(stderr,"New Vector Byte: %d\n", newVector);
	fprintf(stderr,"Bootloader: %d\n", bootloader);
	
	char buffer[120];
	
	while(!feof(stdin))
	{
		if(fgets(buffer, sizeof(buffer), stdin) == NULL) break;
		
		// For convenience you can pass in 0 as the newVector and we 
		// won't actually make any changes, for chips that have a 
		// separate boot flash area and handle that properly themselves
		if(newVector == 0)
		{
			printf("%s", buffer);
			continue;
		}
		
		if( buffer[3]  == '0' && buffer[4] == '0' && buffer[5] == '0' && buffer[6] == '0' // address 0
		&&  buffer[7]  == '0' && buffer[8] == '0'                                         // type = data
		)
		{
			fprintf(stderr,"Found Address 0x0000\n");
			
			// Read in the first and second bytes these will (should) be a relative jump
			fprintf(stderr,"  %s\n", buffer);
			// First byte is low byte, all address bits
			// Second byte is high byte, lower half are address, upper half is rjmp
			firmware = ( (readByteHex(&buffer[11]) & 0x0F) << 8 ) | readByteHex(&buffer[9]);
			
			fprintf(stderr, "Firmware is found at 0x%02X\n", firmware);
			fprintf(stderr, "Rewrite reset vector to rjmp to bootloader at 0x%04X\n", bootloader);
			
			// Replace it with the bootloader, note rjmp is number of WORDS to jump
			// and then lose one more ( PC = PC + k + 1 where k is the number of words to jump )
			writeByteHex((bootloader/2)-1, &buffer[9]);
			writeByteHex(0xC0 | ((((bootloader/2)-1)>>8)&0xF), &buffer[11]);
			
			// Update the checksum
			fprintf(stderr, "Update checksum\n");
			updateChecksum(buffer);
			fprintf(stderr, "  %s\n", buffer);
		}
		
		uint8_t newVectorIndex = inThisPage(newVector, buffer);
		if(newVectorIndex)
		{
			// Insert the firmware boot vector into there except because
			// it will be an rjmp we need to modify the destination
			// by the difference between the original reset vector (0)
			// and the new vector **MEASURED IN WORDS**
			firmware -= ((newVector-0)/2);
			fprintf(stderr, "Inserting firmware rjmp at vector 0x%02X (index %d) to 0x%02X\n", newVector, newVectorIndex, firmware);
			writeByteHex(firmware, &buffer[newVectorIndex]);
			writeByteHex(0xC0 | ((firmware>>8)&0xF), &buffer[newVectorIndex+2]);
			
			fprintf(stderr, "Update checksum\n");
			updateChecksum(buffer);
			fprintf(stderr, "  %s\n", buffer);
		}
		
		printf("%s", buffer);
	}
	
	return 0;
}

/* Determine if the given address has a word in this buffer 
 * if it does, return the byte index of the first byte in the word.
 * (0 if not, which would not be a valid position)
 */

uint8_t  inThisPage(uint16_t address, const char *buffer)
{
	// Only consider data pages
	if(buffer[7] != '0' || buffer[8] != '0') return 0;
	
	// Rationalize the address to a word address
	address = address & ~0x01;
	
	//fprintf(stderr, "Looking for %02X\n", address);
	
	uint16_t firstAddress	= (readByteHex(&buffer[3])<<8) | (readByteHex(&buffer[5]));
	if(address < firstAddress) return 0;
	
	uint8_t pageSize	= readByteHex(&buffer[1]);
	uint16_t lastAddress = firstAddress + (pageSize-1);	
	if(address > lastAddress) return 0;
	
	// Must be the difference after the first address then
	//   address = 0x02
	//   firstaddress  = 0x00
	//   0x02-0x00 = 0x02 = 3rd byte of the data
	
	//  The first index of the data is 9 (1 start code, 2 digits for count
	//   4 digits for address, 2 digits for type and then the data)
	return 9+((address-firstAddress)*2);
}

uint8_t readByteHex(const char *firstDigit)
{
	char bothDigits[3];
	bothDigits[0] = *firstDigit;
	bothDigits[1] = *(firstDigit+1);
	bothDigits[2] = '\0';
	
	//fprintf(stderr, "The bytes are %s\n", bothDigits);
	
	return (uint8_t) strtoul(bothDigits, NULL, 16);
}

void writeByteHex(uint8_t byteToWrite, char *firstDigit)
{
	char bothDigits[3] = {0};
	sprintf(bothDigits, "%02X", byteToWrite);
	*firstDigit = bothDigits[0];
	*(firstDigit+1) = bothDigits[1];
}

void updateChecksum(char *buffer)
{
	uint8_t numDataBytes = readByteHex(&buffer[1]);
	uint16_t sum 				 = numDataBytes
		+ readByteHex(&buffer[3]) // ADDR1
		+ readByteHex(&buffer[5]) // ADDR2
		+ readByteHex(&buffer[7]) // Type
	;
	uint8_t i = 9; // Next byte start
	while(numDataBytes > 0)
	{
		sum += readByteHex(&buffer[i]);
		i+=2;
		numDataBytes--;
	}
	
	writeByteHex(( (uint8_t) 0 ) - (uint8_t) sum, &buffer[i]);
}