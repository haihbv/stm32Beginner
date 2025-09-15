import sys
import os
from PIL import Image

def bmp_to_rowmajor_bitmap(image_path, output_path=None, width=None, height=None):
	# Lấy tên biến từ tên file ảnh
	base = os.path.splitext(os.path.basename(image_path))[0]
	var_name = base.lower().replace('-', '_').replace(' ', '_') + '_bitmap'

	# Mở ảnh, chuyển sang 1-bit, resize nếu cần
	img = Image.open(image_path).convert('1')
	if width is not None and height is not None:
		img = img.resize((width, height))
	else:
		width, height = img.size

	pixels = img.getdata()
	byte_width = (width + 7) // 8
	data = []
	for y in range(height):
		for x_byte in range(byte_width):
			byte = 0
			for bit in range(8):
				x = x_byte * 8 + bit
				if x < width:
					pixel = pixels[y * width + x]
					if pixel == 0:  # pixel đen => bật bit
						byte |= (1 << (7 - bit))
			data.append(byte)

	# Xuất file
	if output_path is None:
		output_path = base + '.txt'
	with open(output_path, 'w', encoding='utf-8') as f:
		f.write(f'static const uint8_t {var_name}[] = {{\n')
		for i in range(0, len(data), 16):
			line = ', '.join(f'0x{b:02X}' for b in data[i:i+16])
			if i + 16 >= len(data):
				f.write(f'    {line}\n')
			else:
				f.write(f'    {line},\n')
		f.write('};\n')
	print(f'Đã xuất: {output_path} ({width}x{height}, {len(data)} bytes)')

if __name__ == '__main__':
	if len(sys.argv) < 2:
		print('Cách dùng: python bmp2oled.py <image.bmp> [output.txt] [width height]')
		sys.exit(1)
	image_path = sys.argv[1]
	output_path = None
	width = height = None
	if len(sys.argv) >= 3 and not sys.argv[2].isdigit():
		output_path = sys.argv[2]
		idx = 3
	else:
		idx = 2
	if len(sys.argv) >= idx + 2:
		width = int(sys.argv[idx])
		height = int(sys.argv[idx+1])
	bmp_to_rowmajor_bitmap(image_path, output_path, width, height)
