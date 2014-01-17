mkdir cropped
#crop
 for f in images/*.png ; do convert $f -gravity northwest -crop $3x$4+$1+$2 +repage cropped/`basename $f`; done
