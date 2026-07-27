img_add_fullscreen("oil_backdrop.png")
img_neddle_temp = img_add_fullscreen("temperature_needle.png")
img_neddle_press = img_add_fullscreen("pressure_needle.png")
img_add_fullscreen("needle_cover.png")

-- Functions
function PT_oiltemp(oiltemp)
    
    oiltemp = var_cap(oiltemp, -40, 140)
    
    if oiltemp <= 10 then -- 
       rotate(img_neddle_temp, (23 / 50) * oiltemp * -1)
    elseif oiltemp > 10 and oiltemp <= 55 then 
       rotate(img_neddle_temp, (23 / 45) * oiltemp * -1)
    elseif oiltemp > 55 and oiltemp <= 110 then 
       rotate(img_neddle_temp, (((oiltemp - 55) * (30/55)) * -1) - 28)
    else 
       rotate(img_neddle_temp, (((oiltemp - 110) * (10/30)) * -1) - 58)
    end

end

function PT_oilpress(oilpress)

    oilpress = var_cap(oilpress, 0, 120)
    
    if oilpress <= 40 then -- travel 40 PSI within 44 degrees
       rotate(img_neddle_press, (44 / 40) * oilpress)       
    elseif oilpress > 40 and oilpress <= 85 then -- travel 45 PSI within 66 degrees    
       rotate(img_neddle_press, ((oilpress - 40) * (64/45)) + 44)    
    else -- travel 35 PSI within 51 degrees
       rotate(img_neddle_press, ((oilpress - 85) * (51/35)) + 44 + 65)
    end
end

-- Bus subscribe --
msfs_variable_subscribe("A:ENG OIL PRESSURE:1", "PSI", PT_oilpress)
msfs_variable_subscribe("L:BKSQ_CARAVAN_OILTEMPERATURE", "Celsius", PT_oiltemp)
