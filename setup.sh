#! /bin/sh

# download corpus-clean file
echo "Start downloading corpus-clean file"
wget -O ./files/corpora/mscoco/corpus-cleaned.txt https://drive.google.com/open?id=0BwFktEM1WILoclMxX3ZBQUhlbGM
echo "Finish downloading corpus-clean file"

# download vectors.bin
echo "Start downloading vectors.bin"
wget -O ./files/corpora/mscoco/vectors.bin https://drive.google.com/open?id=0BwFktEM1WILoSzk5R0Nka2kzYlE
echo "Finish downloading vectors.bin"

# download mscoco_anno_files.zip
echo "Start downloading mscoco_anno_files.zip"
wget -O ./mscoco_anno_files.zip https://drive.google.com/open?id=0BwFktEM1WILoaGJSUnJXV1pkUk0
echo "Finish downloading mscoco_anno_files.zip"

# download VGG_feat_o_dct_mscoco_2014.zip
echo "Start downloading VGG_feat_o_dct_mscoco_2014.zip"
echo "This file is about 2GB, so it should take a long time to download it, be patient please."
wget -O ./VGG_feat_o_dct_mscoco_2014.zip https://drive.google.com/open?id=0BwFktEM1WILoUDgxZmlVeTZCb1k
echo "Finish downloading VGG_feat_o_dct_mscoco_2014.zip"

# downloading VGG_feat_mRNN_refine_dct_mscoco_2014.zip
echo "Start downloading VGG_feat_mRNN_refine_dct_mscoco_2014.zip"
echo "This file is about 1GB, so it should take a long time to download it, be patient please."
wget -O ./VGG_feat_mRNN_refine_dct_mscoco_2014.zip https://drive.google.com/open?id=0BwFktEM1WILobnJxTHltWnl6Tk0
echo "Finish downloading VGG_feat_mRNN_refine_dct_mscoco_2014.zip"

# unzip the mscoco_anno_files.zip
unzip mscoco_anno_files.zip -d ./data/mscoco_anno_files

# unzip VGG_feat_o_dct_mscoco_2014.zip
unzip VGG_feat_o_dct_mscoco_2014.zip -d ./data/image_features_mRNN

# unzip VGG_feat_mRNN_refine_dct_mscoco_2014.zip 
unzip VGG_feat_mRNN_refine_dct_mscoco_2014.zip -d ./data/image_features_mRNN

rm mscoco_anno_files.zip VGG_feat_o_dct_mscoco_2014.zip VGG_feat_mRNN_refine_dct_mscoco_2014.zip
