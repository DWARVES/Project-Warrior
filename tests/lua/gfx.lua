
gfx = Graphics()
pressed = false
act = false
angle = 0

init = function()
    print("Init on namespace : ", gfx.namespace())
    if not gfx.loadTexture("img", "img.png") then
        print("Couldn't load img.png")
        return false
    end
    gfx.hotpoint("img", 0, 25);

    if not gfx.loadTexture("text", "text.png") then
        print("Couldn't load text.png")
        return false
    end

    if not gfx.loadFont("font", "font.png") then
        print("Couldn't load font.png")
        return false
    end

    return true
end

press = function()
    pressed = true
    act = not act
    print("Pressed !")
end

unpress = function()
    pressed = false
    print("Unpressed !")
end

update = function()
    gfx.push()
    gfx.move(300,200)
    gfx.drawRect("text", 200, 200, 5, 3)
    gfx.move(100,100);

    if pressed then
        gfx.drawText("Hello world", "font", 30)
    end

    gfx.push()
    if act then
        angle = angle + 1
        while angle >= 360 do
            angle = angle - 360
        end
    end
    gfx.rotate(angle)
    gfx.scale(5, 1)
    gfx.blit("img")
    gfx.pop()
    gfx.pop()
end
