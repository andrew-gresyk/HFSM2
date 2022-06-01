import re

#===============================================================================

def mergeTo(folder, path, included, commentRE, output):
	pathTokens = path.split("/")

	current = folder + "/" + pathTokens[-1]
	with open(current, 'r', encoding='utf-8') as input:
		lastLineEmpty = False

		if included:
			output.write("\n")
			lastLineEmpty = True

		for line in input:
			if line.startswith('#include "'):
				next = line[10 : -2]

				if next not in included:
					nextTokens = next.split("/")
					included.append(nextTokens[-1])

					#output.write("// inlined '" + pathTokens[-1] + "' -> '" + nextTokens[-1] + "'\n")

					if len(nextTokens) == 1:
						mergeTo(folder, next, included, commentRE, output)
					else:
						name = nextTokens.pop()
						mergeTo(folder + "/" + "/".join(nextTokens), name, included, commentRE, output)

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

#-------------------------------------------------------------------------------

output = open("../include/hfsm2/machine.hpp", 'w', encoding='utf-8-sig')
included = []
commentRE = re.compile("(?:\s*\/\/ COMMON)|(?:\s*\/\/ SPECIFIC)|(?:\s*\/\/\/\/)|(?:\s*\/\/--)|(?:\s*\/\/ -)")

mergeTo("../development/hfsm2", "machine_dev.hpp", included, commentRE, output)

output.close()

#===============================================================================
