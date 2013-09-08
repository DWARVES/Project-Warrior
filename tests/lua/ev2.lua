
process = function()
    save = EventsVariables()
    print("Ev1 : ", save.namespace())
    save.create("rogue", 2)
    save.create("warrior", 1)
    if save.exists("rogue") then
        print("Rogue : ", save.get("rogue"))
    end
    if save.exists("warrior") then
        print("Deleting angle.")
        save.delete("warrior")
    end
    save.set("rogue", 500)
end
