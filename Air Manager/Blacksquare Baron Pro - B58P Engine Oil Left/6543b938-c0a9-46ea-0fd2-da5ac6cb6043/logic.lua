img_add_fullscreen("oil_backdrop.png")
img_neddle_tmp = img_add("engine_neddle.png",98,0,60,256)
img_neddle_press = img_add("engine_neddle.png",98,0,60,256)

img_add("engine_center.png",98,98,60,60)
rotate(img_neddle_tmp,225)
rotate(img_neddle_press,135)

function PT_tmp(tmp)
    angle = tmp[1]*90/120+225
    
    angle = var_cap(angle, 225, 315)
    rotate(img_neddle_tmp, angle)
end

function PT_press(press)
    angle = 135-press[1]*90/100

    angle = var_cap(angle, 45, 135)
    rotate(img_neddle_press, angle)
end

function PT_tmp_FSX(tmp)

    PT_tmp({tmp})

end

function PT_press_FSX(press)

    PT_press({press})
    
end

msfs_variable_subscribe("L:BKSQ_OIL_TEMP_1", "Celsius", PT_tmp_FSX)
msfs_variable_subscribe("L:BKSQ_OIL_PRESS_1", "PSI", PT_press_FSX)