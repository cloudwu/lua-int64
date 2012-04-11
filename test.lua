int64 = require "int64"

a = 1 + int64(1)
b = int64 "\16"
print("+", a+b)
print("-", a-b)
print("*", a*b)
print("/", a/b)
print("%", a%b)
print("^", a^b)
print("==", a == b)
print(">", a > b)
print("#", #a)
