
num = true
var = xargs("Hello world", true, 42, 27, false)

add = function(a, b, c)
    return a + b + c
end

echo = function(str1, str2, a, b)
    print(str1)
    print(str2)
    print(a)
	print(b)
end

obj = Additionner()
obj.add(27)
print("Added value : ", obj.get())
obj.add(30)
print("Added value : ", obj.get())
obj.value = 255;
print("Value : ", obj.value)

