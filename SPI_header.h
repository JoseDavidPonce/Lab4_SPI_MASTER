#ifndef __SPI_HEADER_H_
#define	__SPI_HEADER_H_

#include <xc.h> 
#include <stdint.h>

void Init_SPI (uint8_t a, uint8_t just_one_slave);
void Init_SPI_int(void);


#endif	

