################################################################################

def mergeTo(folder, path, included, pragmaOnceCounter, output):
	pathTokens = path.split("/")

	current = folder + "/" + pathTokens[-1]
	with open(current, 'r', encoding='utf-8') as input:
		for line in input:

			if line.startswith('#include "'):
				next = line[10 : -2]

				if next not in included:
					nextTokens = next.split("/")
					included.append(nextTokens[-1])

					#output.write("// inlined '" + pathTokens[-1] + "' -> '" + nextTokens[-1] + "'\n")

					if len(nextTokens) == 1:
						mergeTo(folder, next, included, pragmaOnceCounter, output)
					else:
						name = nextTokens.pop()
						mergeTo(folder + "/" + "/".join(nextTokens), name, included, pragmaOnceCounter, output)
			else:
				if line.startswith('\ufeff'):
					line = line[1:]

				if line.startswith('#pragma'):
					pragma = line[8:]

					if pragma.startswith('once'):
						pragmaOnceCounter += 1
						if pragmaOnceCounter > 1:
							continue

					elif pragma.startswith('region') or pragma.startswith('endregion'):
						continue

				output.write(line)

################################################################################

output = open("../include/hfsm2/machine.hpp", 'w', encoding='utf-8-sig')
included = []
pragmaOnceCounter = 0
mergeTo("../include/hfsm2", "machine_dev.hpp", included, pragmaOnceCounter, output)

output.close()

################################################################################
