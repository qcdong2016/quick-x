
function ctrl_1(name, get, set, func, arg1, arg2)
    return function()
        local c, v1 = func(name, get(), arg1, arg2)
        if c then
            set(v1)
        end
    end
end
function ctrl_2(name, get, set, func, arg1, arg2)
    return function()

        local a1,a2 = get()

        local c, v1,v2 = func(name, a1,a2, arg1, arg2)
        if c then
            set(v1,v2)
        end
    end
end
function ctrl_3(name, get, set, func, arg1, arg2)
    return function()
        local a1,a2,a3 = get()
        local c, v1,v2,v3 = func(name, a1,a2,a3, arg1, arg2)
        if c then
            set(v1,v2,v3)
        end
    end
end
function ctrl_4(name, get, set, func, arg1, arg2)
    return function()
        local a1,a2,a3,a4 = get()
        local c, v1,v2,v3,v4 = func(name, a1,a2,a3,a4, arg1, arg2)
        if c then
            set(v1,v2,v3,v4)
        end
    end
end
function editor(name, node, get_name, set_name, ctrl, arg1, arg2)
    local get, set
    if type(get_name) == 'function' then
        get = get_name
    else
        get = node[get_name]
    end

    if type(set_name) == 'function' then
        set = set_name
    else
        set = node[set_name]
    end

    return ctrl(name,
        function() return get(node) end,
        function(...) return set(node, ...) end,
        arg1, arg2)
end

function int_ctrl(name, get, set, step)   return ctrl_1(name, get, set, imgui.InputInt, step) end
function bool_ctrl(name, get, set)        return ctrl_1(name, get, set, imgui.Checkbox) end
function combo_ctrl(name, get, set, data) return ctrl_1(name, get, set, imgui.Combo, data) end
function float_ctrl(name, get, set, step) return ctrl_1(name, get, set, imgui.InputFloat, step) end
function float2_ctrl(name, get, set)      return ctrl_2(name, get, set, imgui.InputFloat2) end
function float3_ctrl(name, get, set)      return ctrl_3(name, get, set, imgui.InputFloat3) end
function float4_ctrl(name, get, set)      return ctrl_4(name, get, set, imgui.InputFloat4) end
function color3_ctrl(name, get, set)      return ctrl_3(name, get, set, imgui.ColorEdit3) end
