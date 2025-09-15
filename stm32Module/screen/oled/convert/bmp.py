import sys
import struct

# Lấy tên file từ tham số dòng lệnh
file_name = sys.argv[1]                     # VD: "hinh.bmp"
file_name = file_name.split(".")[0]         # bỏ phần mở rộng ".bmp"

with open(f'{file_name}.bmp', 'rb') as bmp:
    # offset: vị trí dữ liệu pixel bắt đầu
    bmp.seek(10, 0)
    offset = struct.unpack('<I', bmp.read(4))[0]
    print("Offset:", offset)

    # Width và Height (unsigned 32-bit)
    bmp.seek(18, 0)
    bmp_width = struct.unpack('<I', bmp.read(4))[0]
    bmp_height = struct.unpack('<I', bmp.read(4))[0]
    print("Width x Height:", bmp_width, "x", bmp_height)

    # Bits per pixel (unsigned 16-bit)
    bmp.seek(28, 0)
    bits_per_pixel = struct.unpack('<H', bmp.read(2))[0]
    print("Bits per pixel:", bits_per_pixel)

    # Image size (pixel data only, không tính header)
    bmp.seek(34, 0)
    bmp_size = struct.unpack('<I', bmp.read(4))[0]
    print("Image size:", bmp_size)

    # Tính row_size chuẩn (bao gồm padding)
    row_size = ((bits_per_pixel * bmp_width + 31) // 32) * 4
    print("Row size:", row_size)

    # Nhảy đến vị trí pixel
    bmp.seek(offset, 0)

    bmp_line = ''
    bmp_list = []
    bmp_list_v = []

    # Đọc từng dòng pixel
    for line in range(bmp_height):
        for _ in range(row_size):
            bmp_byte = bmp.read(1)
            bmp_line += format(255 - struct.unpack('B', bmp_byte)[0], "08b")

        bmp_list.append(bmp_line[:bmp_width])                # dữ liệu raw
        bmp_list_v.append(bmp_line[:bmp_width].replace("0", " "))  # để in
        bmp_line = ''

    # BMP lưu từ dưới lên → đảo ngược lại
    bmp_list_v.reverse()

    # In ảnh ASCII ra console
    for line in bmp_list_v:
        print(line)

# Tạo dữ liệu bitmap theo dạng SSD1306 (8 pixel dọc = 1 byte)
byte_word = ""
bitmap_line = []
bitmap_array = []

for line in range(0, bmp_height, 8):
    for bit_num in range(bmp_width):
        for bit in range(line, line + 8):
            if bit > bmp_height - 1:
                byte_word += "0"
            else:
                byte_word += bmp_list[bit][bit_num]
        bitmap_line.append(hex(int(byte_word, 2)))
        byte_word = ''
    bitmap_array.append(bitmap_line)
    bitmap_line = []

# Đảo ngược để đúng chiều hiển thị
bitmap_array.reverse()

# Xuất ra file .txt (dạng mảng C)
with open(f'{file_name}.txt', 'w') as text_file:
    text_file.write(
        f'static const uint8_t {file_name}_{bmp_width}x{bmp_height}[] = \n'
    )
    text_file.write('{\n')

    for l_cnt, lines in enumerate(bitmap_array):
        text_file.write('    ')  # indent 4 spaces cho đẹp
        for cnt, hexa in enumerate(lines):
            text_file.write(f'{hexa}')
            if cnt < len(lines) - 1:
                text_file.write(',')
        if l_cnt < len(bitmap_array) - 1:
            text_file.write(',\n')
        else:
            text_file.write('\n')

    text_file.write('};\n')

