
process = function()
    save = EventsVariables()
    print("Ev1 : ", save.namespace())
    save.create("thief", 2)
    save.create("angel", 1)
    if save.exists("thief") then
        print("Thief : ", save.get("thief"))
    end
    if save.exists("angel") then
        print("Deleting angle.")
        save.delete("angel")
    end
    save.set("thief", 500)
end
