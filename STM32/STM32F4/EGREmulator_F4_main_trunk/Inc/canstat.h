/*
**                   Copyright 1995-2011 by KVASER AB, SWEDEN      
**                        WWW: http://www.kvaser.com
**
** This software is furnished under a license and may be used and copied
** only in accordance with the terms of such license.
**
** Description:
**   This file defines status codes for canlib.
** ---------------------------------------------------------------------------
*/
#ifndef _CANSTAT_H_
#define _CANSTAT_H_
#define NULL 0
#ifndef TRUE
# define TRUE                          (1U)
#endif

#ifndef FALSE
# define FALSE                         (0U)
#endif
//
// Don't forget to update canGetErrorText in canlib.c if this is changed!
//
typedef enum {
	  canERR_TX								= 0x00,     // Error in parameter
    canOK                  			= 0x01,		 //CAN transmission succeeded
    canERR_PENDING        = 0x02,    // CAN transmission pending
	  canERR_NOMSG			  = 0x03,    //Timeout occurred
    canERR_NO_MB      	  = 0x04,    // CAN cell did not provide an empty mailbox
    // The last entry - a dummy so we know where NOT to place a comma.
    canERR_RESERVED       = 0x10
} canStatus;

/*
** Common bus speeds. Used in canSetBaud().
*/
typedef enum{
	 canBITRATE_1M = -1,
	 canBITRATE_500K = -2,
	 canBITRATE_250K = -3,
	 canBITRATE_125K = -4,
	 canBITRATE_100K = -5,
	 canBITRATE_62K = -6,
	 canBITRATE_50K = -7,
	 canBITRATE_83K  = -8,
	 canBITRATE_10K = -9
}canBITRATE;

#define CANSTATUS_SUCCESS(X) ((X) == canOK)
#define CANSTATUS_FAILURE(X) ((X) != canOK)

// Circuit status flags.
//
#define canSTAT_ERROR_PASSIVE   0x00000001  // The circuit is error passive
#define canSTAT_BUS_OFF         0x00000002  // The circuit is Off Bus
#define canSTAT_ERROR_WARNING   0x00000004  // At least one error counter > 96
#define canSTAT_ERROR_ACTIVE    0x00000008  // The circuit is error active.
#define canSTAT_TX_PENDING      0x00000010  // There are messages pending transmission
#define canSTAT_RX_PENDING      0x00000020  // There are messages in the receive buffer
#define canSTAT_RESERVED_1      0x00000040
#define canSTAT_TXERR           0x00000080  // There has been at least one TX error
#define canSTAT_RXERR           0x00000100  // There has been at least one RX error of some sort
#define canSTAT_HW_OVERRUN      0x00000200  // The has been at least one HW buffer overflow
#define canSTAT_SW_OVERRUN      0x00000400  // The has been at least one SW buffer overflow
//
// For convenience.
#define canSTAT_OVERRUN         (canSTAT_HW_OVERRUN | canSTAT_SW_OVERRUN)

//
// Message information flags, < 0x100
// All flags and/or combinations of them are meaningful for received messages
// RTR,STD,EXT,WAKEUP,ERROR_FRAME are meaningful also for transmitted messages
//
#define canMSG_MASK             0x00ff      // Used to mask the non-info bits
#define canMSG_RTR              0x0001      // Message is a remote request
#define canMSG_STD              0x0002      // Message has a standard ID
#define canMSG_EXT              0x0004      // Message has an extended ID
#define canMSG_WAKEUP           0x0008      // Message to be sent / was received in wakeup mode
#define canMSG_NERR             0x0010      // NERR was active during the message
#define canMSG_ERROR_FRAME      0x0020      // Message is an error frame
#define canMSG_TXACK            0x0040      // Message is a TX ACK (msg is really sent)
#define canMSG_TXRQ             0x0080      // Message is a TX REQUEST (msg is transfered to the chip)

//
// Message error flags, >= 0x0100
//
#define canMSGERR_MASK          0xff00      // Used to mask the non-error bits
// 0x0100 reserved
#define canMSGERR_HW_OVERRUN    0x0200      // HW buffer overrun
#define canMSGERR_SW_OVERRUN    0x0400      // SW buffer overrun
#define canMSGERR_STUFF         0x0800      // Stuff error
#define canMSGERR_FORM          0x1000      // Form error
#define canMSGERR_CRC           0x2000      // CRC error
#define canMSGERR_BIT0          0x4000      // Sent dom, read rec
#define canMSGERR_BIT1          0x8000      // Sent rec, read dom

//
// Convenience values for the message error flags.
//
#define canMSGERR_OVERRUN       0x0600      // Any overrun condition.
#define canMSGERR_BIT           0xC000      // Any bit error.
#define canMSGERR_BUSERR        0xF800      // Any RX error

#endif
