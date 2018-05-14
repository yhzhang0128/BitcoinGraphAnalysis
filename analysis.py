f = open("result.log", "r")


mode = "out"
time_result = [[], [], []]
in_degree_result = [[], [], []]
out_degree_result = [[], [], []]
motif1_result = [[], [], []]
motif2_result = [[], [], []]
motif3_result = [[], [], []]

dataset_id = 0
date_id = 0
for line in f:
    #print(line)
    if line.startswith("@data"):
        date_id += 1
        if date_id > 30:
            date_id = 1
            dataset_id += 1

    if line.startswith("### out"):
        mode = "out"
    if line.startswith("### in"):
        mode = "in"

    ## time
    if line.startswith("@time for motif"):
        time_result[dataset_id].append(float(line.split()[-1]))

    ## degree
    if line.startswith("50") or line.startswith("90") or line.startswith("95") or line.startswith("99") or line.startswith("100"):
        if mode == "in":
            in_degree_result[dataset_id].append(int(line.split()[-1]))
        if mode == "out":
            out_degree_result[dataset_id].append(int(line.split()[-1]))

    ## motifs
    if line.startswith("Motif1"):
        motif1_result[dataset_id].append(int(line.split()[-1]))
    if line.startswith("Motif2"):
        motif2_result[dataset_id].append(int(line.split()[-1]))
    if line.startswith("Motif3"):
        motif3_result[dataset_id].append(int(line.split()[-1]))

    
print("## time:")        

for i in time_result:
    for j in i:
        print(str(j) + "\t", end='')
    print('\n')

print("## out degree:")
for j in range(3):
    print("dataset" + str(j))
    for i in range(30):
        for k in range(6):
            print(str(out_degree_result[j][i * 6 + k]) + "\t", end='')
        print()
    print()

print("## Motif3:")
for i in range(30):
    for j in range(3):
        print(str(motif3_result[j][i]) + "\t", end='')
    print()
