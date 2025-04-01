#ifndef _VGA_BALL_H
#define _VGA_BALL_H

#include <linux/ioctl.h>
#include <linux/types.h>




typedef struct {
    uint16_t x;
    uint16_t y;
} vga_ball_position_t;


typedef struct {
  unsigned char red, green, blue;
} vga_ball_color_t;
  

typedef struct {
  vga_ball_position_t position;
  vga_ball_color_t background;
} vga_ball_arg_t;

#define VGA_BALL_MAGIC 'q'

/* ioctls and their arguments */
#define VGA_BALL_WRITE_BACKGROUND _IOW(VGA_BALL_MAGIC, 1, vga_ball_arg_t)
#define VGA_BALL_READ_BACKGROUND  _IOR(VGA_BALL_MAGIC, 2, vga_ball_arg_t)
#define VGA_BALL_WRITE_POSITION _IOW('v', 2,  vga_ball_arg_t)
#define VGA_BALL_READ_POSITION  _IOR('v', 3,  vga_ball_arg_t)


#endif
