play_sounds = user_prop_add_boolean("Play Sounds", true, "Play sounds in Air Manager")                           -- Use sounds in Air Manager    

-- play sounds
if user_prop_get(play_sounds) then
    press_snd = sound_add("click.wav")
else
    press_snd = sound_add("silence.wav")
end
-- end play sounds

-- END INITIALIZE USER PROPERTIES

--*********************************************END USER PROPERTY CONFIG***********************************************

img_add_fullscreen("bg.png")

function event_button_press(ref)
    msfs_event("K:ROTOR_BRAKE", ref)
    sound_play(press_snd)
end

-- ================= BATTERY =================
EVENT_BATTERY = 101

function battery_press()
    event_button_press(EVENT_BATTERY)
end
button_add(nil, nil, 415, 88, 158, 158, battery_press)

local gImgBatteryOn = img_add("battery_on.png", 427, 95, 138, 140)
local gImgBatteryOff = img_add("indicator_off.png", 462, 174, 74, 32)

function battery_indicator(switch_01_a, switch_01_c)
    visible(gImgBatteryOn, switch_01_a > 0)
    visible(gImgBatteryOff, switch_01_c > 0)
end
msfs_variable_subscribe("L:switch_01_a", "number", "L:switch_01_c", "number", battery_indicator)

-- ================= APU GEN =================
EVENT_APU_GEN = 201

function apu_gen_press()
    event_button_press(EVENT_APU_GEN)
end
button_add(nil, nil, 415, 281, 158, 158, apu_gen_press)

local gImgApuGenOn = img_add("apu_gen_on.png", 426, 290, 138, 140)
local gImgApuGenOff = img_add("indicator_off.png", 461, 369, 74, 32)

function apu_gen_indicator(switch_02_a, switch_02_c)
    visible(gImgApuGenOn, switch_02_a > 0)
    visible(gImgApuGenOff, switch_02_c > 0)
end
msfs_variable_subscribe("L:switch_02_a", "number", "L:switch_02_c", "number", apu_gen_indicator)

-- ================= BUS TIE =================
EVENT_BUS_TIE_LEFT = 501
EVENT_BUS_TIE_RIGHT = 601

function bus_tie_left_press()
    event_button_press(EVENT_BUS_TIE_LEFT)
end
button_add(nil, nil, 102, 446, 157, 155, bus_tie_left_press)

function bus_tie_right_press()
    event_button_press(EVENT_BUS_TIE_RIGHT)
end
button_add(nil, nil, 725, 446, 157, 155, bus_tie_right_press)

local gImgBusTieLeftAuto = img_add("bus_tie_left_auto.png", 111, 454, 139, 139)
local gImgBusTieLeftIsln = img_add("indicator_isln.png", 139, 537, 80, 32)
local gImgBusTieRightAuto = img_add("bus_tie_right_auto.png", 735, 457, 148, 135)
local gImgBusTieRightIsln = img_add("indicator_isln.png", 763, 540, 80, 32)

function bus_tie_left_indicator(switch_05_a, switch_05_c)
    visible(gImgBusTieLeftAuto, switch_05_a > 0)
    visible(gImgBusTieLeftIsln, switch_05_c > 0)
end

function bus_tie_right_indicator(switch_06_a, switch_06_c)
    visible(gImgBusTieRightAuto, switch_06_a > 0)
    visible(gImgBusTieRightIsln, switch_06_c > 0)
end
msfs_variable_subscribe("L:switch_05_a", "number", "L:switch_05_c", "number", bus_tie_left_indicator)
msfs_variable_subscribe("L:switch_06_a", "number", "L:switch_06_c", "number", bus_tie_right_indicator)

-- ================= APU KNOB =================
EVENT_APU_INC = 307
EVENT_APU_DEC = 308

function apu_inc_press()
    event_button_press(EVENT_APU_INC)
end
button_add(nil, nil, 826, 44, 154, 261, apu_inc_press)

function apu_dec_press()
    event_button_press(EVENT_APU_DEC)
end
button_add(nil, nil, 671, 44, 154, 261, apu_dec_press)

local gImgApuOff = img_add("apu_off.png", 749, 136, 160, 151)
local gImgApuStart = img_add("apu_start.png", 749, 136, 160, 151)
local gImgApuFault = img_add("apu_fault.png", 752, 334, 117, 36)

function apu_knob(switch_03_a)
    visible(gImgApuOff, switch_03_a == 0)
    visible(gImgApuStart, switch_03_a == 100)
end

function apu_fault(switch_04_a)
    visible(gImgApuFault, switch_04_a > 0)
end

msfs_variable_subscribe("L:switch_03_a", "number", apu_knob)
msfs_variable_subscribe("L:switch_04_a", "number", apu_fault)

