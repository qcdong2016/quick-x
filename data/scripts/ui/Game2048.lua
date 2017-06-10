--[[=============================================================================
#     FileName: MainScene.lua
#         Desc: mainScene for 2048 game
#               full edition in https://github.com/hanxi/quick-cocos2d-x-2048/tree/release
#       Author: hanxi
#        Email: hanxi.info@gmail.com
#     HomePage: http://www.hanxi.info
#      Version: 0.0.1
#   LastChange: 2014-05-09 09:13:11
#      History:
=============================================================================]]

math.randomseed(os.time())
local function printGrid(grid)
    local celllen = 8  -- one cell have character count
    local gridStrLines = {}
    table.insert(gridStrLines,"\n-------------------------------------")
    for i,row in ipairs(grid) do
        local line = {} 
        -- format the number in the center of the cell
        for _,num in ipairs(row) do
            if num==0 then
                local pres = ""
                for tmp=1,celllen do
                    pres = pres .. " "
                end
                local s = string.format("%s",pres)
                table.insert(line,s)
            else
                local s = tostring(num)
                local l = string.len(s)
                local l = (celllen-l)/2
                local prel = math.floor(l)
                local sufl = math.ceil(l)
                local pres = ""
                for tmp=1,prel do
                    pres = pres .. " "
                end
                local sufs = pres
                if sufl>prel then
                    sufs = pres.. " "
                end
                local s = string.format("%s%s%s",pres,s,sufs)
                table.insert(line,s)
            end
        end
        local line = table.concat(line,"|")
        line = "|" .. line .. "|"
        table.insert(gridStrLines,line)
        table.insert(gridStrLines,"-------------------------------------")
    end
    local gridStr = table.concat(gridStrLines,"\n")
    print(gridStr)
end

local function getRandomZeroPos(grid)
    local m = #grid
    local n = #grid[1]
    local zeros = {}
    for i=1,m do
        for j=1,n do
            if grid[i][j]==0 then
                table.insert(zeros,{i=i,j=j})
            end
        end
    end
    if #zeros>0 then
        local r = math.random(1,#zeros)
        return zeros[r].i,zeros[r].j
    end
end

local function randomNum(grid)
    local i,j = getRandomZeroPos(grid)
    if i and j then
        local r = math.random()
        if r<0.9 then
            grid[i][j] = 2
        else
            grid[i][j] = 4
        end
        return i,j
    end
end

local function moveLeft(grid)
    print("==============moveLeft===============")
    local score = 0
    local win = false
    local m = #grid
    local n = #grid[1]
    for i=1,m do
        local line = {}
        for j=1,n do
            if grid[i][j]~=0 then
                table.insert(line,grid[i][j])
            end
        end
        local k=#line
        for j=1,n do
            if j<=k then
                grid[i][j] = line[j]
            else
                grid[i][j] = 0
            end
        end
        for j=1,k-1 do
            if grid[i][j]==grid[i][j+1] then
                grid[i][j+1] = grid[i][j] + grid[i][j+1]
                if grid[i][j+1]==2048 then
                    win = true
                end
                score = score + grid[i][j+1]
                for x=j,n-1 do
                    grid[i][x] = grid[i][x+1]
                end
                grid[i][n] = 0
            end             
        end
    end
    return score,win
end

local function moveRight(grid)
    print("==============moveRight==============")
    local score = 0
    local win = false
	local m = #grid
	local n = #grid[1]
    for i=1,m do
		local line = {}
		for j=n,1,-1 do
			if grid[i][j]~=0 then
				table.insert(line,grid[i][j])
			end
		end
		local k = #line
		for j=n,1,-1 do
			if n-j+1<=k then
				grid[i][j] = line[n-j+1]
			else
				grid[i][j] = 0
			end
		end
		for j=n,n-k+2,-1 do
			if grid[i][j]==grid[i][j-1] then
				grid[i][j-1] = grid[i][j] + grid[i][j-1]
                if grid[i][j-1]==2048 then
                    win = true
                end
                score = score + grid[i][j-1]
				for x=j,2,-1 do
					grid[i][x] = grid[i][x-1]
				end
				grid[i][1] = 0
			end
		end
    end
    return score,win
end


local function moveUp(grid)
    print("===============moveUp================")
    local score = 0
    local win = false
	local m = #grid
	local n = #grid[1]
    for j=1,n do
		local line = {}
		for i=1,m do
			if grid[i][j]~=0 then
				table.insert(line,grid[i][j])
			end
		end
		local k = #line
		for i=1,m do
			if i<=k then
				grid[i][j] = line[i]
			else
				grid[i][j] = 0
			end
		end
		for i=1,k-1 do
			if grid[i][j]==grid[i+1][j] then
				grid[i+1][j] = grid[i][j] + grid[i+1][j]
                if grid[i+1][j]==2048 then
                    win = true
                end
                score = score + grid[i+1][j]
				for x=i,m-1 do
					grid[x][j] = grid[x+1][j]
				end
				grid[m][j] = 0
			end				
		end
    end
    return score,win
end

local function moveDown(grid)
    print("==============moveDown===============")
    local score = 0
    local win = false
	local m = #grid
	local n = #grid[1]
    for j=1,n do
		local line = {}
		for i=m,1,-1 do
			if grid[i][j]~=0 then
				table.insert(line,grid[i][j])
			end
		end
		local k = #line
		for i=m,1,-1 do
			if m-i+1<=k then
				grid[i][j] = line[m-i+1]
			else
				grid[i][j] = 0
			end
		end
		for i=m,m-k+2,-1 do
			if grid[i][j]==grid[i-1][j] then
				grid[i-1][j] = grid[i][j] + grid[i-1][j]
                if grid[i-1][j]==2048 then
                    win = true
                end
                score = score + grid[i-1][j]
				for x=i,2,-1 do
					grid[x][j] = grid[x-1][j]
				end
				grid[1][j] = 0
			end
		end
    end
    return score,win
end

local function copyGrid(grid)
    local m = #grid
    local n = #grid[1]
    local tmpGrid = {}
    for i=1,m do
        if not tmpGrid[i] then
            tmpGrid[i] = {}
        end
        for j=1,n do
            tmpGrid[i][j] = grid[i][j]
        end
    end
    return tmpGrid
end

local function getOpList(beforeGrid,grid)
    local m = #grid
    local n = #grid[1]
    local op_list = {}
    for i=1,m do
        for j=1,n do
            if beforeGrid[i][j]~=grid[i][j] then
                table.insert(op_list,{'setnum',i,j,grid[i][j]})
            end
        end
    end
    return op_list
end

-- serialize lua table. for save lua table in file
function serialize(t)
    local mark={}
    local assign={}
    
    local function ser_table(tbl,parent)
        mark[tbl]=parent
        local tmp={}
        for k,v in pairs(tbl) do
            local key= type(k)=="number" and "["..k.."]" or k
            if type(v)=="table" then
                local dotkey= parent..(type(k)=="number" and key or "."..key)
                if mark[v] then
                    table.insert(assign,dotkey.."="..mark[v])
                else
                    table.insert(tmp, key.."="..ser_table(v,dotkey))
                end
            else
                table.insert(tmp, key.."="..v)
            end
        end
        return "{"..table.concat(tmp,",").."}"
    end
 
    return ser_table(t,"ret")..table.concat(assign," ")
end

function initGrid(m,n)
    local grid = {}
    for i=1,m do
        if not grid[i] then
            grid[i] = {}
        end
        for j=1,n do
            grid[i][j] = 0
        end
    end
    randomNum(grid)
    randomNum(grid)
    return grid
end

function canMove(grid)
	local m = #grid
	local n = #grid[1]
    for i=1,m do
        for j=1,n do
            if grid[i][j]==0 then
                return true
            end
            if (i<m and grid[i][j]==grid[i+1][j] )
                or (j<n and grid[i][j]==grid[i][j+1]) then
                return true
            end
        end
    end
    return false
end

local ops = {
    left  = moveLeft,
    right = moveRight,
    up    = moveUp,
    down  = moveDown,
}
function touch_op(grid,op)
    local beforeGrid = copyGrid(grid)
    local score,win = ops[op](grid)
    randomNum(grid)
    printGrid(grid)
    local op_list = getOpList(beforeGrid,grid)
    return op_list,score,win
end

-------------------------------------------------------------------------------
-- for lua console 2048
-------------------------------------------------------------------------------
local function main()
    local grid = initGrid(4,4)
    randomNum(grid)
    printGrid(grid)
    io.write("next step 'a'[←],'w'[↑],'s'[↓],'d'[→],'q'[exit] >> ")
    local input = io.read()
    while input~="q" do
        if input=="a" or input=="w" or input=="s" or input=="d" then
            if input=="a" then
                moveLeft(grid)
            elseif input=="w" then
                moveUp(grid)
            elseif input=="s" then
                moveDown(grid)
            elseif input=="d" then
                moveRight(grid)
            end
            randomNum(grid)
            printGrid(grid)
        else
            print("error input. please input 'a'[←] or 'w'[↑] or 's'[↓] or 'd'[→] or 'q'[exit]")
        end
        io.write("next step 'a'[←],'w'[↑],'s'[↓],'d'[→],'q'[exit] >> ")
        input = io.read()
    end
end

-- uncomment this. you can in console run command to play console 2048. "$ lua logic.lua"
-- main()
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

local MainScene = class('SpriteTest', function()
    return display.newLayer()
end)

local totalScore = 0
local bestScore = 0
local WINSTR = ""
local touchStart={0,0}
local configFile = device.writablePath.."hxgame.config"

local function doOpList(op_list)
    for _,op in ipairs(op_list or {}) do
        local o = op[1]
        if o=='setnum' then
            local i,j,num = op[2],op[3],op[4]
            setnum(gridShow[i][j],num,i,j)
        end
    end
end

function getPosFormIdx(i,j)
    local cellsize=110   -- cell size
    local x = display.cx - cellsize * 2
    local y = display.cy + cellsize * 2
    return x + j * cellsize, y -i * cellsize
end

local colors = {
    [-1]   = ccc4(0xee, 0xe4, 0xda, 100),
    [0]    = ccc3(0xee, 0xe4, 0xda),
    [2]    = ccc3(0xee, 0xe4, 0xda),
    [4]    = ccc3(0xed, 0xe0, 0xc8),
    [8]    = ccc3(0xf2, 0xb1, 0x79),
    [16]   = ccc3(0xf5, 0x95, 0x63),
    [32]   = ccc3(0xf6, 0x7c, 0x5f),
    [64]   = ccc3(0xf6, 0x5e, 0x3b),
    [128]  = ccc3(0xed, 0xcf, 0x72),
    [256]  = ccc3(0xed, 0xcc, 0x61),
    [512]  = ccc3(0xed, 0xc8, 0x50),
    [1024] = ccc3(0xed, 0xc5, 0x3f),
    [2048] = ccc3(0xed, 0xc2, 0x2e),
    [4096] = ccc3(0x3c, 0x3a, 0x32),
}
local numcolors = {
    [0] = ccc3(0x77,0x6e,0x65),
    [2] = ccc3(0x77,0x6e,0x65),
    [4] = ccc3(0x77,0x6e,0x65),
    [8] = ccc3(0x77,0x6e,0x65),
    [16] = ccc3(0x77,0x6e,0x65),
    [32] = ccc3(0x77,0x6e,0x65),
    [64] = ccc3(0x77,0x6e,0x65),
    [128] = ccc3(0x77,0x6e,0x65),
}

function setnum(self,num,i,j)
    local s = tostring(num)
    --s = s.."("..i..","..j..")"
    if s=='0' then
        s=''
        self.background:setOpacity(100)
    else
        self.background:setOpacity(255)
    end
    local c=colors[num]
    if not c then
        c=colors[4096]
    end
    self.num:setString(s)
    self.background:setColor(c)
    local nc = numcolors[num]
    if not nc then
        nc = numcolors[128]
    end
    self.num:setColor(nc)
end

function saveStatus()
    local gridstr = serialize(grid)
    local isOverstr = "false"
    if isOver then isOverstr = "true" end
    local str = string.format("do local grid,bestScore,totalScore,WINSTR,isOver \
                              =%s,%d,%d,\'%s\',%s return grid,bestScore,totalScore,WINSTR,isOver end",
                              gridstr,bestScore,totalScore,WINSTR,isOverstr)
    io.writefile(configFile,str)
end

function MainScene:loadStatus()
    if io.exists(configFile) then
        local str = io.readfile(configFile)
        if str then
            local f = loadstring(str)
            local _grid,_bestScore,_totalScore,_WINSTR,_isOver = f()
            if _grid and _bestScore and _totalScore and _WINSTR then
                grid,bestScore,totalScore,WINSTR,isOver = _grid,_bestScore,_totalScore,_WINSTR,_isOver
            end
        end
    end
    self:reLoadGame()
end

function MainScene:createLabel(title)
    self.scoreLabel = ui.newTTFLabel({
        text = "SCORE:0",
        size = 30,
        color = display.COLOR_BLUE,
    })
    self.scoreLabel:align(display.CENTER,display.cx,display.top - 100):addTo(self)
end

function MainScene:onTouch(event, x, y)
    if isOver then
        return true
    end

    if event=='began' then
        touchStart={x,y}
    elseif event=='ended' then
        local tx,ty=x-touchStart[1],y-touchStart[2]
        if tx==0 then
            tx = tx+1
            ty = ty+1
        end
        local dis = tx*tx+ty*ty
        if dis<3 then   -- touch move too short will ignore
            return true
        end
        local dt = ty/tx
        local op_list,score,win
        if dt>=-1 and dt<=1 then
            if tx>0 then
                op_list,score,win = touch_op(grid,'right')
            else
                op_list,score,win = touch_op(grid,'left')
            end
        else
            if ty>0 then
                op_list,score,win = touch_op(grid,'up')
            else
                op_list,score,win = touch_op(grid,'down')
            end
        end
        doOpList(op_list)
        if win then
            WINSTR = "YOUR ARE WINER"
        end
        totalScore = totalScore + score
        if totalScore>bestScore then
            bestScore = totalScore
        end
        self:updateScore()
        isOver = not canMove(grid)
        saveStatus()
    end
    return true
end

function MainScene:createGridShow()
    gridShow = {}
    for tmp=0,15 do
        local i,j = math.floor(tmp/4)+1,math.floor(tmp%4)+1
        local num = grid[i][j]
        local s = tostring(num)
        --s = s.."("..i..","..j..")"
        if s=='0' then
            s=''
        end
        if not gridShow[i] then
            gridShow[i] = {}
        end
        local cell = {
            backgroundsize = 100,
            background = CCLayerColor:create(colors[-1], 100, 100),
            num = ui.newTTFLabel({
                text = s,
                size = 40,
                color = numcolors[0],
            }),
        }
        gridShow[i][j] = cell

        local x,y = getPosFormIdx(i,j)
        local bsz = cell.backgroundsize/2
        cell.background:setPosition(ccp(x-bsz,y-bsz))
        cell.num:addTo(cell.background)
        self:addChild(cell.background)
    end

end

function MainScene:reLoadGame()
    local m = #grid
    local n = #grid[1]
    for i=1,m do
        for j=1,n do
            setnum(gridShow[i][j],grid[i][j],i,j)
        end
    end
    self:updateScore()
end

function MainScene:updateScore()
    self.scoreLabel:setString(string.format("BEST:%d  SCORE:%d %s",bestScore,totalScore,WINSTR or ""))
end

function MainScene:restartGame()
    grid = initGrid(4,4)
    totalScore = 0
    WINSTR = ""
    isOver = false
    self:reLoadGame()
    saveStatus()
end

function MainScene:ctor()
    local layer = display.newColorLayer(ccc4(0xfa,0xf8,0xef, 255)):addTo(self):center()
    layer:setSize(CCSize(display.width, display.height))
    WINSTR = ""
    grid = initGrid(4,4)

    self:createLabel("2048")
    self:createGridShow()

    self:loadStatus()
    if isOver then
        self:restartGame()
    end

    self:setTouchEnabled(true)
    self:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
        return self:onTouch(event.name, event.x, event.y)
    end)
end

local M = class('ShaderTest', TestBase)

function M:setup()
    self.game = MainScene:new():addTo(self):pos(-display.cx, -display.cy)

    local btn = Button:create():addTo(self):scale(0.5)
    btn:loadTextureNormal('AllSprites/AddCoinButton.png')
    btn:setPosition(ccp(display.right - 40, display.top -40))
    btn:onClicked(function()
        self.game:restartGame()
    end)
end


return M
