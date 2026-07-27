-- PMDG 737-700 FS2020 Nav Radio

-- Detlef von Reusner

-- Dec 2 2022

-- Illumination options per user properties: Day,Night,Real.
-- Ambient Light Dimmer Instrument needed for option "Real". (From SIMSTRUMENTATION)
-- You may set the Userprops in the Ambient Light Dimmer Instrument to "Hide and autodimm".
-- See also comments in lib file: illumination.lua


-- July 14 2022 - Initial Version
-- July 20 2022 - Nav2 user prop added
-- July 24 2022 - on only if avionics bus energized
-- Sep 27 2022 - Illumination changed
-- Dec 2 2022: Illumination updated (Background only)




local gPropNavSelect = user_prop_get(user_prop_add_enum("NAV radio","NAV1,NAV2", "NAV1", "Select radio"))

local knobIndex = 2
if gPropNavSelect == "NAV2" then
  knobIndex = 3
end
  
gSelect = {}

if gPropNavSelect == "NAV1" then
  gSelect.event_inc_whole = 73307
  gSelect.event_dec_whole = 73308
  gSelect.event_inc_fract = 73207
  gSelect.event_dec_fract = 73208
  gSelect.event_swap = 72901
  gSelect.lvar_active = "NAV ACTIVE FREQUENCY:1"
  gSelect.lvar_standby = "NAV STANDBY FREQUENCY:1"
else
  gSelect.event_inc_whole = 84807
  gSelect.event_dec_whole = 84808
  gSelect.event_inc_fract = 84907
  gSelect.event_dec_fract = 84908
  gSelect.event_swap = 84501
  gSelect.lvar_active = "NAV ACTIVE FREQUENCY:2"
  gSelect.lvar_standby = "NAV STANDBY FREQUENCY:2"
end

function UpdateRadioKnobIndex()
  fs2024_variable_write("L:OREN_RADIO_KNOB_INDEX", "number", knobIndex)
end


local gBgDay = img_add_fullscreen("bg-day.png")


local gColorDisplay = string.format("#%02x%02x%02x", 246, 148, 49)


local gColorText = string.format("#%02x%02x%02x", 237, 237, 237)


-- ================================== global variables ============================

local gFreq = {}
gFreq.active = 0
gFreq.standby = 0

local gImgButtonReleased
local gButtonText

-- =============================== Battery Energy =====================

local gEnergized = false
msfs_variable_subscribe("ELECTRICAL AVIONICS BUS VOLTAGE", "number", function(v)
  gEnergized = (v>12)
  ShowDisplays()
end)

-- ================================== Frequency knob ====================

button_add("outer-knob.png", nil, 253, 84, 60, 60, UpdateRadioKnobIndex)
button_add("inner-knob.png", nil, 262, 94, 41, 41, UpdateRadioKnobIndex)


-- ======================== image order needed for illumination ===============

local gLightMask = img_add_fullscreen("sun-mask.png")
local gBgNight = img_add_fullscreen("bg-night.png")

local gTxtActiveP1 = txt_add("987", "font:digital-7-mono.ttf; size:27;color:"..gColorDisplay..";halign:left;", 42, 22, 60, 30)
local gTxtActiveP2 = txt_add("000", "font:digital-7-mono.ttf; size:27;color:"..gColorDisplay..";halign:left;", 91, 22, 60, 30)

local gTxtStandbyP1 = txt_add("328", "font:digital-7-mono.ttf; size:27;color:"..gColorDisplay..";halign:left;", 206, 22, 120, 30)
local gTxtStandbyP2 = txt_add("610", "font:digital-7-mono.ttf; size:27;color:"..gColorDisplay..";halign:left;", 255, 22, 120, 30)

local gImgDotActive = canvas_add(85, 42, 6, 6, function()
  _circle(0, 0, 3)
  _fill(gColorDisplay)
end)
local gImgDotStandby = canvas_add(249, 42, 6, 6, function()
  _circle(0, 0, 3)
  _fill(gColorDisplay)
end)

-- ========================= Button functions ===========================================

function ButtonPressed()
  msfs_event("ROTOR_BRAKE", gSelect.event_swap)
  visible(gImgButtonReleased, false)
  move(gButtonText, 66, 31)
end

function ButtonReleased()
  visible(gImgButtonReleased, true)
  move(gButtonText, 66, 30)
end

img_add("button-pressed.png", 150, 28, 36, 18)
gImgButtonReleased = img_add("button-released.png", 150, 28, 36, 18)
gButtonText = img_add("swap-arrow.png", 0, 0, 130, 15)
button_add(nil, nil, 140, 15, 60, 35, ButtonPressed, ButtonReleased)
ButtonReleased()

--button_add(nil, nil, 282, 114, 20, 20, ButtonPressed, ButtonReleased)
-- ============================ Displays ==============================
function toint(n)
  -- needed because math.floor() returns floats
    local s = tostring(n)
    local i, j = s:find('%.')
    if i then
        return tonumber(s:sub(1, i-1))
    else
        return n
    end
end


function MakeFrequencyString(v)
  local p1, p2
  v = (v+1)/1000000
  p1 = toint(math.floor(v))
  p2 = toint(math.floor((v-p1)*1000))
  return string.format("%03d", p1), string.format("%03d", p2)
end

function ShowDisplays()
  local p1, p2

  if not gEnergized then
    visible(gImgDotActive, false)
    visible(gImgDotStandby, false)
    txt_set(gTxtActiveP1, "")
    txt_set(gTxtActiveP2, "")

    txt_set(gTxtStandbyP1, "")
    txt_set(gTxtStandbyP2, "")
    return
  end

  visible(gImgDotActive, true)
  visible(gImgDotStandby, true)
  p1, p2 = MakeFrequencyString(gFreq.active)
  txt_set(gTxtActiveP1, p1)
  txt_set(gTxtActiveP2, p2)

  p1, p2 = MakeFrequencyString(gFreq.standby)
  txt_set(gTxtStandbyP1, p1)
  txt_set(gTxtStandbyP2, p2)
end

function cbFreqChange(a, b)
  gFreq.standby = a
  gFreq.active = b
  ShowDisplays()
end

msfs_variable_subscribe(
  gSelect.lvar_standby, "number", 
  gSelect.lvar_active, "number",
  cbFreqChange)

-- ================== illumination ========================

local gDebugIllumination = false

if gDebugIllumination then
  debug_bg = canvas_add(4, 15, 396, 70, function()
    _rect(0, 0, 396, 70)
    _fill("#00000080")
  end)
end
local gTxtDebug = txt_add("", "font:arimo_bold.ttf;color:white;size:16", 2, 25, 400, 20)
local gTxtDebug2 = txt_add("", "font:arimo_bold.ttf;color:white;size:16", 2, 40, 400, 20)
local gTxtDebug3 = txt_add("", "font:arimo_bold.ttf;color:white;size:16", 2, 55, 400, 20)

function DrawAllCanvas(color_print, r, g, b, switch_brightness)
  gColorPrint = color_print
  -- canvas_draw(gImgLines, DrawAllLines)
  -- canvas_draw(gImgTexts, DrawAllTexts)
  -- print(r.."  "..g.."  "..b)
  gSwitchColorHi = string.format("#%.2x%.2x%.2xff", r//1, g//1, b//1)
  gSwitchColorLo = string.format("#%.2x%.2x%.2xff", (r*.9)//1, (g*.9)//1, (b*.9)//1)
  -- DrawSwitches()
end

ControlIllumination(DrawAllCanvas, gBgNight, gLightMask, gDebugIllumination, gTxtDebug, gTxtDebug2, gTxtDebug3)
