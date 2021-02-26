"""
python.py
-------------

HowTo?
	python3 python.py -p=<path_to_image> -s=<to_show> -f=<to_file>

	* path_to_image - (Required) absolute path to some image on the local computer.
	* to_show - (Option) boolean value. Should be either True or False.
	* to_file - (Option) boolean value. Should be either True or False.

* If <to_file> is True, it saves to image to local file called `lena_as_file.out`.
"""

import numpy
import argparse
from PIL import Image


def __lists_to_lines(np_arr):
	def __single_list_to_str(lst, end_of_line=True):
		s = ""
		for i, e in enumerate(lst):
			s += str(e) + " " if i != len(lst) - 1 else str(e)
		s += '\n' if end_of_line else ''
		return s

	lines = []
	for ind, line in enumerate(list(np_arr)):
		lines.append(__single_list_to_str(list(line))) if ind != len(list(np_arr)) - 1 \
			else lines.append(__single_list_to_str(list(line), False))

	return lines


def __save_to_file(img):
	as_arr = numpy.asarray(img)
	fp = open("image.out", 'w')
	fp.writelines(__lists_to_lines(as_arr))
	fp.close()


if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('-p', action='store', required=True, dest='img_path', help="""(str) The path to the image.""")
	parser.add_argument('-s', action='store', dest='show', type=bool, default=False,
						help="""(bool) Display image, 1 for True, 0 for False.""")
	parser.add_argument('-f', action='store', dest='to_file', type=bool, default=False,
						help="""(bool) Convert to file. If its true, saves to local dir""")
	args = parser.parse_args()

	img = Image.open(args.img_path).convert('L').resize((128, 128))

	if args.show:
		img.show(img)

	if args.to_file:
		__save_to_file(img)
