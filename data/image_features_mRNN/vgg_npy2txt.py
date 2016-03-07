import numpy as np 
import os

npy_name = 'VGG_feat_o_dct_mscoco_2014.npy'
txt_name = 'VGG_feat_o_dct_mscoco_2014.txt'

features = np.load(npy_name).tolist()
print 'load data success!'
if os.path.exists(txt_name):
    os.remove(txt_name)
txt_file = open(txt_name, 'a')

for id in features:
    print id
    temp = str(id) + '\t'
    for feature in features[id]:
        temp = temp + str(feature) + ','
    temp = temp[:-1]  #remove the last ','
    temp += '\n'
    txt_file.write(temp)
    txt_file.flush()

txt_file.close()
