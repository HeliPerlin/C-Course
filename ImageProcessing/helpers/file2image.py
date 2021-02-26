"""
file2image.py
-------------

HowTo?
	python3 file2image.py -p=<path_to_file>

	* path_to_file - (Required) absolute path to some image on the local computer.

* 	It saves to image to local file called `lena_from_file.jpg'.
"""
import numpy
import argparse
from PIL import Image


def __limit(num):
    if num < 0:
        return 0
    elif num > 255:
        return 255
    else:
        return num


def __parse_file(file_path):
    file = open(file_path, "r")
    lines = file.readlines()
    lists = []
    for line in lines:
        new_list = [__limit(int(elem)) for elem in line.split(' ')]
        lists.append(new_list)
    return lists


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', action='store', required=True, dest='file_path', help="""(str) The path to the file.""")
    args = parser.parse_args()

    file_as_list_of_lists = __parse_file(args.file_path)
    as_np_array = numpy.array(file_as_list_of_lists)
    im = Image.fromarray(as_np_array)
    im = im.convert('L')
    im = im.resize((128, 128))
    im.save("image.jpg", format="JPEG")
