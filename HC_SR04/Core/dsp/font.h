#ifndef __font_H__
#define __font_H__

#ifdef __cplusplus

extern "C" {

#endif

#include "stdint.h"
#include "string.h"
typedef struct ASCIIFont {
  uint8_t h;
  uint8_t w;
  uint8_t *chars;
} ASCIIFont;

/*�����С����*/
extern const ASCIIFont afont8x6;
extern const ASCIIFont afont12x6;
extern const ASCIIFont afont16x8;
extern const ASCIIFont afont24x12;



/**
 * @brief ����ṹ��
 * @note  �ֿ�ǰ4�ֽڴ洢utf8���� ʣ���ֽڴ洢��ģ����
 * @note �ֿ����ݿ���ʹ�ò����ɶ�LEDȡģ��������
 */
typedef struct Font {
  uint8_t h;              // �ָ߶�
  uint8_t w;              // �ֿ��
  const uint8_t *chars;   // �ֿ� �ֿ�ǰ4�ֽڴ洢utf8���� ʣ���ֽڴ洢��ģ����
  uint8_t len;            // �ֿⳤ�� ����256�����Ϊuint16_t
  const ASCIIFont *ascii; // ȱʡASCII���� ���ֿ���û�ж�Ӧ�ַ�����Ҫ��ʾASCII�ַ�ʱʹ��
} Font;

extern const Font font16x16;

/**
 * @brief ͼƬ�ṹ��
 * @note  ͼƬ���ݿ���ʹ�ò����ɶ�LEDȡģ��������
 */
typedef struct Image {
  uint8_t w;           // ͼƬ���
  uint8_t h;           // ͼƬ�߶�
  const uint8_t *data; // ͼƬ����
} Image;

/*ͼƬ����*/

extern const Image ikun_Img;
extern const Image ButlerImg;
extern const Image funImg;
extern const Image funxImg;
extern const Image tImg ;
extern const Image hshiduImg;
extern const Image cahjsImg;
extern const Image eeeImg;
#ifdef __cplusplus

}

#endif

#endif /*__font_H__ */
