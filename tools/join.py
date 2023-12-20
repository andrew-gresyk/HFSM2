import re

#===============================================================================

def merge(path, folder, lastLineEmpty, included, output, commentRE):
	pathTokens = path.split("/")

	current = folder + "/" + pathTokens[-1]
	with open(current, 'r', encoding='utf-8') as input:
		if not lastLineEmpty:
			output.write("\n")
			lastLineEmpty = True

		for line in input:
			hashIndex = line.find('#include "')

			if hashIndex != -1:
				next = line[hashIndex + 10 : -2]

				if next not in included:
					nextTokens = next.split("/")
					included.append(nextTokens[-1])

					#output.write("// inlined '" + pathTokens[-1] + "' -> '" + nextTokens[-1] + "'\n")

					if len(nextTokens) == 1:
						lastLineEmpty = merge(next, folder,    lastLineEmpty, included, output, commentRE)
					else:
						name = nextTokens.pop()
						subFolder = folder + "/" + "/".join(nextTokens)
						lastLineEmpty = merge(name, subFolder, lastLineEmpty, included, output, commentRE)

			else:
				if line.startswith('\ufeff'):
					line = line[1:]

				if commentRE.match(line):
					continue

				if line == "\n":
					if lastLineEmpty:
						continue

					lastLineEmpty = True
				else:
					lastLineEmpty = False

				output.write(line)

	return lastLineEmpty

#-------------------------------------------------------------------------------

commentRE = re.compile("(?:\s*\/\/ COMMON)|(?:\s*\/\/ SPECIFIC)|(?:\s*\/\/\/\/)|(?:\s*\/\/--)|(?:\s*\/\/ -)")

output = open("../include/hfsm2/machine.hpp"	 , 'w', encoding='utf-8-sig')
merge("machine_dev.hpp"		, "../development/hfsm2", True, [], output, commentRE)
output.close()

#===============================================================================
