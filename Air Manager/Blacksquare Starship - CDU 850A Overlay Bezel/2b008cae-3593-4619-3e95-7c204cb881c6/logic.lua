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

local row_1 = 506
local row_2 = 577
local row_3 = 662
local row_4 = 735
local row_5 = 809
local row_6 = 883

local col_a = 138
local col_b = 227
local col_c = 316

local col_1 = 26
local col_2 = 112
local col_3 = 198
local col_4 = 282
local col_5 = 367
local col_6 = 452

local line_col_a = 1
local line_col_b = 480

local line_row_1 = 91
local line_row_2 = 156
local line_row_3 = 220
local line_row_4 = 283
local line_row_5 = 344

local width_1 = 77
local height_1 = 51
local width_2 = 54
local height_2 = 51
local width_3 = 50
local height_3 = 50

function event_button_press(event_name)
    fs2020_event(event_name)
    sound_play(press_snd)
end

function line_1_l_press()
    event_button_press("H:CDU_Line1L_1")
end
button_add(nil, nil, line_col_a,line_row_1,width_3,height_3, line_1_l_press)

function line_2_l_press()
    event_button_press("H:CDU_Line2L_1")
end
button_add(nil, nil, line_col_a,line_row_2,width_3,height_3, line_2_l_press)

function line_3_l_press()
    event_button_press("H:CDU_Line3L_1")
end
button_add(nil, nil, line_col_a,line_row_3,width_3,height_3, line_3_l_press)

function line_4_l_press()
    event_button_press("H:CDU_Line4L_1")
end
button_add(nil, nil, line_col_a,line_row_4,width_3,height_3, line_4_l_press)

function line_5_l_press()
    event_button_press("H:CDU_Line5L_1")
end
button_add(nil, nil, line_col_a,line_row_5,width_3,height_3, line_5_l_press)

function line_1_r_press()
    event_button_press("H:CDU_Line1R_1")
end
button_add(nil, nil, line_col_b,line_row_1,width_3,height_3, line_1_r_press)

function line_2_r_press()
    event_button_press("H:CDU_Line2R_1")
end
button_add(nil, nil, line_col_b,line_row_2,width_3,height_3, line_2_r_press)

function line_3_r_press()
    event_button_press("H:CDU_Line3R_1")
end
button_add(nil, nil, line_col_b,line_row_3,width_3,height_3, line_3_r_press)

function line_4_r_press()
    event_button_press("H:CDU_Line4R_1")
end
button_add(nil, nil, line_col_b,line_row_4,width_3,height_3, line_4_r_press)

function line_5_r_press()
    event_button_press("H:CDU_Line5R_1")
end
button_add(nil, nil, line_col_b,line_row_5,width_3,height_3, line_5_r_press)

function hsi_press()
    event_button_press("H:CDU_HsiButton_1")
end
button_add(nil, nil, col_a,row_1,width_1,height_1, hsi_press)

function nav_source_press()
    event_button_press("H:CDU_SourceButton_1")
end
button_add(nil, nil, col_a,row_2,width_1,height_1, nav_source_press)

function rdr_press()
    event_button_press("H:CDU_RadarButton_1")
end
button_add(nil, nil, col_b,row_1,width_1,height_1, rdr_press)

function arc_map_press()
    event_button_press("H:CDU_ArcMapButton_1")
end
button_add(nil, nil, col_c,row_1,width_1,height_1, arc_map_press)

function brg_press()
    event_button_press("H:CDU_BearingButton_1")
end
button_add(nil, nil, col_c,row_2,width_1,height_1, brg_press)

function rdr_control_press()
    event_button_press("H:CDU_RadarControlButton_1")
end
button_add(nil, nil, col_b,row_2,width_1,height_1, rdr_control_press)

function com_press()
    event_button_press("H:CDU_ComButton_1")
end
button_add(nil, nil, col_1,row_3,width_2,height_2, com_press)

function nav_press()
    event_button_press("H:CDU_NavButton_1")
end
button_add(nil, nil, col_1,row_4,width_2,height_2, nav_press)

function adf_press()
    event_button_press("H:CDU_AdfButton_1")
end
button_add(nil, nil, col_1,row_5,width_2,height_2, adf_press)

function atc_press()
    event_button_press("H:CDU_TransponderButton_1")
end
button_add(nil, nil, col_1,row_6,width_2,height_2, atc_press)

function fltpln_press()
    event_button_press("H:CDU_FlightPlanButton_1")
end
button_add(nil, nil, col_2,row_3,width_2,height_2, fltpln_press)

function sysctrl_press()
    event_button_press("H:CDU_SystemControlButton_1")
end
button_add(nil, nil, col_2,row_4,width_2,height_2, sysctrl_press)

function idx_press()
    event_button_press("H:CDU_IndexButton_1")
end
button_add(nil, nil, col_2,row_5,width_2,height_2, idx_press)

function dir_press()
    event_button_press("H:CDU_DirectButton_1")
end
button_add(nil, nil, col_2,row_6,width_2,height_2, dir_press)

function up_press()
    event_button_press("H:CDU_UpButton_1")
end
button_add(nil, nil, col_3,row_3,width_2,height_2, up_press)

function down_press()
    event_button_press("H:CDU_DownButton_1")
end
button_add(nil, nil, col_3,row_4,width_2,height_2, down_press)

function vnav_press()
    event_button_press("H:CDU_VnavButton_1")
end
button_add(nil, nil, col_3,row_5,width_2,height_2, vnav_press)

function msg_press()
    event_button_press("H:CDU_MessageButton_1")
end
button_add(nil, nil, col_3,row_6,width_2,height_2, msg_press)

function up_press()
    event_button_press("H:CDU_UpButton_1")
end
button_add(nil, nil, col_3,row_3,width_2,height_2, up_press)

function key_1_press()
    event_button_press("H:CDU_1Button_1")
end
button_add(nil, nil, col_4,row_3,width_2,height_2, key_1_press)

function key_2_press()
    event_button_press("H:CDU_2Button_1")
end
button_add(nil, nil, col_5,row_3,width_2,height_2, key_2_press)

function key_3_press()
    event_button_press("H:CDU_3Button_1")
end
button_add(nil, nil, col_6,row_3,width_2,height_2, key_3_press)

function key_4_press()
    event_button_press("H:CDU_4Button_1")
end
button_add(nil, nil, col_4,row_4,width_2,height_2, key_4_press)

function key_5_press()
    event_button_press("H:CDU_5Button_1")
end
button_add(nil, nil, col_5,row_4,width_2,height_2, key_5_press)

function key_6_press()
    event_button_press("H:CDU_6Button_1")
end
button_add(nil, nil, col_6,row_4,width_2,height_2, key_6_press)

function key_7_press()
    event_button_press("H:CDU_7Button_1")
end
button_add(nil, nil, col_4,row_5,width_2,height_2, key_7_press)

function key_8_press()
    event_button_press("H:CDU_8Button_1")
end
button_add(nil, nil, col_5,row_5,width_2,height_2, key_8_press)

function key_9_press()
    event_button_press("H:CDU_9Button_1")
end
button_add(nil, nil, col_6,row_5,width_2,height_2, key_9_press)

function clr_press()
    event_button_press("H:CDU_ClearButton_1")
end
button_add(nil, nil, col_4,row_6,width_2,height_2, clr_press)

function key_0_press()
    event_button_press("H:CDU_0Button_1")
end
button_add(nil, nil, col_5,row_6,width_2,height_2, key_0_press)

function ltr_press()
    event_button_press("H:CDU_LetterButton_1")
end
button_add(nil, nil, col_6,row_6,width_2,height_2, ltr_press)