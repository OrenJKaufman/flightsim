--[[
******************************************************************************************
******************************CDU 850A Overlay Bezel**********************************
******************************************************************************************
]]--

play_sounds = user_prop_add_boolean("Play Sounds", true, "Play sounds in Air Manager")                           -- Use sounds in Air Manager    

-- play sounds
if user_prop_get(play_sounds) then
    press_snd = sound_add("click.wav")
    dial_snd = sound_add("dial.wav")
else
    press_snd = sound_add("silence.wav")
    dial_snd = sound_add("silence.wav")
end
-- end play sounds

-- END INITIALIZE USER PROPERTIES

--*********************************************END USER PROPERTY CONFIG***********************************************

img_add_fullscreen("bg.png")

local line_row_1 = 142
local line_row_2 = 219
local line_row_3 = 294
local line_row_4 = 371
local line_row_5 = 446
local line_row_6 = 523

local line_col_a = 0
local line_col_b = 782

local line_bottom = 662
local line_col_1 = 128
local line_col_2 = 216
local line_col_3 = 304
local line_col_4 = 391
local line_col_5 = 479
local line_col_6 = 567
local line_col_7 = 655

local width_1 = 63
local height_1 = 63

function event_button_press(event_name)
    fs2020_event(event_name)
    sound_play(press_snd)
end

function line_1_l_press()
    event_button_press("H:MFD_Line1L")
end
button_add(nil, nil, line_col_a,line_row_1,width_1,height_1, line_1_l_press)

function line_2_l_press()
    event_button_press("H:MFD_Line2L")
end
button_add(nil, nil, line_col_a,line_row_2,width_1,height_1, line_2_l_press)

function line_3_l_press()
    event_button_press("H:MFD_Line3L")
end
button_add(nil, nil, line_col_a,line_row_3,width_1,height_1, line_3_l_press)

function line_4_l_press()
    event_button_press("H:MFD_Line4L")
end
button_add(nil, nil, line_col_a,line_row_4,width_1,height_1, line_4_l_press)

function line_5_l_press()
    event_button_press("H:MFD_Line5L")
end
button_add(nil, nil, line_col_a,line_row_5,width_1,height_1, line_5_l_press)

function line_6_l_press()
    event_button_press("H:MFD_Line6L")
end
button_add(nil, nil, line_col_a,line_row_6,width_1,height_1, line_6_l_press)

function line_1_r_press()
    event_button_press("H:MFD_Line1R")
end
button_add(nil, nil, line_col_b,line_row_1,width_1,height_1, line_1_r_press)

function line_2_r_press()
    event_button_press("H:MFD_Line2R")
end
button_add(nil, nil, line_col_b,line_row_2,width_1,height_1, line_2_r_press)

function line_3_r_press()
    event_button_press("H:MFD_Line3R")
end
button_add(nil, nil, line_col_b,line_row_3,width_1,height_1, line_3_r_press)

function line_4_r_press()
    event_button_press("H:MFD_Line4R")
end
button_add(nil, nil, line_col_b,line_row_4,width_1,height_1, line_4_r_press)

function line_5_r_press()
    event_button_press("H:MFD_Line5R")
end
button_add(nil, nil, line_col_b,line_row_5,width_1,height_1, line_5_r_press)

function line_6_r_press()
    event_button_press("H:MFD_Line6R")
end
button_add(nil, nil, line_col_b,line_row_6,width_1,height_1, line_6_r_press)

function line_1_b_press()
    event_button_press("H:MFD_Line1B")
end
button_add(nil, nil, line_col_1,line_bottom,width_1,height_1, line_1_b_press)

function line_2_b_press()
    event_button_press("H:MFD_Line2B")
end
button_add(nil, nil, line_col_2,line_bottom,width_1,height_1, line_2_b_press)

function line_3_b_press()
    event_button_press("H:MFD_Line3B")
end
button_add(nil, nil, line_col_3,line_bottom,width_1,height_1, line_3_b_press)

function line_4_b_press()
    event_button_press("H:MFD_Line4B")
end
button_add(nil, nil, line_col_4,line_bottom,width_1,height_1, line_4_b_press)

function line_5_b_press()
    event_button_press("H:MFD_Line5B")
end
button_add(nil, nil, line_col_5,line_bottom,width_1,height_1, line_5_b_press)

function line_6_b_press()
    event_button_press("H:MFD_Line6B")
end
button_add(nil, nil, line_col_6,line_bottom,width_1,height_1, line_6_b_press)

function line_7_b_press()
    event_button_press("H:MFD_Line7B")
end
button_add(nil, nil, line_col_7,line_bottom,width_1,height_1, line_7_b_press)
