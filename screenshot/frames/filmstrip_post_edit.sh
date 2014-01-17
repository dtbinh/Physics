#./filmstrip_post_edit.sh filmstrip_file_path only_delete_last_image(0)/also_put_first_image_from_aux_file(1)/only_put_last_image_from_aux_file(2) real_num_images filmstrip_width filmstrip_height
#./filmstrip_post_edit.sh filmstrip.png 0 11 3300 400

if (( $2 == 0 )); then
 convert $1 -gravity northwest -crop $(( (($3-1)*$4)/$3 ))x$5+0+0 +repage `basename $1 .png`_post_edited_0.png
fi

if (( $2 == 1 )); then
 convert $1 -gravity northwest -crop $(( (($3-1)*$4)/$3 ))x$5+0+0 +repage `basename $1 .png`_post_edited_0_temp.png
 convert `basename $1 .png`_first_image.png -gravity northwest -crop $(( $4/$3 ))x$5+0+0 +repage `basename $1 .png`_first_image_temp.png
 montage `basename $1 .png`_first_image_temp.png `basename $1 .png`_post_edited_0_temp.png -background white -geometry +0+0 `basename $1 .png`_post_edited_1.png
 rm `basename $1 .png`_post_edited_0_temp.png
 rm `basename $1 .png`_first_image_temp.png
fi

if (( $2 == 2 )); then
 convert `basename $1 .png`_last_image.png -gravity northwest -crop $(( $4/$3 ))x$5+$(( (($3-1)*$4)/$3 ))+0 +repage `basename $1 .png`_last_image_temp.png
 montage $1 `basename $1 .png`_last_image_temp.png -background white -geometry +0+0 `basename $1 .png`_post_edited_2.png
 rm `basename $1 .png`_last_image_temp.png
fi

