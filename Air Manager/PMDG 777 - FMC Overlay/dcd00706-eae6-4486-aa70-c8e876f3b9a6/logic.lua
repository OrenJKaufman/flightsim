--[[
******************************************************************************************
******************************CDU 850A Overlay Bezel**********************************
******************************************************************************************
]]--

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

-- LINE SELECT
local ls_col_a = 32
local ls_col_b = 1227

local ls_row_1 = 267
local ls_row_2 = 375
local ls_row_3 = 483
local ls_row_4 = 590
local ls_row_5 = 700
local ls_row_6 = 804

local width_ls = 86
local height_ls = 65

-- LETTER
local letter_col_a = 540
local letter_col_b = 670
local letter_col_c = 805
local letter_col_d = 942
local letter_col_e = 1074

local letter_row_1 = 1315
local letter_row_2 = 1436
local letter_row_3 = 1564
local letter_row_4 = 1689
local letter_row_5 = 1817
local letter_row_6 = 1941

local width_letter = 107
local height_letter = 113

-- NUMBER
local number_col_a = 124
local number_col_b = 256
local number_col_c = 391

local number_row_1 = 1564
local number_row_2 = 1689
local number_row_3 = 1817
local number_row_4 = 1941

local width_number = 107
local height_number = 113

-- FUNCTION
local function_col_a = 127
local function_col_b = 301
local function_col_c = 474
local function_col_d = 645
local function_col_e = 815

local function_row_1 = 1034
local function_row_2 = 1167
local function_row_3 = 1300
local function_row_4 = 1434

local width_function = 168
local height_function = 124

-- SPECIAL
local exec_col = 1042
local exec_row = 1193
local width_exec = 154
local height_exec = 95

EVENT_LS_1 = 32801
EVENT_LS_2 = 32901
EVENT_LS_3=  33001
EVENT_LS_4 = 33101
EVENT_LS_5 = 33201
EVENT_LS_6 = 33301
EVENT_LS_7 = 33401
EVENT_LS_8 = 33501
EVENT_LS_9 = 33601
EVENT_LS_10 = 33701
EVENT_LS_11 = 33801
EVENT_LS_12 = 33901

EVENT_INIT_REF = 34001
EVENT_RTE = 34101
EVENT_DEP_ARR = 342001
EVENT_ALTN = 34301
EVENT_VNAV = 34401
EVENT_FIX = 34501
EVENT_LEGS = 34601
EVENT_HOLD = 34701
EVENT_FMC_COMM = 347101
EVENT_PROG = 34801
EVENT_MENU = 35001
EVENT_NAV_RAD = 35101
EVENT_PREV_PAGE = 35201 
EVENT_NEXT_PAGE = 35301
EVENT_EXEC = 34901

EVENT_1 = 35401
EVENT_2 = 35501
EVENT_3 = 35601
EVENT_4 = 35701
EVENT_5 = 35801
EVENT_6 = 35901
EVENT_7 = 36001
EVENT_8 = 36101
EVENT_9 = 36201
EVENT_DECIMAL = 36301
EVENT_0 = 36401
EVENT_PLUS_MINUS = 36501

EVENT_A = 36601
EVENT_B = 36701
EVENT_C = 36801
EVENT_D = 36901
EVENT_E = 37001
EVENT_F = 37101
EVENT_G = 37201
EVENT_H = 37301
EVENT_I = 37401
EVENT_J = 37501
EVENT_K = 37601
EVENT_L = 37701
EVENT_M = 37801
EVENT_N = 37901
EVENT_O = 38001
EVENT_P = 38101
EVENT_Q = 38201
EVENT_R = 38301
EVENT_S = 38401
EVENT_T = 38501
EVENT_U = 38601
EVENT_V = 38701
EVENT_W = 38801
EVENT_X = 38901
EVENT_Y = 39001
EVENT_Z = 39101
EVENT_SP = 39201
EVENT_DEL = 39301
EVENT_SLASH = 39401
EVENT_CLR = 39501

function event_button_press(ref)
    msfs_event("K:ROTOR_BRAKE", ref)
    sound_play(press_snd)
end

function line_1_l_press()
    event_button_press(EVENT_LS_1)
end
button_add(nil, nil, ls_col_a,ls_row_1,width_ls,height_ls, line_1_l_press)

function line_2_l_press()
    event_button_press(EVENT_LS_2)
end
button_add(nil, nil, ls_col_a,ls_row_2,width_ls,height_ls, line_2_l_press)

function line_3_l_press()
    event_button_press(EVENT_LS_3)
end
button_add(nil, nil, ls_col_a,ls_row_3,width_ls,height_ls, line_3_l_press)

function line_4_l_press()
    event_button_press(EVENT_LS_4)
end
button_add(nil, nil, ls_col_a,ls_row_4,width_ls,height_ls, line_4_l_press)

function line_5_l_press()
    event_button_press(EVENT_LS_5)
end
button_add(nil, nil, ls_col_a,ls_row_5,width_ls,height_ls, line_5_l_press)

function line_6_l_press()
    event_button_press(EVENT_LS_6)
end
button_add(nil, nil, ls_col_a,ls_row_6,width_ls,height_ls, line_6_l_press)

function line_1_r_press()
    event_button_press(EVENT_LS_7)
end
button_add(nil, nil, ls_col_b,ls_row_1,width_ls,height_ls, line_1_r_press)

function line_2_r_press()
    event_button_press(EVENT_LS_8)
end
button_add(nil, nil, ls_col_b,ls_row_2,width_ls,height_ls, line_2_r_press)

function line_3_r_press()
    event_button_press(EVENT_LS_9)
end
button_add(nil, nil, ls_col_b,ls_row_3,width_ls,height_ls, line_3_r_press)

function line_4_r_press()
    event_button_press(EVENT_LS_10)
end
button_add(nil, nil, ls_col_b,ls_row_4,width_ls,height_ls, line_4_r_press)

function line_5_r_press()
    event_button_press(EVENT_LS_11)
end
button_add(nil, nil, ls_col_b,ls_row_5,width_ls,height_ls, line_5_r_press)

function line_6_r_press()
    event_button_press(EVENT_LS_12)
end
button_add(nil, nil, ls_col_b,ls_row_6,width_ls,height_ls, line_6_r_press)

function letter_a_press()
    event_button_press(EVENT_A)
end
button_add(nil, nil, letter_col_a,letter_row_1,width_letter,height_letter, letter_a_press)

function letter_b_press()
    event_button_press(EVENT_B)
end
button_add(nil, nil, letter_col_b,letter_row_1,width_letter,height_letter, letter_b_press)

function letter_c_press()
    event_button_press(EVENT_C)
end
button_add(nil, nil, letter_col_c,letter_row_1,width_letter,height_letter, letter_c_press)

function letter_d_press()
    event_button_press(EVENT_D)
end
button_add(nil, nil, letter_col_d,letter_row_1,width_letter,height_letter, letter_d_press)

function letter_e_press()
    event_button_press(EVENT_E)
end
button_add(nil, nil, letter_col_e,letter_row_1,width_letter,height_letter, letter_e_press)

function letter_f_press()
    event_button_press(EVENT_F)
end
button_add(nil, nil, letter_col_a,letter_row_2,width_letter,height_letter, letter_f_press)

function letter_g_press()
    event_button_press(EVENT_G)
end
button_add(nil, nil, letter_col_b,letter_row_2,width_letter,height_letter, letter_g_press)

function letter_h_press()
    event_button_press(EVENT_H)
end
button_add(nil, nil, letter_col_c,letter_row_2,width_letter,height_letter, letter_h_press)

function letter_i_press()
    event_button_press(EVENT_I)
end
button_add(nil, nil, letter_col_d,letter_row_2,width_letter,height_letter, letter_i_press)

function letter_j_press()
    event_button_press(EVENT_J)
end
button_add(nil, nil, letter_col_e,letter_row_2,width_letter,height_letter, letter_j_press)

function letter_k_press()
    event_button_press(EVENT_K)
end
button_add(nil, nil, letter_col_a,letter_row_3,width_letter,height_letter, letter_k_press)

function letter_l_press()
    event_button_press(EVENT_L)
end
button_add(nil, nil, letter_col_b,letter_row_3,width_letter,height_letter, letter_l_press)

function letter_m_press()
    event_button_press(EVENT_M)
end
button_add(nil, nil, letter_col_c,letter_row_3,width_letter,height_letter, letter_m_press)

function letter_n_press()
    event_button_press(EVENT_N)
end
button_add(nil, nil, letter_col_d,letter_row_3,width_letter,height_letter, letter_n_press)

function letter_o_press()
    event_button_press(EVENT_O)
end
button_add(nil, nil, letter_col_e,letter_row_3,width_letter,height_letter, letter_o_press)

function letter_p_press()
    event_button_press(EVENT_P)
end
button_add(nil, nil, letter_col_a,letter_row_4,width_letter,height_letter, letter_p_press)

function letter_q_press()
    event_button_press(EVENT_Q)
end
button_add(nil, nil, letter_col_b,letter_row_4,width_letter,height_letter, letter_q_press)

function letter_r_press()
    event_button_press(EVENT_R)
end
button_add(nil, nil, letter_col_c,letter_row_4,width_letter,height_letter, letter_r_press)

function letter_s_press()
    event_button_press(EVENT_S)
end
button_add(nil, nil, letter_col_d,letter_row_4,width_letter,height_letter, letter_s_press)

function letter_t_press()
    event_button_press(EVENT_T)
end
button_add(nil, nil, letter_col_e,letter_row_4,width_letter,height_letter, letter_t_press)

function letter_u_press()
    event_button_press(EVENT_U)
end
button_add(nil, nil, letter_col_a,letter_row_5,width_letter,height_letter, letter_u_press)

function letter_v_press()
    event_button_press(EVENT_V)
end
button_add(nil, nil, letter_col_b,letter_row_5,width_letter,height_letter, letter_v_press)

function letter_w_press()
    event_button_press(EVENT_W)
end
button_add(nil, nil, letter_col_c,letter_row_5,width_letter,height_letter, letter_w_press)

function letter_x_press()
    event_button_press(EVENT_X)
end
button_add(nil, nil, letter_col_d,letter_row_5,width_letter,height_letter, letter_x_press)

function letter_y_press()
    event_button_press(EVENT_Y)
end
button_add(nil, nil, letter_col_e,letter_row_5,width_letter,height_letter, letter_y_press)

function letter_z_press()
    event_button_press(EVENT_Z)
end
button_add(nil, nil, letter_col_a,letter_row_6,width_letter,height_letter, letter_z_press)

function letter_sp_press()
    event_button_press(EVENT_SP)
end
button_add(nil, nil, letter_col_b,letter_row_6,width_letter,height_letter, letter_sp_press)

function letter_del_press()
    event_button_press(EVENT_DEL)
end
button_add(nil, nil, letter_col_c,letter_row_6,width_letter,height_letter, letter_del_press)

function letter_slash_press()
    event_button_press(EVENT_SLASH)
end
button_add(nil, nil, letter_col_d,letter_row_6,width_letter,height_letter, letter_slash_press)

function letter_clr_press()
    event_button_press(EVENT_CLR)
end
button_add(nil, nil, letter_col_e,letter_row_6,width_letter,height_letter, letter_clr_press)

function number_1_press()
    event_button_press(EVENT_1)
end
button_add(nil, nil, number_col_a,number_row_1,width_number,height_number, number_1_press)

function number_2_press()
    event_button_press(EVENT_2)
end
button_add(nil, nil, number_col_b,number_row_1,width_number,height_number, number_2_press)

function number_3_press()
    event_button_press(EVENT_3)
end
button_add(nil, nil, number_col_c,number_row_1,width_number,height_number, number_3_press)

function number_4_press()
    event_button_press(EVENT_4)
end
button_add(nil, nil, number_col_a,number_row_2,width_number,height_number, number_4_press)

function number_5_press()
    event_button_press(EVENT_5)
end
button_add(nil, nil, number_col_b,number_row_2,width_number,height_number, number_5_press)

function number_6_press()
    event_button_press(EVENT_6)
end
button_add(nil, nil, number_col_c,number_row_2,width_number,height_number, number_6_press)

function number_7_press()
    event_button_press(EVENT_7)
end
button_add(nil, nil, number_col_a,number_row_3,width_number,height_number, number_7_press)

function number_8_press()
    event_button_press(EVENT_8)
end
button_add(nil, nil, number_col_b,number_row_3,width_number,height_number, number_8_press)

function number_9_press()
    event_button_press(EVENT_9)
end
button_add(nil, nil, number_col_c,number_row_3,width_number,height_number, number_9_press)

function number_decimal_press()
    event_button_press(EVENT_DECIMAL)
end
button_add(nil, nil, number_col_a,number_row_4,width_number,height_number, number_decimal_press)

function number_0_press()
    event_button_press(EVENT_0)
end
button_add(nil, nil, number_col_b,number_row_4,width_number,height_number, number_0_press)

function number_plus_minus_press()
    event_button_press(EVENT_PLUS_MINUS)
end
button_add(nil, nil, number_col_c,number_row_4,width_number,height_number, number_plus_minus_press)

function function_init_ref_press()
    event_button_press(EVENT_INIT_REF)
end
button_add(nil, nil, function_col_a,function_row_1,width_function,height_function, function_init_ref_press)

function function_rte_press()
    event_button_press(EVENT_RTE)
end
button_add(nil, nil, function_col_b,function_row_1,width_function,height_function, function_rte_press)

function function_dep_arr_press()
    event_button_press(EVENT_INIT_REF)
end
button_add(nil, nil, function_col_c,function_row_1,width_function,height_function, function_dep_arr_press)

function function_altn_press()
    event_button_press(EVENT_ALTN)
end
button_add(nil, nil, function_col_d,function_row_1,width_function,height_function, function_altn_press)

function function_vnav_press()
    event_button_press(EVENT_VNAV)
end
button_add(nil, nil, function_col_e,function_row_1,width_function,height_function, function_vnav_press)

function function_fix_press()
    event_button_press(EVENT_FIX)
end
button_add(nil, nil, function_col_a,function_row_2,width_function,height_function, function_fix_press)

function function_legs_press()
    event_button_press(EVENT_LEGS)
end
button_add(nil, nil, function_col_b,function_row_2,width_function,height_function, function_legs_press)

function function_hold_press()
    event_button_press(EVENT_HOLD)
end
button_add(nil, nil, function_col_c,function_row_2,width_function,height_function, function_hold_press)

function function_fmc_comm_press()
    event_button_press(EVENT_FMC_COMM)
end
button_add(nil, nil, function_col_d,function_row_2,width_function,height_function, function_fmc_comm_press)

function function_prog_press()
    event_button_press(EVENT_PROG)
end
button_add(nil, nil, function_col_e,function_row_2,width_function,height_function, function_prog_press)

function function_menu_press()
    event_button_press(EVENT_MENU)
end
button_add(nil, nil, function_col_a,function_row_3,width_function,height_function, function_menu_press)

function function_nav_rad_press()
    event_button_press(EVENT_NAV_RAD)
end
button_add(nil, nil, function_col_b,function_row_3,width_function,height_function, function_nav_rad_press)

function function_prev_page_press()
    event_button_press(EVENT_PREV_PAGE)
end
button_add(nil, nil, function_col_a,function_row_4,width_function,height_function, function_prev_page_press)

function function_next_page_press()
    event_button_press(EVENT_NEXT_PAGE)
end
button_add(nil, nil, function_col_b,function_row_4,width_function,height_function, function_next_page_press)

function function_exec_press()
    event_button_press(EVENT_EXEC)
end
button_add(nil, nil, exec_col,exec_row,width_exec,height_exec, function_exec_press)

-- =================== Exec light ==================
local gImgExecLightOff = img_add("led-exec-off.png", 1070, 1155, 100, 25)
local gImgExecLightOn = img_add("led-exec-on.png", 1070, 1155, 100, 25)
msfs_variable_subscribe("L:switch_3491_a", "number", function(v) visible(gImgExecLightOn, v>0) end) -- 0.5 if on