file = "nop_delay.c"

unitTime = 1000000000/168000000

with open(file, 'w') as f:
    for i in range(50,201,30):
        f.write("\nvoid nop_delay_"+str(i)+"ns(void)\n{\n")
        for i in range(0,round((i-25)/unitTime)):
            f.write("    __nop();\n")
        f.write("}\n")

