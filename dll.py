from ctypes import *
import sys
import string
import glob

kernel32 = windll.kernel32

if len(sys.argv)>=1:
	print "Usage: dll.py <dll(s) to resolve>"
	sys.exit(0)

for arg in sys.argv[1:]:
	for dll in glob.glob(arg):
		try:
			windll.LoadLibrary(dll)
		except WindowsError:
			continue
		loadAddr = kernel32.GetModuleHandleA(dll)
		print "\n"+string.upper(dll)
		print hex(loadAddr) + " Load Address"
		print hex(loadAddr) + 0x1000) + " Text Segment"