/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;                                                                     
;                     Software License Agreement                      
;                                                                     
;     �2007 Microchip Technology Inc
;     Mirochip Technology Inc. ("Microchip") licenses this software to 
;     you solely for the use with Microchip Products. The software is
;     owned by Microchip and is protected under applicable copyright
;     laws. All rights reserved.
;
;     SOFTWARE IS PROVIDED "AS IS." MICROCHIP EXPRESSLY DISCLAIMS ANY
;     WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
;     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
;     PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
;     BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
;     DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
;     PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
;     BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
;     ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
;     
;                                                                
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;	Filename:			    i2c_Func.c            
;	Date:				    February 21, 2007          
;	File Version:		  	1.0                             
;	Assembled using:		MPLAB IDE 7.51.00.0               
; 	Author:		  	    	Martin Bowman              
;	Company:			    Microchip Technology, Inc.
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//#include "P33ep128gp504.h"
#include "i2c.h"
#include "syschip.h"
void delay(void){
 for(unsigned i=0;i<999;i++)
        ;   
}
/*********************************************************************
* Function:        InitI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Initialises the I2C(1) peripheral
*
* Note:			Sets up Master mode, No slew rate control, 100Khz
********************************************************************/
unsigned int InitI2C(void)
{	
	//This function will initialize the I2C(1) peripheral.
	//First set the I2C(2) BRG Baud Rate.

	//Consult the dSPIC Data Sheet for information on how to calculate the
	//Baud Rate. fcy=fosc/2=140/2=70M --> 70M
    //fcy/2/fscl=35 000
    //doze-> 2  
    //fcy=35M
//fcy/fscl/2-2=BRG
    //35M/2/fscl=35000/100/2=175
    //175-1.75-2=171-> 100K
    
    //35M/100k=350
    //350-3.5-2=344->100k
    CLKDIVbits.ROI =0;
    CLKDIVbits.DOZEN=0;
    CLKDIVbits.DOZE=1;//default is 8
    delay();
    CLKDIVbits.DOZEN=1;//peripheral divided by 2->70M/2=35M
    //70 0/4=175
	I2C2BRG = //
//            171;//
            344;
//            510;//0x004f
    
	//Now we will initialise the I2C peripheral for Master Mode, No Slew Rate
	//Control, and leave the peripheral switched off.
	
	I2C2CON = 0x1200;
	
	I2C2RCV = 0x0000;
	I2C2TRN = 0x0000;
	//Now we can enable the peripheral
	 
//	I2C2CON |= 0x9200;
    return(0);
}


/*********************************************************************
* Function:        StartI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates an I2C Start Condition
*
* Note:			None
********************************************************************/
unsigned int StartI2C(void)
{
	//This function generates an I2C start condition and returns status 
	//of the Start.
    /*
     * 
#define SCL		LATBbits.LATB4                             
#define SDA		LATAbits.LATA8                             
#define DELAY 	I2C_delay_ms(100)
     */
	LATBbits.LATB4 =0;
	 delay();delay();delay();delay();delay();delay();delay();delay();delay();
      delay();delay();delay();delay();delay();delay();delay();delay();delay();
	LATBbits.LATB8 =1;
	  delay();delay();delay();delay();delay();delay();delay();delay();delay();
    	I2C2CON |= 0x9200;
	I2C2CONbits.SEN = 1;		//Generate Start COndition
////    delay();delay();delay();delay();delay();delay();
//	while (I2C2CONbits.SEN);	//Wait for Start COndition
	//return(I2C2STATbits.S);	//Optionally return status
    return(0);
}


/*********************************************************************
* Function:        RestartI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a restart condition and optionally returns status
*
* Note:			None
********************************************************************/
unsigned int RestartI2C(void)
{
	//This function generates an I2C Restart condition and returns status 
	//of the Restart.

	I2C2CONbits.RSEN = 1;		//Generate Restart		
	while (I2C2CONbits.RSEN);	//Wait for restart	
	//return(I2C2STATbits.S);	//Optional - return status
    delay();
    return(0);
}


/*********************************************************************
* Function:        StopI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a bus stop condition
*
* Note:			None
********************************************************************/
unsigned int StopI2C(void)
{
	//This function generates an I2C stop condition and returns status 
	//of the Stop.

	I2C2CONbits.PEN = 1;		//Generate Stop Condition
	while (I2C2CONbits.PEN);	//Wait for Stop
	//return(I2C2STATbits.P);	//Optional - return status
    delay();
    return(0);
}


/*********************************************************************
* Function:        WriteI2C()
*
* Input:		Byte to write.
*
* Output:		None.
*
* Overview:		Writes a byte out to the bus
*
* Note:			None
********************************************************************/
unsigned int WriteI2C(unsigned char byte)
{
	//This function transmits the byte passed to the function
//	while (I2C2STATbits.TRSTAT);	//Wait for bus to be idle
	I2C2TRN = byte;					//Load byte to I2C2 Transmit buffer
	while (I2C2STATbits.TBF);		//wait for data transmission
    delay();
return(0);
}


/*********************************************************************
* Function:        IdleI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Waits for bus to become Idle
*
* Note:			None
********************************************************************/
unsigned int IdleI2C(void)
{
	while (I2C2STATbits.TRSTAT);		//Wait for bus Idle
    delay();
    return(0);
}


/*********************************************************************
* Function:        LDByteWriteI2C()
*
* Input:		Control Byte, 8 - bit address, data.
*
* Output:		None.
*
* Overview:		Write a byte to low density device at address LowAdd
*
* Note:			None
********************************************************************/
unsigned int LDByteWriteI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char data)
{
	unsigned int ErrorCode;

	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start COndition
	WriteI2C(ControlByte);			//Write Control byte
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status
	
	WriteI2C(LowAdd);				//Write Low Address
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(data);					//Write Data
	IdleI2C();
	StopI2C();						//Initiate Stop Condition
	EEAckPolling(ControlByte);		//Perform ACK polling
	return(ErrorCode);
}


/*********************************************************************
* Function:        LDByteReadI2C()
*
* Input:		Control Byte, Address, *Data, Length.
*
* Output:		None.
*
* Overview:		Performs a low density read of Length bytes and stores in *Data array
*				starting at Address.
*
* Note:			None
********************************************************************/
unsigned int LDByteReadI2C(unsigned char ControlByte, unsigned char Address, unsigned char *Data, unsigned char Length)
{
	IdleI2C();					//wait for bus Idle
	StartI2C();					//Generate Start Condition
	WriteI2C(ControlByte);		//Write Control Byte
	IdleI2C();					//wait for bus Idle
	WriteI2C(Address);			//Write start address
	IdleI2C();					//wait for bus Idle

	RestartI2C();				//Generate restart condition
	WriteI2C(ControlByte | 0x01);	//Write control byte for read
	IdleI2C();					//wait for bus Idle

	getsI2C(Data, Length);		//read Length number of bytes
	NotAckI2C();				//Send Not Ack
	StopI2C();					//Generate Stop
    return(0);
}


/*********************************************************************
* Function:        HDByteWriteI2C()
*
* Input:		ControlByte, HighAddr, LowAddr, Data.
*
* Output:		None.
*
* Overview:		perform a high density byte write of data byte, Data.
*				starting at the address formed from HighAdd and LowAdd
*
* Note:			None
********************************************************************/
unsigned int HDByteWriteI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char data)
{
	unsigned int ErrorCode;

	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start COndition
	WriteI2C(ControlByte);			//Write Control byte
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status
	
	WriteI2C(HighAdd);
	IdleI2C();						//Write High Address
	WriteI2C(LowAdd);				//Write Low Address
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(data);					//Write Data
	IdleI2C();
	StopI2C();						//Initiate Stop Condition
	EEAckPolling(ControlByte);		//perform Ack Polling
	return(ErrorCode);
}


/*********************************************************************
* Function:        HDByteReadI2C()
*
* Input:		Control Byte, HighAdd, LowAdd, *Data, Length.
*
* Output:		None.
*
* Overview:		Performs a low density read of Length bytes and stores in *Data array
*				starting at Address formed from HighAdd and LowAdd.
*
* Note:			None
********************************************************************/
unsigned int HDByteReadI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char *Data, unsigned char Length)
{
//    unsigned int i;
    
	IdleI2C();						//Wait for bus Idle
	StartI2C();						//Generate Start condition
	WriteI2C(ControlByte);			//send control byte for write
	IdleI2C();						//Wait for bus Idle

	WriteI2C(HighAdd);				//Send High Address
	IdleI2C();						//Wait for bus Idle
	WriteI2C(LowAdd);				//Send Low Address
	IdleI2C();						//Wait for bus Idle

	RestartI2C();					//Generate Restart
	WriteI2C(ControlByte | 0x01);	//send control byte for Read
	IdleI2C();						//Wait for bus Idle

	getsI2C(Data, Length);			//Read Length number of bytes to Data
	NotAckI2C();					//send Not Ack
	StopI2C();						//Send Stop Condition
	return(0);
}
//////////////////////////////
unsigned int HDByteReadRandomI2C(unsigned char ControlByte, unsigned char Add,  unsigned char *Data)
{
//    unsigned int i;
    
	IdleI2C();						//Wait for bus Idle
	StartI2C();						//Generate Start condition
//      delay();  delay();  delay();  delay();  delay();  delay();  delay();
	WriteI2C(ControlByte);			//send control byte for write
	IdleI2C();						//Wait for bus Idle
//    while(1);
//	WriteI2C(HighAdd);				//Send High Address
//	IdleI2C();						//Wait for bus Idle
	WriteI2C(Add);				//Send Low Address
	IdleI2C();						//Wait for bus Idle

	RestartI2C();					//Generate Restart
	WriteI2C(ControlByte | 0x01);	//send control byte for Read
	IdleI2C();						//Wait for bus Idle

	getsI2C(Data, 1);			//Read Length number of bytes to Data
	NotAckI2C();					//send Not Ack
	StopI2C();						//Send Stop Condition
	return(0);
}
unsigned int HDByteWriteRandomI2C(unsigned char ControlByte, unsigned char Add,  unsigned char Data)
//unsigned int HDByteWriteI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char data)
{
	unsigned int ErrorCode;

	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start COndition
	WriteI2C(ControlByte);			//Write Control byte
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status
	
	WriteI2C(Add);
	IdleI2C();						//Write High Address
//	WriteI2C(LowAdd);				//Write Low Address
//	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(Data);					//Write Data
	IdleI2C();
	StopI2C();						//Initiate Stop Condition
//	EEAckPolling(ControlByte);		//perform Ack Polling
	return(ErrorCode);
}
/*********************************************************************
* Function:        LDPageWriteI2C()
*
* Input:		ControlByte, LowAdd, *wrptr.
*
* Output:		None.
*
* Overview:		Write a page of data from array pointed to be wrptr
*				starting at LowAdd
*
* Note:			LowAdd must start on a page boundary
********************************************************************/
unsigned int LDPageWriteI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char *wrptr)
{
	IdleI2C();					//wait for bus Idle
	StartI2C();					//Generate Start condition
	WriteI2C(ControlByte);		//send controlbyte for a write
	IdleI2C();					//wait for bus Idle
	WriteI2C(LowAdd);			//send low address
	IdleI2C();					//wait for bus Idle
	putstringI2C(wrptr);		//send data
	IdleI2C();					//wait for bus Idle
	StopI2C();					//Generate Stop
	return(0);
}


/*********************************************************************
* Function:        HDPageWriteI2C()
*
* Input:		ControlByte, HighAdd, LowAdd, *wrptr.
*
* Output:		None.
*
* Overview:		Write a page of data from array pointed to be wrptr
*				starting at address from HighAdd and LowAdd
*
* Note:			Address must start on a page boundary
********************************************************************/
unsigned int HDPageWriteI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char *wrptr)
{
//    unsigned int i;
	IdleI2C();				//wait for bus Idle
	StartI2C();				//Generate Start condition
	WriteI2C(ControlByte);	//send controlbyte for a write
	IdleI2C();				//wait for bus Idle
	WriteI2C(HighAdd);		//send High Address
	IdleI2C();				//wait for bus Idle
	WriteI2C(LowAdd);		//send Low Address
	IdleI2C();				//wait for bus Idle
	putstringI2C(wrptr);	//Send data
	IdleI2C();				//wait for bus Idle
	StopI2C();				//Generate a stop
	return(0);
}


/*********************************************************************
* Function:        LDSequentialReadI2C()
*
* Input:		ControlByte, address, *rdptr, length.
*
* Output:		None.
*
* Overview:		Performs a sequential read of length bytes starting at address
*				and places data in array pointed to by *rdptr
*
* Note:			None
********************************************************************/
unsigned int LDSequentialReadI2C(unsigned char ControlByte, unsigned char address, unsigned char *rdptr, unsigned char length)
{
	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Initiate start condition
	WriteI2C(ControlByte);			//write 1 byte
	IdleI2C();						//Ensure module is Idle
	WriteI2C(address);				//Write word address
	IdleI2C();						//Ensure module is idle
	RestartI2C();					//Generate I2C Restart Condition
	WriteI2C(ControlByte | 0x01);	//Write 1 byte - R/W bit should be 1 for read
	IdleI2C();						//Ensure bus is idle
	getsI2C(rdptr, length);			//Read in multiple bytes
	NotAckI2C();					//Send Not Ack
	StopI2C();						//Send stop condition
	return(0);
}


/*********************************************************************
* Function:        HDSequentialReadI2C()
*
* Input:		ControlByte, HighAdd, LowAdd, *rdptr, length.
*
* Output:		None.
*
* Overview:		Performs a sequential read of length bytes starting at address
*				and places data in array pointed to by *rdptr
*
* Note:			None
********************************************************************/
unsigned int HDSequentialReadI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char *rdptr, unsigned char length)
{
	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Initiate start condition
	WriteI2C(ControlByte);			//write 1 byte
	IdleI2C();						//Ensure module is Idle
	WriteI2C(HighAdd);				//Write High word address
	IdleI2C();						//Ensure module is idle
	WriteI2C(LowAdd);				//Write Low word address
	IdleI2C();						//Ensure module is idle
	RestartI2C();					//Generate I2C Restart Condition
	WriteI2C(ControlByte | 0x01);	//Write 1 byte - R/W bit should be 1 for read
	IdleI2C();						//Ensure bus is idle
	getsI2C(rdptr, length);			//Read in multiple bytes
	NotAckI2C();					//Send Not Ack
	StopI2C();						//Send stop condition
	return(0);
}


/*********************************************************************
* Function:        ACKStatus()
*
* Input:		None.
*
* Output:		Acknowledge Status.
*
* Overview:		Return the Acknowledge status on the bus
*
* Note:			None
********************************************************************/
unsigned int ACKStatus(void)
{
	return (!I2C2STATbits.ACKSTAT);		//Return Ack Status
}


/*********************************************************************
* Function:        NotAckI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a NO Acknowledge on the Bus
*
* Note:			None
********************************************************************/
unsigned int NotAckI2C(void)
{
	I2C2CONbits.ACKDT = 1;			//Set for NotACk
	I2C2CONbits.ACKEN = 1;
	while(I2C2CONbits.ACKEN);		//wait for ACK to complete
	I2C2CONbits.ACKDT = 0;			//Set for NotACk
    return(0);
}


/*********************************************************************
* Function:        AckI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates an Acknowledge.
*
* Note:			None
********************************************************************/
unsigned int AckI2C(void)
{
	I2C2CONbits.ACKDT = 0;			//Set for ACk
	I2C2CONbits.ACKEN = 1;
	while(I2C2CONbits.ACKEN);		//wait for ACK to complete
    return(0);
}


/*********************************************************************
* Function:       getsI2C()
*
* Input:		array pointer, Length.
*
* Output:		None.
*
* Overview:		read Length number of Bytes into array
*
* Note:			None
********************************************************************/
unsigned int getsI2C(unsigned char *rdptr, unsigned char Length)
{
	while (Length --)
	{
		*rdptr++ = getI2C();		//get a single byte
		
		if(I2C2STATbits.BCL)		//Test for Bus collision
		{
			return(-1);
		}

		if(Length)
		{
			AckI2C();				//Acknowledge until all read
		}
	}
	return(0);
}


/*********************************************************************
* Function:        getI2C()
*
* Input:		None.
*
* Output:		contents of I2C2 receive buffer.
*
* Overview:		Read a single byte from Bus
*
* Note:			None
********************************************************************/
unsigned int getI2C(void)
{
	I2C2CONbits.RCEN = 1;			//Enable Master receive
//	Nop();-----------
	while(!I2C2STATbits.RBF);		//Wait for receive bufer to be full
	return(I2C2RCV);				//Return data in buffer
}


/*********************************************************************
* Function:        EEAckPolling()
*
* Input:		Control byte.
*
* Output:		error state.
*
* Overview:		polls the bus for an Acknowledge from device
*
* Note:			None
********************************************************************/
unsigned int EEAckPolling(unsigned char control)
{
	IdleI2C();				//wait for bus Idle
	StartI2C();				//Generate Start condition
	
	if(I2C2STATbits.BCL)
	{
		return(-1);			//Bus collision, return
	}

	else
	{
		if(WriteI2C(control))
		{
			return(-3);		//error return
		}

		IdleI2C();			//wait for bus idle
		if(I2C2STATbits.BCL)
		{
			return(-1);		//error return
		}

		while(ACKStatus())
		{
			RestartI2C();	//generate restart
			if(I2C2STATbits.BCL)
			{
				return(-1);	//error return
			}

			if(WriteI2C(control))
			{
				return(-3);
			}

			IdleI2C();
		}
	}
	StopI2C();				//send stop condition
	if(I2C2STATbits.BCL)
	{
		return(-1);
	}
	return(0);
}


/*********************************************************************
* Function:        putstringI2C()
*
* Input:		pointer to array.
*
* Output:		None.
*
* Overview:		writes a string of data upto PAGESIZE from array
*
* Note:			None
********************************************************************/
unsigned int putstringI2C(unsigned char *wrptr)
{
	unsigned char x;

	for(x = 0; x < PAGESIZE; x++)		//Transmit Data Until Pagesize
	{	
		if(WriteI2C(*wrptr))			//Write 1 byte
		{
			return(-3);				//Return with Write Collision
		}
		IdleI2C();					//Wait for Idle bus
		if(I2C2STATbits.ACKSTAT)
		{
			return(-2);				//Bus responded with Not ACK
		}
		wrptr++;
	}
	return(0);
}
