// be sure to adjust DMX slots too if this define is changed
#define RGB_LIGHTS (DMX_SLOTS/3)

enum {
  MODE_RESERVED = 0,
  MODE_BLACKOUT = 1,
  MODE_FIXED = 2,
  MODE_WASH = 3,
  MODE_RAINBOW = 4,
  MODE_RANDOM = 5,
  MODE_INDEP_RANDOM = 6,
  MODE_WHITE = 7,
  MODE_LFSR_RANDOM = 8,
  MODE_HALLOWEEN = 9,
  MODE_PURE_WHITE = 10
};

static const unsigned long randomColorTable[134] = {
    0xFF0017, 0x004BFF, 0xFF8400, 0x6800FF, 0x00FFB8, 0x004BFF, 
    0xFF2400, 0xBBFF00, 0xFF009B, 0x00FF40, 0xFF002F, 0x97FF00,
    0xFF006B, 0x4FFF00, 0xFF0047, 0x4400FF, 0x00FF04, 0xFF006B,
    0x3800FF, 0xF7FF00, 0xFF0017, 0xD400FF, 0x2BFF00, 0x5C00FF,
    0x00FF94, 0x9400FF, 0x00FF94, 0xFF00FB, 0x003FFF, 0xFF7800,
    0x00FFDC, 0xDC00FF, 0x00FFDC, 0xEBFF00, 0xFF0C00, 0xEC00FF,
    0x1FFF00, 0xD400FF, 0x006FFF, 0xFF0077, 0x00FF88, 0xA400FF,
    0xEBFF00, 0x00B7FF, 0xFF00BF, 0xFF3C00, 0x004BFF, 0xFF1800,
    0x00FFFF, 0xFFB400, 0xE000FF, 0x0003FF, 0x7FFF00, 0x0033FF,
    0x00FF64, 0xFF6C00, 0x67FF00, 0x001BFF, 0xFF00BF, 0xC7FF00,
    0x00DBFF, 0xEBFF00, 0x0057FF, 0xFF6000, 0x0093FF, 0xAFFF00,
    0x0003FF, 0xFF0083, 0xF7FF00, 0xD400FF, 0xFF0C00, 0x8000FF,
    0x00B7FF, 0x43FF00, 0x00FFAC, 0xFF002F, 0xC800FF, 0xF7FF00,
    0xFF1800, 0xBC00FF, 0x009FFF, 0x73FF00, 0xFF9C00, 0xFF0047,
    0xFF9C00, 0x6800FF, 0xFFD800, 0x00FFFF, 0xEBFF00, 0x4400FF,
    0xFF006B, 0xC7FF00, 0xFF003B, 0x00FF58, 0x0093FF, 0x4FFF00,
    0xFF00B3, 0xFFA800, 0x00DBFF, 0xA400FF, 0xDFFF00, 0xFF0047,
    0xBBFF00, 0xFF0053, 0xBC00FF, 0xFF6C00, 0xFF005F, 0x9800FF,
    0xFFE400, 0xFF0000, 0xFF00EF, 0x00B7FF, 0xDFFF00, 0xFF1800,
    0x07FF00, 0xFF00E3, 0xDFFF00, 0x2C00FF, 0xFF00B3, 0x00FF34,
    0x3400FF, 0x00FF34, 0x006FFF, 0xFF005F, 0x2000FF, 0x67FF00,
    0xFF9C00, 0x00FFE8, 0xFF4800, 0x009FFF, 0x00FF40, 0xFF005F,
    0x37FF00, 0x00ABFF
};

uint8_t effects_mode;
uint8_t effects_data[DMX_SLOTS];
uint32_t effects_fixedColor;
uint8_t effects_direction;
uint16_t effects_step;
uint32_t effects_washColor;
uint16_t effects_offset;
uint16_t effects_delay;
uint8_t effects_whiteLevel;
uint8_t effects_randomColorIndex;
uint16_t effects_randomColorTimer;
uint16_t effects_randomColors[RGB_LIGHTS];
uint16_t effects_randomLfsr16;


uint8_t xlatehi;
uint8_t xlatelo;
uint8_t xlater;
uint8_t xlateg;
uint8_t xlateb;


//---------------------------------------------------------------------------------------------
// EFFECTS INIT
//

void effects_Init (void)
{
  effects_mode = 3;
  for (int i = 0; i < DMX_SLOTS; i++) {
    effects_data[i] = 0x00;
  }
  effects_fixedColor = 0xFF0000;
  effects_direction = 1;
  effects_step = 0x7;
  effects_washColor = 0;
  effects_offset = 0x0060;
  effects_delay = 150;
  effects_whiteLevel = 0x10;
  effects_randomColorIndex = 0;
  effects_randomColorTimer = 0xFFFF;
  for (int i = 0; i < RGB_LIGHTS; i++) {
    effects_randomColors[i] = 0;
  }
  effects_randomLfsr16 = 2;
}


//---------------------------------------------------------------------------------------------
// EFFECTS RUN
//

bool effects_Run (void *)
{
  dmx.tx (effects_data);
  
  // calculate next step in effects sequence
  switch (effects_mode) {
    case MODE_BLACKOUT:     effects_ModeBlackOut ();    break;
    case MODE_FIXED:        effects_ModeFixedColor ();  break;
    case MODE_WASH:         effects_ModeColorWash ();   break;
    case MODE_RAINBOW:      effects_ModeOffsetWash ();  break;
    case MODE_RANDOM:       effects_ModeTableRandom (); break;
    case MODE_INDEP_RANDOM: effects_ModeIndepRandom (); break;
    case MODE_WHITE:        effects_ModeWhite ();       break;
  }  

  return true;
}


//---------------------------------------------------------------------------------------------
// EFFECTS GENERATORS
//

void effects_ModeBlackOut (void)
{
  for (int i = 0; i < DMX_SLOTS; i++) {
    effects_data[i] = 0x00;
  }
}

void effects_ModeFixedColor (void)
{
  uint8_t r = (effects_fixedColor >> 16) & 0xff;
  uint8_t g = (effects_fixedColor >>  8) & 0xff;
  uint8_t b = (effects_fixedColor >>  0) & 0xff;

  for (int i = 0; i < DMX_SLOTS; i+=3) {
    effects_data[i+0] = r;
    effects_data[i+1] = g;
    effects_data[i+2] = b;
  }
}

void effects_ModeColorWash (void)
{
  if (effects_direction == 0) {
    effects_AddDelayToColor ();
  } else {
    effects_SubDelayFromColor ();
  }
  xlatehi = (effects_washColor >> 16) & 0xff;
  xlatelo = (effects_washColor >> 8) & 0xff;
  xlatestate ();
  xlatetoall ();
}

void effects_ModeOffsetWash (void)
{
  if (effects_direction == 0) {
    effects_AddDelayToColor ();
  } else {
    effects_SubDelayFromColor ();
  }
  xlatehi = (effects_washColor >> 16) & 0xff;
  xlatelo = (effects_washColor >> 8) & 0xff;
  for (int i = 0; i < RGB_LIGHTS; i++) {
    xlatestate ();
    xlatetoled (i);
    uint16_t temp = (((unsigned short)xlatehi) << 8) | (unsigned short)xlatelo;
    temp += effects_offset;
    if (temp >= 1536) {
      temp -= 1536;
    }
    xlatehi = temp >> 8;
    xlatelo = temp & 0xff;
  }
}

void effects_ModeTableRandom (void)
{
  uint32_t color;

  if (effects_randomColorTimer >= effects_delay) {
    // reset timer
    effects_randomColorTimer = 0;

    // set color
    color = randomColorTable[effects_randomColorIndex];
    xlater = (color >> 16) & 0xff;
    xlateg = (color >>  8) & 0xff;
    xlateb = (color >>  0) & 0xff;
    xlatetoall ();

    // advance index
    effects_randomColorIndex = effects_randomColorIndex + 1;
    if (effects_randomColorIndex >= 134) {
      effects_randomColorIndex = 0;
    }
  } else {
    effects_randomColorTimer++;
  }
}

void effects_ModeIndepRandom (void)
{
  if (effects_randomColorTimer >= effects_delay) {
    // reset timer
    effects_randomColorTimer = 0;

    for (int i = 0; i < RGB_LIGHTS; i++) {
      // pick next random color (rndclr = (rndclr + 384 + (3*[0-255])) % 1536
      if (effects_randomLfsr16 & 1) {
        effects_randomLfsr16 = (effects_randomLfsr16 >> 1) ^ 0xB400;
      } else {
        effects_randomLfsr16 = (effects_randomLfsr16 >> 1);
      }
  
      effects_randomColors[i] += 384; 
      effects_randomColors[i] += effects_randomLfsr16 & 0xff;
      effects_randomColors[i] += effects_randomLfsr16 & 0xff;
      effects_randomColors[i] += effects_randomLfsr16 & 0xff;
      if (effects_randomColors[i] >= 1536) {
        effects_randomColors[i] -= 1536;
      }
  
      xlatehi = effects_randomColors[i] >> 8;
      xlatelo = effects_randomColors[i] & 0xff;
      xlatestate ();
      xlatetoled (i);
    }
  } else {
    effects_randomColorTimer++;
  }
}

void effects_ModeWhite (void)
{
  xlater = effects_whiteLevel;
  xlateg = effects_whiteLevel;
  xlateb = effects_whiteLevel;
  xlatetoall ();
}


//---------------------------------------------------------------------------------------------
// EFFECTS HELPER FUNCTIONS
//

void effects_AddDelayToColor (void)
{
  effects_washColor += effects_step;
  
  if (effects_washColor >= 393216) {
    effects_washColor -= 393216;
  }
}

void effects_SubDelayFromColor (void)
{
  effects_washColor -= effects_step;
  if (effects_washColor >= 393216) {
    effects_washColor += 393216;
  }
}

void xlatestate (void)
{
  switch (xlatehi) {
    case 0: xlater = 0xff;           xlateg = 0x00;           xlateb = xlatelo; break;
    case 1: xlater = 0xff - xlatelo; xlateg = 0x00;           xlateb = 0xff; break;
    case 2: xlater = 0x00;           xlateg = xlatelo;        xlateb = 0xff; break;
    case 3: xlater = 0x00;           xlateg = 0xff;           xlateb = 0xff - xlatelo; break;
    case 4: xlater = xlatelo;        xlateg = 0xff;           xlateb = 0x00; break;
    case 5: xlater = 0xff;           xlateg = 0xff - xlatelo; xlateb = 0x00; break;
  }   
}

void xlatetoall (void)
{
  for (int i = 0; i < DMX_SLOTS; i += 3) {
    effects_data[i + 0] = xlater;
    effects_data[i + 1] = xlateg;
    effects_data[i + 2] = xlateb;
  }
}

void xlatetoled (uint8_t led)
{
  effects_data[3 * led + 0] = xlater;
  effects_data[3 * led + 1] = xlateg;
  effects_data[3 * led + 2] = xlateb;
}


//---------------------------------------------------------------------------------------------
// COMMANDS FROM UI
//

void SetMode (int newMode)
{
  if ((effects_mode != MODE_RANDOM) && (newMode == MODE_RANDOM)) {
    effects_randomColorTimer = 0xffff;
  }

  if ((effects_mode != MODE_LFSR_RANDOM) && (newMode == MODE_LFSR_RANDOM)) {
    effects_randomColorTimer = 0xffff;
  }

 if ((effects_mode != MODE_INDEP_RANDOM) && (newMode == MODE_INDEP_RANDOM)) {
    effects_randomColorTimer = 0xffff;
  }

  effects_mode = newMode;

  Serial.printf ("Set Mode: %d\n\r", effects_mode);
}

void SetFixedColor (int newColor)
{
  effects_fixedColor = newColor;
}

void SetDirection (int newDirection)
{
  effects_direction = newDirection;
}

void SetStep (int newCycleTime)
{
  effects_step = newCycleTime;
}

void SetOffset (int newWashOffset)
{
  effects_offset = newWashOffset;
}

void SetRandomDelay (int newRandomDelay)
{
  effects_delay = newRandomDelay;
}

void SetWhiteLevel (int newWhiteLevel)
{
  effects_whiteLevel = newWhiteLevel;
}
