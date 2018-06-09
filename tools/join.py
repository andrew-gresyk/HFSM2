def mergeTo(folder, path, included, output):
	pathTokens = path.split("/")

	if len(pathTokens) > 1:
		folder += "/" + pathTokens[0]

	current = folder + "/" + pathTokens[-1]
	with open(current, 'r', encoding='utf-8') as input:
		for line in input:
			if line.startswith('#include "'):
				next = line[10 : -2]

				if next not in included:
					nextTokens = next.split("/")
					included.append(nextTokens[-1])

					#output.write("// inlined '" + pathTokens[-1] + "' -> '" + nextTokens[-1] + "'\n")

					if len(nextTokens) > 1:
						mergeTo(folder + "/" + nextTokens[0], nextTokens[-1], included, output)
					else:
						mergeTo(folder, next, included, output)
			else:
				if line.startswith('\ufeff'):
					line = line[1:]

				output.write(line)

output = open("../include/hfsm/machine_single.hpp", 'w', encoding='utf-8-sig')
included = []
mergeTo("../include", "hfsm/machine.hpp", included, output)

output.close()