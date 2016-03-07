import numpy as np 
import os

file_names = ['anno_list_mscoco_test2014', 'anno_list_mscoco_trainModelVal_m_RNN', 'anno_list_mscoco_crVal_m_RNN']

for file_name in file_names:
    npy_file_name = file_name + '.npy'
    txt_file_name = file_name + '.txt'
    if os.path.exists(txt_file_name):
        os.remove(txt_file_name)
    txt_file = open(txt_file_name, 'a')
    all_items = np.load(npy_file_name)
    print 'load ' + npy_file_name + ' and ' + 'open ' + txt_file_name + ' success!'

    keys =[]
    keys_str = '#'
    for key in all_items[0]:
        keys.append(key)
        keys_str = keys_str + str(key).strip() + '\t'
    keys_str = keys_str[:-1] + '\n' # remove the final \t and set it as \n
    txt_file.write(keys_str)
    txt_file.flush()

    for item in all_items:
        if item.has_key('id'):
            print item['id']
        temp_str = ''
        for key in keys:
            if key == 'sentences':  # deal with sentence 
                for sentence in item['sentences']:
                    sentence = ' '.join([str(word).strip() for word in sentence])
                    temp_str = temp_str + sentence + ','
                temp_str = temp_str[:-1] + '\t' # remove the last ',' ans set it as '\t'
            else:    
                temp_str = temp_str + str(item[key]).strip() + '\t'  
        temp_str = temp_str[:-1] + '\n'
        txt_file.write(temp_str)
        txt_file.flush()

    txt_file.close()
    print 'create ' + txt_file_name + ' success!'
