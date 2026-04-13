-- Global variables --
img_add_fullscreen("temp_backdrop.png")
img_neddle_cht = img_add("engine_neddle.png",98,0,60,256)
img_neddle_egt = img_add("engine_neddle.png",98,0,60,256)
rotate(img_neddle_cht, -135)
rotate(img_neddle_egt, 132)

img_add("engine_center.png",98,98,60,60)

function PT_cht(cht)
    cht = var_cap(cht[1], 50, 250)
    rotate(img_neddle_cht, -135 + (90 / 200 * (cht - 50)) )
end

function PT_egt(egtraw)
    egt = var_cap(egtraw[1], 300, 950)
    rotate(img_neddle_egt, (47 / 280 * (egt - 505) - 110) * -1)   
end

function PT_cht_FSX(cht)
    PT_cht({cht})
end

function PT_egt_FSX(egt)
    PT_egt({egt})
end

-- Bus subscribe --
msfs_variable_subscribe("L:BKSQ_CHT_1", "Celsius", PT_cht_FSX)
msfs_variable_subscribe("L:BKSQ_EGT_1", "Celsius", PT_egt_FSX)