// loosely based on LXSAMD21DMX by Claude Heintz

#include <Arduino.h>
#include <SERCOM.h>
#include <Uart.h>
#include <variant.h>
#include <wiring_private.h> 
#include "samd21dmx.h"

samd21dmx dmx;

Uart SerialDMX (&sercom2, 0ul, 1ul, SERCOM_RX_PAD_3, UART_TX_PAD_2);

void SERCOM2_Handler ()
{
  // digitalWrite (11, HIGH);

  if ( SERCOM2->USART.INTFLAG.bit.TXC ) {
  	dmx.transmissionComplete();
  } else if ( SERCOM2->USART.INTFLAG.bit.DRE ) {
    dmx.dataRegisterEmpty();
  }

  // digitalWrite (11, LOW);
}

void setBaudRate (uint32_t baudrate)
{
  SERCOM2->USART.CTRLA.bit.ENABLE = 0x0u;
  uint16_t sampleRateValue = 16;
  uint32_t baudTimes8 = (SystemCoreClock * 8) / (sampleRateValue * baudrate);
  SERCOM2->USART.BAUD.FRAC.FP   = (baudTimes8 % 8);
  SERCOM2->USART.BAUD.FRAC.BAUD = (baudTimes8 / 8);
  SERCOM2->USART.CTRLA.bit.ENABLE = 0x1u;
}

samd21dmx::samd21dmx ()
{
}

samd21dmx::~samd21dmx ()
{
}

void samd21dmx::begin (void)
{
	SerialDMX.begin (250000, SERIAL_8N2);
	SERCOM2->USART.INTENCLR.reg = SERCOM_USART_INTENCLR_RXC | 
                                 SERCOM_USART_INTENCLR_ERROR;

	pinPeripheral (0ul, PIO_SERCOM_ALT);
	pinPeripheral (1ul, PIO_SERCOM_ALT);
	this->busy = false;
	this->state = DMX_STATE_BREAK;
	this->next_send_slot = DMX_SLOTS;
}

void samd21dmx::tx (uint8_t *txslots)
{
	if (!this->busy) {
		this->busy = true;
		for (int i = 0; i < DMX_SLOTS; i++) {
			this->slots[i] = txslots[i];
		}
		this->state = DMX_STATE_BREAK;
		transmissionComplete ();
	}
}

void samd21dmx::transmissionComplete (void)
{
	if (this->state == DMX_STATE_BREAK) {
		setBaudRate (DMX_BREAK_BAUD);
        this->state = DMX_STATE_START;
        next_send_slot = 0;
        SERCOM2->USART.INTENCLR.reg = SERCOM_USART_INTENCLR_DRE;
        SERCOM2->USART.INTENSET.reg = SERCOM_USART_INTENSET_TXC;
        SERCOM2->USART.DATA.reg = 0;
	} else if (this->state == DMX_STATE_IDLE) {
		SERCOM2->USART.INTFLAG.bit.TXC = 1;
        SERCOM2->USART.INTENCLR.reg = SERCOM_USART_INTENCLR_DRE;
		SERCOM2->USART.INTENCLR.reg = SERCOM_USART_INTENCLR_TXC;
		this->state = DMX_STATE_BREAK;
		this->busy = false;
	} else if (this->state == DMX_STATE_START) {
		setBaudRate(DMX_DATA_BAUD);
        this->state = DMX_STATE_DATA;
        SERCOM2->USART.INTENCLR.reg = SERCOM_USART_INTENCLR_TXC;
        SERCOM2->USART.INTENSET.reg = SERCOM_USART_INTENSET_DRE;
		SERCOM2->USART.DATA.reg = 0;
	}
}

void samd21dmx::dataRegisterEmpty (void)
{
	if (next_send_slot < DMX_SLOTS) {
		SERCOM2->USART.DATA.reg = this->slots[next_send_slot++];	
	} else {
		this->state = DMX_STATE_IDLE;
		SERCOM2->USART.INTENCLR.reg = SERCOM_USART_INTENCLR_DRE;
		SERCOM2->USART.INTENSET.reg = SERCOM_USART_INTENSET_TXC;
	}
}
