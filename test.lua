lib = require "int64"

local int64 = lib.new

a = 1 + int64(1)
b = int64 "\16" + int64("10",10)
print(lib.tostring(a,10), lib.tostring(b,2))
print("+", a+b)
print("-", lib.tostring(a-b,10))
print("*", a*b)
print("/", a/b)
print("%", a%b)
print("^", a^b)
print("==", a == b)
print(">", a > b)
print("#", #a)
