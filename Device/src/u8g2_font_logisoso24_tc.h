/*
  .\otf2bdf.exe -r 72 -p 34 .\Logisoso.ttf -o Logisoso2.bdf
  .\bdfconv.exe -f 1 -m "67,176" -b 0 -n u8g2_font_logisoso24_tc -o u8g2_font_logisoso24_tc.h .\Logisoso2.bdf
*/

#include <U8g2lib.h>

/*
  Fontname: -FreeType-Logisoso-Medium-R-Normal--34-340-72-72-P-42-ISO10646-1
  Copyright: Created by Mathieu Gabiot with FontForge 2.0 (http://fontforge.sf.net) - Brussels - 2009
  Glyphs: 2/527
  BBX Build Mode: 0
*/
const uint8_t u8g2_font_logisoso24_tc[68] U8G2_FONT_SECTION("u8g2_font_logisoso24_tc") = 
  "\2\0\4\4\4\5\3\6\6\14\34\2\0\30\372\27\0\0\0\0\0\0'C\31\214\15\302S\14\215\12"
  "B#\206Y\71\377g\306F\14\42\241\6\31!\0\260\14el\247\61\342\4\211\23#\0\0\0\0\4"
  "\377\377\0";
