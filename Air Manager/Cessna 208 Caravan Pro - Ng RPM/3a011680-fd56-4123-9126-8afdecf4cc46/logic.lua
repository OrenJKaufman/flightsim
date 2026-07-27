img_add_fullscreen("ng_backdrop.png")
img_needle_big = img_add_fullscreen("engine_needle.png")
img_needle_small = img_add("small_needle.png",0,138,500,500)

function PT_ng_FSX(rpm_percent)
    
    rpm_percent = var_cap(rpm_percent, 0, 104)

    -- Turn the needle 27 degress for every 10% 
    rotate(img_needle_big, 270 / 100 * rpm_percent)
    -- fraction of the percentage to be turned 36 degrees for every 1 fraction
    rotate(img_needle_small, 360 / 10 * rpm_percent)

end

-- Bus subscribe --
msfs_variable_subscribe("L:BKSQ_CARAVAN_NG", "Percent", PT_ng_FSX)