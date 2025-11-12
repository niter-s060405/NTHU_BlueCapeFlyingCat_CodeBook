from os import *
f = "pA"
while 1:
    i = input("> ") or f; system("clear"); f = i
    print(f"file = {f}")
    if system(f"g++ {f}.cpp -std=c++17 -Wall -Wextra -Wshadow -O2 -D LOCAL -g -fsanitize=undefined,address -o {f}"):
        print("CE"); continue
    for x in sorted(listdir()):
        if x.startswith(f) and x.endswith(".in"):
            print(x); system(f"./{f} < {x}") and print("RE"); print()