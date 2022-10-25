//Include standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//Include custom libraries
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"
#define accuracy 70
//Compare function
int absDiff(unsigned char* background, unsigned char* foreground,int index_height, int index_width, int height, int width, int channel){
  float sum = 0;
  for (int k=0; k<channel; k++){
    if (k==1){sum += 3 * abs(background[index_height*width*channel + index_width*channel+k]-foreground[index_height*width*channel + index_width*channel+k]);}
    if (k!=1){sum += 1 * abs(background[index_height*width*channel + index_width*channel+k]-foreground[index_height*width*channel + index_width*channel+k]);}
  } 
  return sum/5;
}
//Masking function
void mask_image(unsigned char* background, unsigned char* foreground, unsigned char* forecaster, int width, int height, int channel){
  for (int i=0; i<height; i++){
    for (int j=0; j<width; j++){
      if( absDiff(background,foreground,i,j,height,width,channel) > accuracy ){
        for (int k=0; k<channel; k++){
          forecaster[i*width*channel + j*channel+k] = foreground[i*width*channel + j*channel+k];
        }
      }
    }
  }
}
//Main function
int main(){
  int std_width, std_height, std_channel;
  int width, height, channel;
  char path_background[] = "./images/background.png";
  char path_foreground[] = "./images/foreground.png";
  char path_forecaster[] = "./images/weather_forecast-resized.jpg";
  char path_output[] = "./images/output.png";

  unsigned char *background = stbi_load(path_background,&width,&height,&channel,0);
  printf("Background\nWidth: %d\nHeight: %d\nChannel: %d\n\n", width,height,channel);
  std_width = width; std_height = height; std_channel = channel;
  if (background == NULL){
    printf("Background not found!"); exit(1);
  }

  unsigned char *foreground = stbi_load(path_foreground,&width,&height,&channel,0);
  printf("Foreground\nWidth: %d\nHeight: %d\nChannel: %d\n\n", width,height,channel);
  if ((std_width!=width) || (std_height!=height) || (std_channel!=channel)){
    printf("Unmatching size!"); exit(1);
  }

  unsigned char *forecaster = stbi_load(path_forecaster,&width,&height,&channel,0);
  printf("Forecaster\nWidth: %d\nHeight: %d\nChannel: %d\n\n", width,height,channel);
  if ((std_width!=width) || (std_height!=height) || (std_channel!=channel)){
    printf("Unmatching size!"); exit(1);
  }

  mask_image(background,foreground,forecaster,width,height,channel);
  stbi_write_png(path_output,width,height,channel,forecaster,width*channel);
}