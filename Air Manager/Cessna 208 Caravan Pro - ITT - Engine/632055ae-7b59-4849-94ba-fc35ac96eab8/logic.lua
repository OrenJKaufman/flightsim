-- Load images in Z-order
--Cessna 208 Caravan
img_add_fullscreen("itt_face.png")
img_needle = img_add_fullscreen("needle.png")
img_add_fullscreen("needlecap.png")

-- Functions
function new_itttemp(itttemp)

    temp = var_cap(itttemp[1], 100, 1100)
    
    if temp > 1000 then
        rotate(img_needle, 197 + (34 / 100 * (temp - 1000)) )
    elseif temp > 800 then
        rotate(img_needle, 125 + (72 / 200 * (temp - 800)) )
    elseif temp > 700 then
        rotate(img_needle, 86.5 + (38.5 / 100 * (temp - 700)) )
    elseif temp > 600 then
        rotate(img_needle, 61.5 + (25 / 100 * (temp - 600)) )
    else
        rotate(img_needle, 61.5 / 500 * (temp - 100) )
    end

end

function new_itttemp_FSX(itt)

    new_itttemp({itt})
    
end

-- Data bus subscribe
msfs_variable_subscribe("L:BKSQ_CARAVAN_ITT", "Celsius", new_itttemp_FSX)