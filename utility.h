#ifndef UTILITY_H
#define	UTILITY_H

void float2char(unsigned int g, unsigned int l, float number, char *str);
float char2float(char *str);

unsigned char Change10ToBCD(unsigned char data);
unsigned char ChangeBCDTo10(unsigned char data);

#endif	/* UTILITY_H */

