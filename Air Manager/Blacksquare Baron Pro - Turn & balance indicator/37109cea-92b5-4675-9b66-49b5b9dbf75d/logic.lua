-- Load and display text and images
img_add_fullscreen("turnback.png")
img_ball             = img_add("ball.png", 231,335,37,37)
img_background       = img_add_fullscreen("background.png")
img_plane            = img_add_fullscreen("turnbalanceairplane.png")
img_background_black = img_add_fullscreen("background_black.png")

-- Callback functions (handles data received from X-plane)

function new_ball_deflection(slipArg)
    slip = (slipArg + 500) / 500 * 8.1
    slip_rad = math.rad(slip * 2.65)
    
    x = (0 * math.cos(slip_rad)) - (300 * math.sin(slip_rad))
    y = (0 * math.sin(slip_rad)) + (300 * math.cos(slip_rad))
    
    move(img_ball, x + 231,y + 35,nil,nil)
end

function new_turnrate(roll)
    roll = var_cap(roll, -45, 45)
    rotate(img_plane, roll)
end

function new_ball_deflection_fsx(slip)
    
    slip = slip * -10
    new_ball_deflection(slip)
    
end

function new_turnrate_fsx(roll)
    
    roll = roll * 360
    new_turnrate(roll)
    
end

-- subscribe functions on the AirBus
msfs_variable_subscribe("L:BKSQ_TurnCoordinatorBall", "Number", new_ball_deflection_fsx)
msfs_variable_subscribe("TURN INDICATOR RATE", "Radians", new_turnrate_fsx)