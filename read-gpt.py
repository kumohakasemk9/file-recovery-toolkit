import sys,struct

f = open(sys.argv[1],"rb")
f.seek(1024)
for i in range(62):
	g = f.read(128)
	pt0, pt1, pt2 = struct.unpack("<LHH",g[0:8])
	pt3, pt4, pt5 = struct.unpack(">HLH",g[8:16])
	ptuid = "%08X-%04X-%04X-%04X-%08X%04X" % (pt0,pt1,pt2,pt3,pt4,pt5)
	pg0, pg1, pg2 = struct.unpack("<LHH",g[16:24])
	pg3, pg4, pg5 = struct.unpack(">HLH",g[24:32])
	pguid = "%08X-%04X-%04X-%04X-%08X%04X" % (pg0,pg1,pg2,pg3,pg4,pg5)
	firstlba, lastlba = struct.unpack("<QQ",g[32:48])
	firstlba = firstlba * 512
	lastlba = lastlba * 512
	partname = g[56:].decode("utf-16")
	if not (pt0 == 0 and pt1 == 0 and pt2 == 0 and pt3 == 0 and pt4 == 0 and pt5 == 0):
		print(f"Partition TYPE:{ptuid} ID:{pguid} Start:{firstlba} End:{lastlba} NAME:{partname}")
f.close()
