#include <XMC1100.h>
#include <xmc_ccu4.h>
#include <xmc_gpio.h>
#include <xmc_scu.h>

#include "lcd2004.h"
#include "lcd_pwm_vo.h"

static inline void SimpleDelay(uint32_t d) {
  uint32_t t = d;
  while (--t) {
    __NOP();
  }
}

static inline void RS_H(void) { XMC_GPIO_SetOutputHigh(RS_PORT, RS_PIN); }

static inline void RS_L(void) { XMC_GPIO_SetOutputLow(RS_PORT, RS_PIN); }

static inline void E_H(void) { XMC_GPIO_SetOutputHigh(E_PORT, E_PIN); }

static inline void E_L(void) { XMC_GPIO_SetOutputLow(E_PORT, E_PIN); }

static inline void D4_H(void) { XMC_GPIO_SetOutputHigh(D4_PORT, D4_PIN); }

static inline void D4_L(void) { XMC_GPIO_SetOutputLow(D4_PORT, D4_PIN); }

static inline void D5_H(void) { XMC_GPIO_SetOutputHigh(D5_PORT, D5_PIN); }

static inline void D5_L(void) { XMC_GPIO_SetOutputLow(D5_PORT, D5_PIN); }

static inline void D6_H(void) { XMC_GPIO_SetOutputHigh(D6_PORT, D6_PIN); }

static inline void D6_L(void) { XMC_GPIO_SetOutputLow(D6_PORT, D6_PIN); }

static inline void D7_H(void) { XMC_GPIO_SetOutputHigh(D7_PORT, D7_PIN); }

static inline void D7_L(void) { XMC_GPIO_SetOutputLow(D7_PORT, D7_PIN); }

static inline void DB4_Wr(uint8_t dat) {
  (0 == (dat & 0x08)) ? D7_L() : D7_H();
  (0 == (dat & 0x04)) ? D6_L() : D6_H();
  (0 == (dat & 0x02)) ? D5_L() : D5_H();
  (0 == (dat & 0x01)) ? D4_L() : D4_H();
}

void LCD_WrCmd_4(uint8_t cmd) {
  SimpleDelay(WAIT_AVAIL_DLY);

  RS_L();
  SimpleDelay(10);
  //	RW_L();
  DB4_Wr(cmd >> 4);
  SimpleDelay(10);
  E_H();
  SimpleDelay(10);
  E_L();
  SimpleDelay(10);
  DB4_Wr(cmd & 0x0F);
  SimpleDelay(10);
  E_H();
  SimpleDelay(10);
  E_L();
}

void LCD_WrDat_4(uint8_t dat) {
  SimpleDelay(WAIT_AVAIL_DLY);

  RS_H();
  SimpleDelay(10);
  //	RW_L();
  DB4_Wr(dat >> 4);
  SimpleDelay(10);
  E_H();
  SimpleDelay(10);
  E_L();
  SimpleDelay(10);
  DB4_Wr(dat & 0x0F);
  SimpleDelay(10);
  E_H();
  SimpleDelay(10);
  E_L();
}

void LCD_SetPos(uint8_t x, uint8_t y) {
  const static uint8_t pos_tab[] = {0x80, 0xc0, 0x94, 0xd4};

  LCD_WrCmd_4(pos_tab[x] + y);
}

void LCD_Initialize(void) {
  XMC_GPIO_SetMode(RS_PORT, RS_PIN, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
  XMC_GPIO_SetMode(E_PORT, E_PIN, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);

  // DB4-DB7
  XMC_GPIO_SetMode(D4_PORT, D4_PIN, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
  XMC_GPIO_SetMode(D5_PORT, D5_PIN, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
  XMC_GPIO_SetMode(D6_PORT, D6_PIN, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
  XMC_GPIO_SetMode(D7_PORT, D7_PIN, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);


  // 4-bit mode
  LCD_WrCmd_4(0x33);
  SimpleDelay(10);

  LCD_WrCmd_4(0x32);
  SimpleDelay(10);

  LCD_WrCmd_4(FUNCTION_SET | OPT_N);
  SimpleDelay(10);

  LCD_WrCmd_4(CLEAR_DISPLAY);
  SimpleDelay(10);
	
  LCD_WrCmd_4(DISPLAY_ON_OFF_CONTROL | OPT_D);
  SimpleDelay(10);
	
  LCD_WrCmd_4(ENTRY_MODE_SET | OPT_INC);
  SimpleDelay(10);

  LCD_PWM_VO_Init();

  LCD_VO_Config(2050);
}

void LCD_displayL(uint8_t l, uint8_t hori, uint8_t *s) {
  LCD_SetPos(l, hori);
  while (*s) {
    LCD_WrDat_4(*s);
    s++;
  }
}
