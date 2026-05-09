#ifndef PIC_H
#define PIC_H

void pic_remap(void);
void pic_mask(unsigned char mask1, unsigned char mask2);
void pic_acknowledge(unsigned char interrupt);

#endif
