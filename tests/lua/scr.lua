
prbl = function(bool)
    if bool then
        return "yes"
    else
        return "no"
    end
end

path = Path()
io.write("Path to process : ")
io.flush()
pth = io.read()
print("Path used : " .. pth)
print("Extension : " .. path.extension(pth))
print("Head : " .. path.head(pth))
print("Body : " .. path.body(pth))
print("Absolute : " .. prbl(path.absolute(pth)))
print("Exists : " .. prbl(path.exists(pth)))
print("Dir : " .. prbl(path.is_dir(pth)))
print("Reg : " .. prbl(path.is_reg(pth)))

if path.is_dir(pth) then
    if not path.list_contents(pth) then
        print("Couldn't list contents.")
        return
    end

    id = 0
    print("Contents : ")
    while path.has_content() do
        tmp_path = path.next_content()
        if path.is_dir(pth .. "/" .. tmp_path) then
            print("  #" .. id .. " : " .. tmp_path .. "/")
        else
            print("  #" .. id .. " : " .. tmp_path)
        end
        id = id + 1
    end
end


