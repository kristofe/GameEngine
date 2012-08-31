import sys,random

if __name__=='__main__':
	if len(sys.argv) !=2:
		print "usage: inputfile, outputfile"
		sys.exit(100)
		
	args = sys.argv[1].split(",")
		
	target = open(args[0], 'r')
	lines = target.readlines()
	target.close()
	
	print "read " + str(len(lines)) +" words"
	
	newlines = []
	for line in lines:
		strLen = len(line)
		if(strLen >= 4 and strLen <= 8):
			newlines.append(line)
			
	random.shuffle(newlines)
	
	outputfile = open(args[1], 'w')
	outputfile.writelines(newlines)
	outputfile.close()
	
	print "wrote " + str(len(newlines)) + " words"