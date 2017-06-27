#include "CCP_CAN_Interface.h"

static CCP_BYTE disable_interrupt_count = 0;

void ccpSend(CCP_BYTEPTR msg)
{
	ccpBootTransmitCRM(msg);
}

CCP_BYTEPTR reverseByteOrder(CCP_DWORD addr, CCP_BYTE len)
{
	int i, j;
	CCP_DWORD addr_rev = 0x0;
	CCP_DWORD mask = 0xFF;
	for(i = 1, j=0; i<=len; ++i, j+=8)
	{
		addr_rev |= ((addr & mask) >> j) << (len-i)*8;
		mask <<= i*8;
	}

	return (CCP_BYTEPTR)addr_rev;
}

CCP_MTABYTEPTR ccpGetPointer(CCP_BYTE addr_ext, CCP_DWORD addr)
{
#ifdef CCP_MASTER_MSB_FIRST
#ifdef CCP_INTEL
	CCP_DWORD addr_intel = addr;
	addr_intel = ((addr & 0xFF) << 24) | ((addr & 0xFF00) << 8) | ((addr & 0x00FF0000) >> 8) | ((addr & 0xFF000000) >> 24);
	return (CCP_MTABYTEPTR)addr_intel;
#endif

#else
	return (CCP_MTABYTEPTR)addr;
#endif
}

void ccpUserBackground(void)
{

}

void disable_interrupt()
{
	// Disable interrupt operator.
	__disable_irq();
	++disable_interrupt_count;
}
	
void enable_interrupt()
{
	if(--disable_interrupt_count == 0)
	{
		// now can enable them.
		__enable_irq();
	}
}

void TIM10_ISR_CCP_DAQ_1ms_Task(void)
{
}
