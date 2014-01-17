#./crop_montage.sh pos_x(pixels) pos_y(pixels) dim_x(pixels) dim_y(pixels) white_space_between_images(pixels)
#./crop_montage.sh 50 20 300 400 5 (dimension 300x400)

mkdir cropped
#crop
 for f in images/*.png ; do convert $f -gravity northwest -crop $3x$4+$1+$2 +repage cropped/`basename $f`; done
#montage
 montage cropped/*.png -background white -geometry +$5+0 filmstrip.png
rm -r cropped

