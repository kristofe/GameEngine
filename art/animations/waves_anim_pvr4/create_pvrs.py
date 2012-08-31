import os.path, fnmatch

def listFiles(root,patterns='*',recurse=1,return_folders=0):
	#expand patterns from semicolon-sep string to list
	pattern_list = patterns.split(';')
	
	class Bunch:
		def __init__(self,**kwds): self.__dict__.update(kwds)
	arg = Bunch(recurse=recurse,pattern_list=pattern_list,return_folders=return_folders,results=[])

	def visit(arg,dirname,files):
		for name in files:
			fullname = os.path.normpath(os.path.join(dirname,name))
			if arg.return_folders or os.path.isfile(fullname):
				for pattern in arg.pattern_list:
					if fnmatch.fnmatch(name,pattern):
						arg.results.append(fullname)
						break
		if not arg.recurse: files[:] = []
		
	os.path.walk(root,visit,arg)

	return arg.results

if __name__=='__main__':
	import sys
	if len(sys.argv) !=2:
		print "usage: root pattern"
		sys.exit(100)
	args = sys.argv[1].split(",")
	#if len(args) != 3:
	#	print "usage: root,pattern,cmd"
	#	sys.exit(200)
	theFiles = listFiles(args[0],args[1]);
	#cmd = args[2]
	for file in theFiles:
		newfile = file.replace('.png','.pvr');
		newfile = newfile.replace('_','_pvr2_');	
		cmd = "../../texturetool -e PVRTC --bits-per-pixel-2 -o "+ newfile +" -f PVR "+ file
		print cmd
		os.system(cmd)
		newfile = file.replace('.png','.pvr');
		newfile = newfile.replace('_','_pvr4_');
		cmd = "../../texturetool -e PVRTC --bits-per-pixel-4 -o "+ newfile +" -f PVR "+ file
		print cmd
		os.system(cmd)
