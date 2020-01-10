# Fibonacci numbers module

print("snow_eevee")

def fib(n):    # write Fibonacci series up to n
    a, b = 0, 1
    while a < n:
        print(a, end=' ')
        a, b = b, a+b
    print()

print("lightning_eevee")

def fib2(n):   # return Fibonacci series up to n
    result = []
    a, b = 0, 1
    while a < n:
        result.append(a)
        a, b = b, a+b
    return result

print("frame_eevee")

if __name__ == "__main__":
    print("main")
    import sys
    fib(int(sys.argv[1]))

print("ice_eevee")

import sys
print(sys.path)
