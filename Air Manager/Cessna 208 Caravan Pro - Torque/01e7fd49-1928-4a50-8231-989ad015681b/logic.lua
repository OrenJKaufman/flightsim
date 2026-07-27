img_add_fullscreen("torque_backdrop.png")
img_needle = img_add_fullscreen("engine_needle.png")

function PT_torque_FSX(torque)

    -- Limit needle to max 2500
    torque = var_cap(torque, 0, 2500)
    --Divide by 10 for scaling
    torque = torque / 10
    
    -- 250 divided by 265 degress turn. 
    rotate(img_needle, (265/250) * torque)

end

msfs_variable_subscribe("L:BKSQ_CARAVAN_TQ", "Number", PT_torque_FSX)