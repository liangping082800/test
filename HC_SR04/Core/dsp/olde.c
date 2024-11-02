#include "oled.h"     
#include  "main.h" 
#include "i2c.h"
#include "gpio.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

// OLED������ַ
#define OLED_ADDRESS 0x78

// OLED����
#define OLED_PAGE 8            // OLEDҳ��
#define OLED_ROW 8 * OLED_PAGE // OLED����
#define OLED_COLUMN 128        // OLED����

// �Դ�
uint8_t OLED_GRAM[OLED_PAGE][OLED_COLUMN];

// ========================== �ײ�ͨ�ź��� ==========================

/**
 * @brief ��OLED�������ݵĺ���
 * @param data Ҫ���͵�����
 * @param len Ҫ���͵����ݳ���
 * @return None
 * @note �˺�������ֲ������ʱ����Ҫ���� ������������ֲ������ƽ̨ʱӦ����ʵ������޸Ĵ˺���
 */
void OLED_Send(uint8_t *data, uint8_t len)
{
  HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, data, len, HAL_MAX_DELAY);
}

/**
 * @brief ��OLED����ָ��
 */
void OLED_SendCmd(uint8_t cmd)
{
  static uint8_t sendBuffer[2] = {0};
  sendBuffer[1] = cmd;
  OLED_Send(sendBuffer, 2);
}

// ========================== OLED�������� ==========================

/**
 * @brief ��ʼ��OLED (SSD1306)
 * @note �˺�������ֲ������ʱ����Ҫ���� ������������ֲ����������оƬʱӦ����ʵ������޸Ĵ˺���
 */
void OLED_Init()
{
  OLED_SendCmd(0xAE); /*�ر���ʾ display off*/

  OLED_SendCmd(0x20);
  OLED_SendCmd(0x10);

  OLED_SendCmd(0xB0);

  OLED_SendCmd(0xC8);

  OLED_SendCmd(0x00);
  OLED_SendCmd(0x10);

  OLED_SendCmd(0x40);

  OLED_SendCmd(0x81);

  OLED_SendCmd(0xDF);
  OLED_SendCmd(0xA1);

  OLED_SendCmd(0xA6);
  OLED_SendCmd(0xA8);

  OLED_SendCmd(0x3F);

  OLED_SendCmd(0xA4);

  OLED_SendCmd(0xD3);
  OLED_SendCmd(0x00);

  OLED_SendCmd(0xD5);
  OLED_SendCmd(0xF0);

  OLED_SendCmd(0xD9);
  OLED_SendCmd(0x22);

  OLED_SendCmd(0xDA);
  OLED_SendCmd(0x12);

  OLED_SendCmd(0xDB);
  OLED_SendCmd(0x20);

  OLED_SendCmd(0x8D);
  OLED_SendCmd(0x14);

  OLED_NewFrame();
  OLED_ShowFrame();

  OLED_SendCmd(0xAF); /*������ʾ display ON*/
}

/**
 * @brief ����OLED��ʾ
 */
void OLED_DisPlay_On()
{
  OLED_SendCmd(0x8D); // ��ɱ�ʹ��
  OLED_SendCmd(0x14); // ������ɱ�
  OLED_SendCmd(0xAF); // ������Ļ
}

/**
 * @brief �ر�OLED��ʾ
 */
void OLED_DisPlay_Off()
{
  OLED_SendCmd(0x8D); // ��ɱ�ʹ��
  OLED_SendCmd(0x10); // �رյ�ɱ�
  OLED_SendCmd(0xAE); // �ر���Ļ
}

/**
 * @brief ������ɫģʽ �ڵװ��ֻ�׵׺���
 * @param ColorMode ��ɫģʽCOLOR_NORMAL/COLOR_REVERSED
 * @note �˺���ֱ��������Ļ����ɫģʽ
 */
void OLED_SetColorMode(OLED_ColorMode mode)
{
  if (mode == OLED_COLOR_NORMAL)
  {
    OLED_SendCmd(0xA6); // ������ʾ
  }
  if (mode == OLED_COLOR_REVERSED)
  {
    OLED_SendCmd(0xA7); // ��ɫ��ʾ
  }
}

// ========================== �Դ�������� ==========================

/**
 * @brief ����Դ� �����µ�һ֡
 */
void OLED_NewFrame()
{
  memset(OLED_GRAM, 0, sizeof(OLED_GRAM));
}

/**
 * @brief ����ǰ�Դ���ʾ����Ļ��
 * @note �˺�������ֲ������ʱ����Ҫ���� ������������ֲ����������оƬʱӦ����ʵ������޸Ĵ˺���
 */
void OLED_ShowFrame()
{
  static uint8_t sendBuffer[OLED_COLUMN + 1];
  sendBuffer[0] = 0x40;
  for (uint8_t i = 0; i < OLED_PAGE; i++)
  {
    OLED_SendCmd(0xB0 + i); // ����ҳ��ַ
    OLED_SendCmd(0x00);     // �����е�ַ��4λ
    OLED_SendCmd(0x10);     // �����е�ַ��4λ
    memcpy(sendBuffer + 1, OLED_GRAM[i], OLED_COLUMN);
    OLED_Send(sendBuffer, OLED_COLUMN + 1);
  }
}

/**
 * @brief ����һ�����ص�
 * @param x ������
 * @param y ������
 * @param color ��ɫ
 */
void OLED_SetPixel(uint8_t x, uint8_t y, OLED_ColorMode color)
{
  if (x >= OLED_COLUMN || y >= OLED_ROW)
    return;
  if (!color)
  {
    OLED_GRAM[y / 8][x] |= 1 << (y % 8);
  }
  else
  {
    OLED_GRAM[y / 8][x] &= ~(1 << (y % 8));
  }
}

/**
 * @brief �����Դ���һ�ֽ����ݵ�ĳ��λ
 * @param page ҳ��ַ
 * @param column �е�ַ
 * @param data ����
 * @param start ��ʼλ
 * @param end ����λ
 * @param color ��ɫ
 * @note �˺������Դ��е�ĳһ�ֽڵĵ�startλ����endλ����Ϊ��data��ͬ
 * @note start��end�ķ�ΧΪ0-7, start����С�ڵ���end
 * @note �˺�����OLED_SetByte_Fine���������ڴ˺���ֻ�������Դ��е�ĳһ��ʵ�ֽ�
 */
void OLED_SetByte_Fine(uint8_t page, uint8_t column, uint8_t data, uint8_t start, uint8_t end, OLED_ColorMode color)
{
  static uint8_t temp;
  if (page >= OLED_PAGE || column >= OLED_COLUMN)
    return;
  if (color)
    data = ~data;

  temp = data | (0xff << (end + 1)) | (0xff >> (8 - start));
  OLED_GRAM[page][column] &= temp;
  temp = data & ~(0xff << (end + 1)) & ~(0xff >> (8 - start));
  OLED_GRAM[page][column] |= temp;
  // ʹ��OLED_SetPixelʵ��
  // for (uint8_t i = start; i <= end; i++) {
  //   OLED_SetPixel(column, page * 8 + i, !((data >> i) & 0x01));
  // }
}

/**
 * @brief �����Դ��е�һ�ֽ�����
 * @param page ҳ��ַ
 * @param column �е�ַ
 * @param data ����
 * @param color ��ɫ
 * @note �˺������Դ��е�ĳһ�ֽ�����Ϊdata��ֵ
 */
void OLED_SetByte(uint8_t page, uint8_t column, uint8_t data, OLED_ColorMode color)
{
  if (page >= OLED_PAGE || column >= OLED_COLUMN)
    return;
  if (color)
    data = ~data;
  OLED_GRAM[page][column] = data;
}

/**
 * @brief �����Դ��е�һ�ֽ����ݵ�ĳ��λ
 * @param x ������
 * @param y ������
 * @param data ����
 * @param len λ��
 * @param color ��ɫ
 * @note �˺������Դ��д�(x,y)��ʼ������lenλ����Ϊ��data��ͬ
 * @note len�ķ�ΧΪ1-8
 * @note �˺�����OLED_SetByte_Fine���������ڴ˺����ĺ��������������������Ϊ��λ��, ���ܳ��ֿ�������ʵ�ֽڵ����(��ҳ)
 */
void OLED_SetBits_Fine(uint8_t x, uint8_t y, uint8_t data, uint8_t len, OLED_ColorMode color)
{
  uint8_t page = y / 8;
  uint8_t bit = y % 8;
  if (bit + len > 8)
  {
    OLED_SetByte_Fine(page, x, data << bit, bit, 7, color);
    OLED_SetByte_Fine(page + 1, x, data >> (8 - bit), 0, len + bit - 1 - 8, color);
  }
  else
  {
    OLED_SetByte_Fine(page, x, data << bit, bit, bit + len - 1, color);
  }
  // ʹ��OLED_SetPixelʵ��
  // for (uint8_t i = 0; i < len; i++) {
  //   OLED_SetPixel(x, y + i, !((data >> i) & 0x01));
  // }
}

/**
 * @brief �����Դ���һ�ֽڳ��ȵ�����
 * @param x ������
 * @param y ������
 * @param data ����
 * @param color ��ɫ
 * @note �˺������Դ��д�(x,y)��ʼ������8λ����Ϊ��data��ͬ
 * @note �˺�����OLED_SetByte���������ڴ˺����ĺ��������������������Ϊ��λ��, ���ܳ��ֿ�������ʵ�ֽڵ����(��ҳ)
 */
void OLED_SetBits(uint8_t x, uint8_t y, uint8_t data, OLED_ColorMode color)
{
  uint8_t page = y / 8;
  uint8_t bit = y % 8;
  OLED_SetByte_Fine(page, x, data << bit, bit, 7, color);
  if (bit)
  {
    OLED_SetByte_Fine(page + 1, x, data >> (8 - bit), 0, bit - 1, color);
  }
}

/**
 * @brief ����һ���Դ�����
 * @param x ��ʼ������
 * @param y ��ʼ������
 * @param data ���ݵ���ʼ��ַ
 * @param w ���
 * @param h �߶�
 * @param color ��ɫ
 * @note �˺������Դ��д�(x,y)��ʼ��w*h����������Ϊdata�е�����
 * @note data������Ӧ�ò�������ʽ����
 */
void OLED_SetBlock(uint8_t x, uint8_t y, const uint8_t *data, uint8_t w, uint8_t h, OLED_ColorMode color)
{
  uint8_t fullRow = h / 8; // ����������
  uint8_t partBit = h % 8; // ���������ֽ��е���Чλ��
  for (uint8_t i = 0; i < w; i++)
  {
    for (uint8_t j = 0; j < fullRow; j++)
    {
      OLED_SetBits(x + i, y + j * 8, data[i + j * w], color);
    }
  }
  if (partBit)
  {
    uint16_t fullNum = w * fullRow; // �������ֽ���
    for (uint8_t i = 0; i < w; i++)
    {
      OLED_SetBits_Fine(x + i, y + (fullRow * 8), data[fullNum + i], partBit, color);
    }
  }
  // ʹ��OLED_SetPixelʵ��
  // for (uint8_t i = 0; i < w; i++) {
  //   for (uint8_t j = 0; j < h; j++) {
  //     for (uint8_t k = 0; k < 8; k++) {
  //       if (j * 8 + k >= h) break; // ��ֹԽ��(���������ֽ�
  //       OLED_SetPixel(x + i, y + j * 8 + k, !((data[i + j * w] >> k) & 0x01));
  //     }
  //   }
  // }
}

// ========================== ͼ�λ��ƺ��� ==========================
/**
 * @brief ����һ���߶�
 * @param x1 ��ʼ�������
 * @param y1 ��ʼ��������
 * @param x2 ��ֹ�������
 * @param y2 ��ֹ��������
 * @param color ��ɫ
 * @note �˺���ʹ��Bresenham�㷨�����߶�
 */
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, OLED_ColorMode color)
{
  static uint8_t temp = 0;
  if (x1 == x2)
  {
    if (y1 > y2)
    {
      temp = y1;
      y1 = y2;
      y2 = temp;
    }
    for (uint8_t y = y1; y <= y2; y++)
    {
      OLED_SetPixel(x1, y, color);
    }
  }
  else if (y1 == y2)
  {
    if (x1 > x2)
    {
      temp = x1;
      x1 = x2;
      x2 = temp;
    }
    for (uint8_t x = x1; x <= x2; x++)
    {
      OLED_SetPixel(x, y1, color);
    }
  }
  else
  {
    // Bresenhamֱ���㷨
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    int16_t ux = ((dx > 0) << 1) - 1;
    int16_t uy = ((dy > 0) << 1) - 1;
    int16_t x = x1, y = y1, eps = 0;
    dx = abs(dx);
    dy = abs(dy);
    if (dx > dy)
    {
      for (x = x1; x != x2; x += ux)
      {
        OLED_SetPixel(x, y, color);
        eps += dy;
        if ((eps << 1) >= dx)
        {
          y += uy;
          eps -= dx;
        }
      }
    }
    else
    {
      for (y = y1; y != y2; y += uy)
      {
        OLED_SetPixel(x, y, color);
        eps += dx;
        if ((eps << 1) >= dy)
        {
          x += ux;
          eps -= dy;
        }
      }
    }
  }
}

/**
 * @brief ����һ������
 * @param x ��ʼ�������
 * @param y ��ʼ��������
 * @param w ���ο��
 * @param h ���θ߶�
 * @param color ��ɫ
 */
void OLED_DrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color)
{
  OLED_DrawLine(x, y, x + w, y, color);
  OLED_DrawLine(x, y + h, x + w, y + h, color);
  OLED_DrawLine(x, y, x, y + h, color);
  OLED_DrawLine(x + w, y, x + w, y + h, color);
}

/**
 * @brief ����һ��������
 * @param x ��ʼ�������
 * @param y ��ʼ��������
 * @param w ���ο��
 * @param h ���θ߶�
 * @param color ��ɫ
 */
void OLED_DrawFilledRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color)
{
  for (uint8_t i = 0; i < h; i++)
  {
    OLED_DrawLine(x, y + i, x + w, y + i, color);
  }
}

/**
 * @brief ����һ��������
 * @param x1 ��һ���������
 * @param y1 ��һ����������
 * @param x2 �ڶ����������
 * @param y2 �ڶ�����������
 * @param x3 �������������
 * @param y3 ��������������
 * @param color ��ɫ
 */
void OLED_DrawTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, OLED_ColorMode color)
{
  OLED_DrawLine(x1, y1, x2, y2, color);
  OLED_DrawLine(x2, y2, x3, y3, color);
  OLED_DrawLine(x3, y3, x1, y1, color);
}

/**
 * @brief ����һ�����������
 * @param x1 ��һ���������
 * @param y1 ��һ����������
 * @param x2 �ڶ����������
 * @param y2 �ڶ�����������
 * @param x3 �������������
 * @param y3 ��������������
 * @param color ��ɫ
 */
void OLED_DrawFilledTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, OLED_ColorMode color)
{
  uint8_t a = 0, b = 0, y = 0, last = 0;
  if (y1 > y2)
  {
    a = y2;
    b = y1;
  }
  else
  {
    a = y1;
    b = y2;
  }
  y = a;
  for (; y <= b; y++)
  {
    if (y <= y3)
    {
      OLED_DrawLine(x1 + (y - y1) * (x2 - x1) / (y2 - y1), y, x1 + (y - y1) * (x3 - x1) / (y3 - y1), y, color);
    }
    else
    {
      last = y - 1;
      break;
    }
  }
  for (; y <= b; y++)
  {
    OLED_DrawLine(x2 + (y - y2) * (x3 - x2) / (y3 - y2), y, x1 + (y - last) * (x3 - x1) / (y3 - last), y, color);
  }
}

/**
 * @brief ����һ��Բ
 * @param x Բ�ĺ�����
 * @param y Բ��������
 * @param r Բ�뾶
 * @param color ��ɫ
 * @note �˺���ʹ��Bresenham�㷨����Բ
 */
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r, OLED_ColorMode color)
{
  int16_t a = 0, b = r, di = 3 - (r << 1);
  while (a <= b)
  {
    OLED_SetPixel(x - b, y - a, color);
    OLED_SetPixel(x + b, y - a, color);
    OLED_SetPixel(x - a, y + b, color);
    OLED_SetPixel(x - b, y - a, color);
    OLED_SetPixel(x - a, y - b, color);
    OLED_SetPixel(x + b, y + a, color);
    OLED_SetPixel(x + a, y - b, color);
    OLED_SetPixel(x + a, y + b, color);
    OLED_SetPixel(x - b, y + a, color);
    a++;
    if (di < 0)
    {
      di += 4 * a + 6;
    }
    else
    {
      di += 10 + 4 * (a - b);
      b--;
    }
    OLED_SetPixel(x + a, y + b, color);
  }
}

/**
 * @brief ����һ�����Բ
 * @param x Բ�ĺ�����
 * @param y Բ��������
 * @param r Բ�뾶
 * @param color ��ɫ
 * @note �˺���ʹ��Bresenham�㷨����Բ
 */
void OLED_DrawFilledCircle(uint8_t x, uint8_t y, uint8_t r, OLED_ColorMode color)
{
  int16_t a = 0, b = r, di = 3 - (r << 1);
  while (a <= b)
  {
    for (int16_t i = x - b; i <= x + b; i++)
    {
      OLED_SetPixel(i, y + a, color);
      OLED_SetPixel(i, y - a, color);
    }
    for (int16_t i = x - a; i <= x + a; i++)
    {
      OLED_SetPixel(i, y + b, color);
      OLED_SetPixel(i, y - b, color);
    }
    a++;
    if (di < 0)
    {
      di += 4 * a + 6;
    }
    else
    {
      di += 10 + 4 * (a - b);
      b--;
    }
  }
}

/**
 * @brief ����һ����Բ
 * @param x ��Բ���ĺ�����
 * @param y ��Բ����������
 * @param a ��Բ����
 * @param b ��Բ����
 */
void OLED_DrawEllipse(uint8_t x, uint8_t y, uint8_t a, uint8_t b, OLED_ColorMode color)
{
  int xpos = 0, ypos = b;
  int a2 = a * a, b2 = b * b;
  int d = b2 + a2 * (0.25 - b);
  while (a2 * ypos > b2 * xpos)
  {
    OLED_SetPixel(x + xpos, y + ypos, color);
    OLED_SetPixel(x - xpos, y + ypos, color);
    OLED_SetPixel(x + xpos, y - ypos, color);
    OLED_SetPixel(x - xpos, y - ypos, color);
    if (d < 0)
    {
      d = d + b2 * ((xpos << 1) + 3);
      xpos += 1;
    }
    else
    {
      d = d + b2 * ((xpos << 1) + 3) + a2 * (-(ypos << 1) + 2);
      xpos += 1, ypos -= 1;
    }
  }
  d = b2 * (xpos + 0.5) * (xpos + 0.5) + a2 * (ypos - 1) * (ypos - 1) - a2 * b2;
  while (ypos > 0)
  {
    OLED_SetPixel(x + xpos, y + ypos, color);
    OLED_SetPixel(x - xpos, y + ypos, color);
    OLED_SetPixel(x + xpos, y - ypos, color);
    OLED_SetPixel(x - xpos, y - ypos, color);
    if (d < 0)
    {
      d = d + b2 * ((xpos << 1) + 2) + a2 * (-(ypos << 1) + 3);
      xpos += 1, ypos -= 1;
    }
    else
    {
      d = d + a2 * (-(ypos << 1) + 3);
      ypos -= 1;
    }
  }
}

/**
 * @brief ����һ��ͼƬ
 * @param x ��ʼ�������
 * @param y ��ʼ��������
 * @param img ͼƬ
 * @param color ��ɫ
 */
void OLED_DrawImage(uint8_t x, uint8_t y, const Image *img, OLED_ColorMode color)
{
  OLED_SetBlock(x, y, img->data, img->w, img->h, color);
}

// ================================ ���ֻ��� ================================

/**
 * @brief ����һ��ASCII�ַ�
 * @param x ��ʼ�������
 * @param y ��ʼ��������
 * @param ch �ַ�
 * @param font ����
 * @param color ��ɫ
 */
void OLED_PrintASCIIChar(uint8_t x, uint8_t y, char ch, const ASCIIFont *font, OLED_ColorMode color)
{
  OLED_SetBlock(x, y, font->chars + (ch - ' ') * (((font->h + 7) / 8) * font->w), font->w, font->h, color);
}

/**
 * @brief ����һ��ASCII�ַ���
 * @param x ��ʼ�������
 * @param y ��ʼ��������
 * @param str �ַ���
 * @param font ����
 * @param color ��ɫ
 */
void OLED_PrintASCIIString(uint8_t x, uint8_t y, char *str, const ASCIIFont *font, OLED_ColorMode color)
{
  uint8_t x0 = x;
  while (*str)
  {
    OLED_PrintASCIIChar(x0, y, *str, font, color);
    x0 += font->w;
    str++;
  }
}

/**
 * @brief ��ȡUTF-8������ַ�����
 */
uint8_t _OLED_GetUTF8Len(char *string)
{
  if ((string[0] & 0x80) == 0x00)
  {
    return 1;
  }
  else if ((string[0] & 0xE0) == 0xC0)
  {
    return 2;
  }
  else if ((string[0] & 0xF0) == 0xE0)
  {
    return 3;
  }
  else if ((string[0] & 0xF8) == 0xF0)
  {
    return 4;
  }
  return 0;
}

/**
 * @brief �����ַ���
 * @param x ��ʼ�������
 * @param y ��ʼ��������
 * @param str �ַ���
 * @param font ����
 * @param color ��ɫ
 *
 * @note Ϊ��֤�ַ����е����Ļᱻ�Զ�ʶ�𲢻���, ��:
 * 1. �������ַ�������ΪUTF-8
 * 2. ʹ�ò����ɶ�LEDȡģ����������ģ(https://led.baud-dance.com)
 */
/**
 * @brief �����ַ���
 * @param x ��ʼ�������
 * @param y ��ʼ��������
 * @param str �ַ���
 * @param font ����
 * @param color ��ɫ
 *
 * @note Ϊ��֤�ַ����е����Ļᱻ�Զ�ʶ�𲢻���, ��:
 * 1. �������ַ�������ΪUTF-8
 * 2. ʹ�ò����ɶ�LEDȡģ����������ģ(https://led.baud-dance.com)
 */
void OLED_PrintString(uint8_t x, uint8_t y, char *str, const Font *font, OLED_ColorMode color)
{
  uint16_t i = 0;                                       // �ַ�������
  uint8_t oneLen = (((font->h + 7) / 8) * font->w) + 4; // һ����ģռ�����ֽ�
  uint8_t found;                                        // �Ƿ��ҵ���ģ
  uint8_t utf8Len;                                      // UTF-8���볤��
  uint8_t *head;                                        // ��ģͷָ��
  while (str[i])
  {
    found = 0;
    utf8Len = _OLED_GetUTF8Len(str + i);
    if (utf8Len == 0)
      break; // �������UTF-8����

    // Ѱ���ַ�  TODO �Ż������㷨, ���ֲ��һ���hash
    for (uint8_t j = 0; j < font->len; j++)
    {
      head = (uint8_t *)(font->chars) + (j * oneLen);
      if (memcmp(str + i, head, utf8Len) == 0)
      {
        OLED_SetBlock(x, y, head + 4, font->w, font->h, color);
        // �ƶ����
        x += font->w;
        i += utf8Len;
        found = 1;
        break;
      }
    }

    // ��δ�ҵ���ģ,��ΪASCII�ַ�, ��ȱʡ��ʾASCII�ַ�
    if (found == 0)
    {
      if (utf8Len == 1)
      {
        OLED_PrintASCIIChar(x, y, str[i], font->ascii, color);
        // �ƶ����
        x += font->ascii->w;
        i += utf8Len;
      }
      else
      {
        OLED_PrintASCIIChar(x, y, ' ', font->ascii, color);
        x += font->ascii->w;
        i += utf8Len;
      }
    }
  }
}
