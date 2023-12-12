from PIL import Image
im = Image.open('flower.bmp', 'r')
width, height = im.size
pixel_values = list(im.getdata())
with open("pixels.txt", "w") as f:
    for p in pixel_values:
        #r = str(p[0])
        #g = str(p[1])
        #b = str(p[2])
        #f.write(" ".join((r,g,b)))
        #f.write("\n")
        r = p[0]
        g = p[1]
        b = p[2]

        f.write('\n{:02x}{:02x}{:02x}'.format(r, g, b))
