/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int vga_ball_fd;

/* Read and print the background color */
void print_background_color() {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_READ_BACKGROUND) failed");
      return;
  }
  printf("%02x %02x %02x\n",
	 vla.background.red, vla.background.green, vla.background.blue);
}

/* Set the background color */
void set_background_color(const vga_ball_color_t *c)
{
  vga_ball_arg_t vla;
  vla.background = *c;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
      return;
  }
}

int main()
{
  vga_ball_arg_t vla;
  int i;
  static const char filename[] = "/dev/vga_ball";

  static const vga_ball_color_t colors[] = {
    { 0xff, 0x00, 0x00 }, /* Red */
    { 0x00, 0xff, 0x00 }, /* Green */
    { 0x00, 0x00, 0xff }, /* Blue */
    { 0xff, 0xff, 0x00 }, /* Yellow */
    { 0x00, 0xff, 0xff }, /* Cyan */
    { 0xff, 0x00, 0xff }, /* Magenta */
    { 0x80, 0x80, 0x80 }, /* Gray */
    { 0x00, 0x00, 0x00 }, /* Black */
    { 0xff, 0xff, 0xff }  /* White */
  };

# define COLORS 9

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  printf("initial state: ");
  print_background_color();
  int pos_x = 100;
  int pos_y = 100;
  int vel_x = 1;
  int vel_y = 1;
  int screen_width = 640;
  int screen_height = 480;
  int r = 20;
  int cnt = 0;
    while(1){
      if (pos_x <= r && vel_x == -1){
        vel_x = 1;
	      cnt += 1;
      }
      if (pos_y <= r && vel_y == -1){
        vel_y = 1;
	      cnt += 1;
      }
      if (pos_x >= screen_width - r && vel_x == 1){
        vel_x = -1;
	      cnt += 1;
      }
      if (pos_y >= screen_height - r && vel_y == 1){
        vel_y = -1;
	      cnt += 1;
      }
      pos_x += vel_x;
      pos_y += vel_y;
      // if (pos_x > screen_width){
      //   pos_x = 0;
      // }
      // if (pos_y > screen_height){
      //   pos_y = 0;
      // }
      uint32_t encoded_red = ((pos_x & 0x3FF) << 22) | colors[cnt % COLORS].red;
      uint32_t encoded_green = ((pos_y & 0x3FF) << 22) | colors[cnt % COLORS].green;
      uint32_t encoded_blue = (colors[(cnt + 1) % COLORS].red << 24) | (colors[(cnt + 1) % COLORS].green << 16) | (colors[(cnt + 1) % COLORS].blue << 8) | colors[cnt % COLORS].blue;
  
      vga_ball_color_t new_color = {
          .red = encoded_red,
          .green = encoded_green,
	  .blue = encoded_blue
          // .blue = colors[cnt % COLORS].blue
      };
  
      set_background_color(&new_color);
      print_background_color();

      usleep(40000);
    }




  for (i = 0 ; i < 24 ; i++) {
    set_background_color(&colors[i % COLORS ]);
    print_background_color();
    usleep(400000);
  }
  
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
