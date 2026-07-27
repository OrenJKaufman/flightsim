img_add_fullscreen("ff_backdrop.png")
img_needle_big = img_add_fullscreen("engine_needle.png")

function PT_ff_FSX(ff)
    
    ff = ff / 14.67 -- gauge scale
    ff = var_cap(ff, 0, 5)
    
    -- Turn the needle 46 degress for every 10  
    rotate(img_needle_big, 46 * ff)

end

-- Bus subscribe --
msfs_variable_subscribe("L:BKSQ_CARAVAN_FuelFlow", "Number", PT_ff_FSX)
