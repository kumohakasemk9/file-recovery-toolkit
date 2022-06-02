import sys, os, struct, math, time

def b2i(d):
	if len(d) == 4:
		return struct.unpack("<L",d)[0]
	elif len(d) == 2:
		return struct.unpack("<H",d)[0]
	else:
		return d[0]

dir_n = str(time.time()) + "_backup"
os.mkdir(dir_n)
f = open(sys.argv[1],"rb")
f.seek(1024)

sb = f.read(1024)
if b2i(sb[0x38:0x3a]) == 0xef53:
	print("Ext4 superblock detect.")
else:
	print("Error")
	sys.exit(0)
s_blocks_count_lo = b2i(sb[0x4:0x8])
s_first_data_block = b2i(sb[0x14:0x18])
s_log_block_size = b2i(sb[0x18:0x1c])
s_blocks_per_group = b2i(sb[0x20:0x24])
block_size = 2 ** (10 + s_log_block_size)
group_count = int(math.ceil(s_blocks_count_lo / s_blocks_per_group))
print(f"Block size: {block_size} byte")
print(f"Group count: {group_count}")
f.seek(s_first_data_block + 1 * block_size)
GD = []
for i in range(group_count):
	grpdsc = f.read(64)
	bg_block_bitmap_lo = b2i(grpdsc[0:0x4])
	bg_block_bitmap_hi = b2i(grpdsc[0x20:0x24])
	offs = bg_block_bitmap_lo + bg_block_bitmap_hi * 0x100000000
	GD.append(offs)
	print(f"Group {i}: Bitmap offset {offs}")
g = 0
for i in GD:
	f.seek(i * block_size)
	bb = f.read(block_size)
	for j in range(math.ceil(s_blocks_per_group / 8)):
		bb_submap = bb[j]
		for k in range(8):
			if (bb_submap & (1 << k)) == 0:
				now_block = g * s_blocks_per_group + j * 8 + k
				f.seek(now_block * block_size)
				now_block_r = f.read(block_size)
				png_width,png_height = struct.unpack(">LL",now_block_r[16:24])
				if now_block_r[0:8] == b"\x89PNG\r\n\x1a\n" and png_width > 500 and png_height > 500:
					print(f"PNG file header found in block {now_block}")
					fw = open(dir_n + "/" + str(now_block) + ".png","wb")
					fw.write(now_block_r)
					fw.close()
	g += 1
