# Mini-proj_02
mini proj 02 AP3 F.Chatrie

convert images to video (20 img/s):
```
ffmpeg -framerate 20 -i ./output/mandelbrot_%d.ppm -c:v libx264 -pix_fmt yuv420p video.mp4
```
